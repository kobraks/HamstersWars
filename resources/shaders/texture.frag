#version 120

uniform sampler2D mytex; 
uniform int mode;
uniform vec4 forced_color = vec4(1.f, 1.f, 1.f, 1.f);

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
	return forced_color;
}

void main() 
{
	vec4 color;
	if (mode == 1)
		color = textured();
	else if (mode == 0)
		color = no_textures();
	else if (mode == -1)
		color = no_color();
   
	gl_FragColor = color;
}