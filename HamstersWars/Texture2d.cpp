#include "Texture2d.h"
#include <utility>


model::Texture2d::
Texture2d(const unsigned& width, const unsigned& height, std::shared_ptr<gl::Texture> texture) : width_(width),
                                                                                                        height_(height),
                                                                                                        texture_(
	                                                                                                        std::move(
		                                                                                                        texture))
{

}

model::Texture2d::operator unsigned() const
{
	return *texture_;
}

void model::Texture2d::bind()
{
	glBindTexture(GL_TEXTURE_2D, *texture_);
}
