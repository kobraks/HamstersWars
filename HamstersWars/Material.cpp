#include "Material.h"

model::Material::Material(const aiColor4D& color, const float& specular_intensity, const float& specular_power,
                          std::shared_ptr<Texture2d> texture) : color(color.r, color.g, color.b, color.a),
                                                                specular_intensity(specular_intensity),
                                                                specular_power(specular_power), texture(texture)
{
}

model::Material::Material(const glm::vec4& color, const float& specular_intensity, const float& specular_power,
                          std::shared_ptr<Texture2d> texture) : color(color), specular_intensity(specular_intensity),
                                                                specular_power(specular_power)
{
}
