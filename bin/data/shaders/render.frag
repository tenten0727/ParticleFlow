#version 120

varying vec2 v_TexCoord;
varying vec4 v_Color;

void main() {
    
    gl_FragColor = v_Color;
}
