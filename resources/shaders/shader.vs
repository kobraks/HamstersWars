#version 120
attribute vec3 inPosition;
attribute vec4 inColor;
attribute vec2 inTexcoord; 
attribute vec3 inNormal; 

varying vec2 texCoord; 
varying vec3 outNormal; 
varying vec4 outColor;
 
void main() 
{    
    gl_Position = gl_ModelViewProjectionMatrix * vec4(inPosition.xyz,1); 
    gl_TexCoord[0].st = inTexcoord; 
    texCoord = inTexcoord; 
    outNormal = normalize(gl_NormalMatrix * inNormal);
	outColor = inColor;
} 