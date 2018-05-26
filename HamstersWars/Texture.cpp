#include "Texture.h"


#define PUSHSTATE() GLint restoreId; glGetIntegerv(GL_TEXTURE_BINDING_2D, &restoreId);
#define POPSTATE() glBindTexture( GL_TEXTURE_2D, restoreId );

gl::Texture::Texture()
{
	obj_ = std::shared_ptr<GLuint>(new GLuint(0), [=](GLuint* id) {glDeleteTextures(1, id); });
}

gl::Texture::Texture(const GLubyte* pixels, unsigned int width, unsigned int height, internal_format::internal_format_t internal_format) : Texture()
{
	PUSHSTATE()

	glGenTextures(1, &*obj_);
	glBindTexture(GL_TEXTURE_2D, *obj_);
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format::RGBA, data_type::UnsignedByte, pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenerateMipmap(GL_TEXTURE_2D);

	POPSTATE()
}

gl::Texture::Texture(const util::Image& image, internal_format::internal_format_t internal_format) : Texture()
{
	PUSHSTATE()

	glGenTextures(1, &*obj_);
	glBindTexture(GL_TEXTURE_2D, *obj_);
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, image.width(), image.height(), 0, format::RGBA, data_type::Float, image.get_pixels());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenerateMipmap(GL_TEXTURE_2D);

	POPSTATE()
}

gl::Texture::operator unsigned() const
{
	return *obj_;
}

void gl::Texture::image_2D(const GLvoid* data, data_type::data_type_t type, format::format_t format, unsigned width,
	unsigned height, internal_format::internal_format_t internal_format)
{
	PUSHSTATE()

	glBindTexture(GL_TEXTURE_2D, *obj_);
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, type, data);

	POPSTATE()
}

void gl::Texture::set_warpping(warpping::warpping_t s)
{
	PUSHSTATE()

	glBindTexture(GL_TEXTURE_2D, *obj_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s);

	glBindTexture(GL_TEXTURE_2D, restoreId);

	POPSTATE()
}

void gl::Texture::set_warpiing(warpping::warpping_t s, warpping::warpping_t t)
{
	PUSHSTATE()

	glBindTexture(GL_TEXTURE_2D, *obj_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t);

	glBindTexture(GL_TEXTURE_2D, restoreId);

	POPSTATE()
}

void gl::Texture::set_warpiing(warpping::warpping_t s, warpping::warpping_t t, warpping::warpping_t r)
{
	PUSHSTATE()

	glBindTexture(GL_TEXTURE_2D, *obj_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, r);

	POPSTATE()
}

void gl::Texture::set_filters(filter::filter_t min, filter::filter_t mag)
{
	PUSHSTATE()

	glBindTexture(GL_TEXTURE_2D, *obj_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);

	POPSTATE()
}

void gl::Texture::set_border_color(const glm::vec4& color)
{
	PUSHSTATE()

	glBindTexture(GL_TEXTURE_2D, *obj_);
	float col[4] = { color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, col);

	POPSTATE()
}

void gl::Texture::generate_mip_maps()
{
	PUSHSTATE()

	glBindTexture(GL_TEXTURE_2D, *obj_);
	glGenerateMipmap(GL_TEXTURE_2D);

	POPSTATE()
}
