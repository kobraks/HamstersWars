#version 120

uniform sampler2D mytex; 
uniform int mode;
uniform vec4 color = vec4(1.f, 1.f, 1.f, 1.f);

varying vec2 texCoord; 
varying vec4 outColor;
varying vec3 normal;
varying vec4 position;

vec4 textured()
{
	vec4 result = texture2D(mytex, texCoord.st);

	return result * outColor;
}

vec4 no_textures()
{
	return outColor;
}

vec4 no_color()
{
	return color;
}

void main() 
{
	vec4 result;
	if (mode == 1)
		result = textured();
	else if (mode == 0)
		result = no_textures();
	else if (mode == -1)
		result = no_color();
   
	gl_FragColor = result;
}