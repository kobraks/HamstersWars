#version 120

uniform sampler2D mytex; 

varying vec2 texCoord; 
varying vec4 outColor;
 
void main() 
{ 
    vec4 color = texture2D(mytex, texCoord.st);
    gl_FragColor = color * outColor;
}