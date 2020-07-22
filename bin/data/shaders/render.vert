#version 120

#pragma include "util.frag"

uniform sampler2DRect u_posAndAgeTex;
uniform sampler2DRect u_velAndMaxAgeTex;

varying vec2 v_TexCoord;
varying vec4 v_Color;

void main() {
    vec2 st = gl_MultiTexCoord0.xy;
    vec3 pos = texture2DRect( u_posAndAgeTex, st ).xyz;
    vec3 vel = texture2DRect( u_velAndMaxAgeTex, st ).xyz;
    v_TexCoord = st;
    
    gl_PointSize = 1.0;
    gl_Position = gl_ModelViewProjectionMatrix * vec4(pos, 1.0);
    v_Color = vec4(normalize(vel), 0.7);
}



