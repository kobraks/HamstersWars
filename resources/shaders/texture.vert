//vertex shader
#version 330

uniform int mode;
uniform mat4 Model;
uniform mat4 Projection;
uniform mat4 View;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 inTexcoord;
layout(location = 3) in vec3 inNormal;


out vec2 texCoord; 
out vec4 outColor;
out vec3 normal;

void main()
{
	texCoord = inTexcoord;
	outColor = inColor;
	
	normal = inNormal;
	normal = normalize(mat3x3(Model) * inNormal);

	gl_Position = Projection * View * Model * vec4(inPosition, 1);
}