#version 120
#pragma include "util.frag"
#pragma include "noise4D.frag"

uniform sampler2DRect u_posAndAgeTex;
uniform sampler2DRect u_velAndMaxAgeTex;
uniform sampler2DRect u_diffTex;

uniform float u_time;

void main(void){
    vec2 st = gl_TexCoord[0].st;
    vec4 posAndAge = texture2DRect(u_posAndAgeTex,st);
    vec4 velAndMaxAge = texture2DRect(u_velAndMaxAgeTex,st);
    
    vec3 pos = posAndAge.xyz;
    vec3 vel = velAndMaxAge.xyz;
    float age = posAndAge.w;
    float maxAge = velAndMaxAge.w;
    vec3 diff = texture2DRect(u_diffTex, st).xyz;
    age++;
    
    if(age > maxAge){
        if(length(diff) > 0.2){
            age = 0;
            pos = vec3(st.x, st.y, 0.0);
            vel.y = -5.0;
        }
    }
    
    vel.x += 10.0*snoise(vec4(pos.x*0.0234, pos.y*0.01, pos.z*0.01, u_time*0.05)) + 10*sin(u_time);
    vel.y += 6.0*snoise(vec4(pos.x*0.01, pos.y*0.0823, pos.z*0.01, u_time*0.05)) - age*age*age*0.01;


    pos += vel;
    
    gl_FragData[0].rgba = vec4(pos, age);
    gl_FragData[1].rgba = vec4(vel, maxAge);
    
}
