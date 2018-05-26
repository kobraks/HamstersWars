#include "Texture2d.h"


model::Texture2d::Texture2d(const unsigned& width, const unsigned& height, const gl::Texture& texture) : width_(width), height_(height), texture_(texture)
{

}

model::Texture2d::operator unsigned() const
{
	return texture_;
}

void model::Texture2d::bind()
{
	glBindTexture(GL_TEXTURE_2D, texture_);
}
