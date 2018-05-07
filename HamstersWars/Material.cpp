#include "Material.h"



model::Material::Material() : specular_intensity(0), specular_power(0)
{
}

model::Material::Material(aiColor4D color, float specular_intensity, float specular_power, Texture2d* texture) :
	color(color.r, color.g, color.b), specular_intensity(specular_intensity), specular_power(specular_power),
	texture(texture)
{

}


model::Material::~Material()
{
}
