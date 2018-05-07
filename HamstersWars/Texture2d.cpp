#include "Texture2d.h"


model::Texture2d::Texture2d(const unsigned& width, const unsigned& height, const GLuint& texture_id) : width_(width), height_(height), texture_id_(texture_id)
{

}

model::Texture2d::~Texture2d()
{
	glDeleteTextures(1, &texture_id_);
}

model::Texture2d::operator unsigned() const
{
	return texture_id_;
}

void model::Texture2d::bind()
{
	glBindTexture(GL_TEXTURE_2D, texture_id_);
}
