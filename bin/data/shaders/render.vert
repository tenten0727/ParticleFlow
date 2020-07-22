#version 120

#pragma include "util.frag"
#pragma include "noise4D.frag"

uniform sampler2DRect u_posAndAgeTex;
uniform sampler2DRect u_velAndMaxAgeTex;
uniform float u_time;

varying vec2 v_TexCoord;
varying vec4 v_Color;

void main() {
    vec2 st = gl_MultiTexCoord0.xy;
    vec3 pos = texture2DRect( u_posAndAgeTex, st ).xyz;
    vec3 vel = texture2DRect( u_velAndMaxAgeTex, st ).xyz;
    float age = texture2DRect( u_posAndAgeTex, st ).z;
    v_TexCoord = st;
    
    gl_PointSize = 1.0;
    gl_Position = gl_ModelViewProjectionMatrix * vec4(pos, 1.0);
    float r = 1.5*snoise(vec4(pos.x * 0.01, 0, 0, age*0.01))*gl_Color.r + 0.1;
    float g = 1.5*snoise(vec4(0, pos.y * 0.01, 0, age*0.01))*gl_Color.g + 0.1;
    float b = 1.5+snoise(vec4(0, 0, pos.z * 0.01, age*0.01))*gl_Color.b + 0.5;
    v_Color = vec4(r, g, b, gl_Color.a);
}



