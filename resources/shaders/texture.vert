#version 120

uniform int mode;
uniform mat4 model;

attribute vec3 inPosition;
attribute vec4 inColor;
attribute vec2 inTexcoord;
attribute vec3 inNormal;


varying vec2 texCoord; 
varying vec4 outColor;
varying vec3 normal;
varying vec4 position;

void main()
{
	texCoord = inTexcoord;
	outColor = inColor;

	position = model * vec4(inPosition.xyz, 1);

	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * position;

	normal = normalize(gl_NormalMatrix * inNormal);
}