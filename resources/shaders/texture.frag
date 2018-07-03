#version 330

uniform sampler2D my_texture; 
uniform int mode;
uniform vec4 color = vec4(1.f, 1.f, 1.f, 1.f);

in vec2 texCoord; 
in vec4 outColor;
in vec3 normal;
out vec4 frag_color;

vec4 textured()
{
	vec4 result = vec4(texture(my_texture, texCoord.st).rgb, 1.f);

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
   
	//gl_FragColor = result;

	frag_color = result;
	//frag_color = vec4(1.f, 1.f, 1.f, 1.f);
}