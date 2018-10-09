#include "stdafx.h"
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

gl::Texture::Texture(const util::Image& image) : Texture()
{
	create(image);
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

unsigned gl::Texture::get_maximum_size()
{
	static bool checked = false;
	static GLint size = 0;

	if (!checked)
	{
		checked = true;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size);
	}

	return static_cast<unsigned int>(size);
}

void gl::Texture::create(const unsigned& width, const unsigned& height)
{
	if ((width == 0) || (height == 0))
	{
		return;
	}

	sf::Vector2u actual_size(get_valid_size(width), get_valid_size(height));
	unsigned int max_size = get_maximum_size();

	if (actual_size.x > max_size || actual_size.y > max_size)
		return;

	size_.x = width;
	size_.y = height;

	actual_size_ = actual_size;

	PUSHSTATE()

	glGenTextures(1, &*obj_);
	glBindTexture(GL_TEXTURE_2D, *obj_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, actual_size_.x, actual_size_.y, 0, GL_RGBA, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	POPSTATE()
}

void gl::Texture::create(const util::Image& image)
{
	PUSHSTATE()

	glGenTextures(1, &*obj_);
	glBindTexture(GL_TEXTURE_2D, *obj_);
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format::RGBA, image.width(), image.height(), 0, format::RGBA, data_type::Float, image.get_pixels());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenerateMipmap(GL_TEXTURE_2D);

	POPSTATE()
}

sf::Vector2u gl::Texture::size() const
{
	return size_;
}

unsigned gl::Texture::width() const
{
	return size_.x;
}

unsigned gl::Texture::height() const
{
	return size_.y;
}

gl::util::Image gl::Texture::to_image() const
{
	std::vector<glm::vec4> pixels(size_.x * size_.y);
	if (size_ == actual_size_)
	{
		glBindTexture(GL_TEXTURE_2D, *obj_);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, &pixels[0]);
	}
	else
	{
		std::vector<glm::vec4> all_pixels(actual_size_.x * actual_size_.y);
		glBindTexture(GL_TEXTURE_2D, *obj_);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, &all_pixels[0]);

		const glm::vec4* src = &all_pixels[0];
		glm::vec4* dst = &pixels[0];
		int src_pitch = actual_size_.x * 4;
		int dst_pitch = size_.x * 4;

		for (size_t i = 0; i < size_.y; ++i)
		{
			std::memcpy(dst, src, dst_pitch);
			src += src_pitch;
			dst += dst_pitch;
		}
	}

	return util::Image(size_, pixels.data());
}

void gl::Texture::update(const glm::vec4* pixels)
{
	update(pixels, size_.x, size_.y, 0, 0);
}

void gl::Texture::update(const glm::vec4* pixels, const unsigned& width, const unsigned& height, const unsigned& x,
	const unsigned& y)
{
	assert(x + width <= size_.x);
	assert(y + height <= size_.y);

	if (pixels)
	{
		glBindTexture(GL_TEXTURE_2D, *obj_);
		glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_RGBA, GL_FLOAT, pixels);
		glFlush();
	}
}

void gl::Texture::update(const Texture& texture)
{
	update(texture, 0, 0);
}

void gl::Texture::update(const Texture& texture, const unsigned& x, const unsigned& y)
{
	assert(texture.size_.x + x <= size_.x);
	assert(texture.size_.y + y <= size_.y);

	update(to_image(), x, y);
}

void gl::Texture::update(const util::Image& image)
{
	update(image.get_pixels(), image.width(), image.height(), 0, 0);
}

void gl::Texture::update(const util::Image& image, const unsigned& x, const unsigned& y)
{
	update(image.get_pixels(), image.width(), image.height(), x, y);
}

void gl::Texture::swap(Texture& right) noexcept
{
	std::swap(size_, right.size_);
	std::swap(actual_size_, right.actual_size_);
	std::swap(obj_, right.obj_);
}

unsigned gl::Texture::get_valid_size(const unsigned& size)
{
	return size;
}
