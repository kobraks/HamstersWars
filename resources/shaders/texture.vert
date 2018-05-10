#version 120
attribute vec3 inPosition;
attribute vec4 inColor;
attribute vec2 inTexcoord; 


varying vec2 texCoord; 
varying vec4 outColor;
 
void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	texCoord = inTexcoord;
	outColor = inColor;
}