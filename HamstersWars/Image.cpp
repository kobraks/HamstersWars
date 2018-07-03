#include "Image.h"
#include <FreeImage/FreeImage.h>
#include <fstream>
#include <algorithm>

#include "OpenFileException.h"
#include "UnknownFormatException.h"

FREE_IMAGE_FORMAT convert(gl::util::image_type::image_type_t type)
{
	switch(type)
	{
	case gl::util::image_type::BMP:
		return FIF_BMP;

	case gl::util::image_type::EXR:
		return FIF_EXR;

	case gl::util::image_type::J2K:
		return FIF_J2K;

	case gl::util::image_type::JP2:
		return FIF_JP2;

	case gl::util::image_type::JPEG:
		return FIF_JPEG;

	case gl::util::image_type::JXR:
		return FIF_JXR;

	case gl::util::image_type::PBM:
		return FIF_PBM;

	case gl::util::image_type::PGM:
		return FIF_PGM;

	case gl::util::image_type::PNG:
		return FIF_PNG;

	case gl::util::image_type::PPM:
		return FIF_PPM;

	case gl::util::image_type::TIFF:
		return FIF_TIFF;

	default:
		return FIF_UNKNOWN;
	}
}

gl::util::Image::Image() : pixels_(nullptr), width_(0), height_(0)
{
}

gl::util::Image::Image(const unsigned& width, const unsigned& height, const glm::vec4& background) : width_(width), height_(height)
{
	create(width, height, background);
}

gl::util::Image::Image(const sf::Vector2u& size, const glm::vec4& background)
{
	create(size.x, size.y, background);
}

gl::util::Image::Image(const unsigned& width, const unsigned& height, const unsigned char* pixels) : width_(width), height_(height)
{
	pixels_ = new glm::vec4[width * height];

	for (size_t i = 0; i < width; ++i)
		for (size_t j = 0; j < height; ++j)
		{
			size_t u = (i + j * width) * sizeof(glm::vec4);
			pixels_[i + j * width] = glm::vec4(pixels[u + 0], pixels[u + 1], pixels[u + 2], pixels[u + 3]);
		}
}

gl::util::Image::Image(const sf::Vector2u& size, const unsigned char* pixels) : Image(size.x, size.y, pixels)
{
}

gl::util::Image::Image(const unsigned& width, const unsigned& height, const glm::vec4* pixels)
{
	pixels_ = new glm::vec4[width * height];
	std::memcpy(pixels_, pixels, width * height);
}

gl::util::Image::Image(const sf::Vector2u& size, const glm::vec4* pixels) : Image(size.x, size.y, pixels)
{
}

gl::util::Image::Image(const unsigned char* pixels, const unsigned& size) : Image()
{
	load(pixels, size);
}

gl::util::Image::Image(const std::string& file_name) : Image()
{
	load(file_name);
}

gl::util::Image::Image(const Image& image) : width_(image.width_), height_(image.height_)
{
	size_t size = width_ * height_;

	pixels_ = new glm::vec4[size];
	std::memcpy(pixels_, image.pixels_, size);
}

gl::util::Image& gl::util::Image::operator=(const Image& image)
{
	width_ = image.width_;
	height_ = image.height_;
	size_t size = width_ * height_;

	pixels_ = new glm::vec4[size];
	std::memcpy(pixels_, image.pixels_, size);

	return *this;
}

gl::util::Image::Image(Image&& image) noexcept
{
	pixels_ = image.pixels_;
	image.pixels_ = nullptr;

	width_ = image.width_;
	height_ = image.height_;
}

gl::util::Image& gl::util::Image::operator=(Image&& image) noexcept
{
	pixels_ = image.pixels_;
	image.pixels_ = nullptr;

	width_ = image.width_;
	height_ = image.height_;

	return *this;
}


gl::util::Image::~Image()
{
	delete[] pixels_;
}

void gl::util::Image::create(const sf::Vector2u& size, const glm::vec4& background)
{
	create(size.x, size.y, background);
}

void gl::util::Image::create(const unsigned& width, const unsigned& height, const glm::vec4& background)
{
	pixels_ = new glm::vec4[width * height];

	std::fill_n(pixels_, width * height, background);
}

void gl::util::Image::load(const unsigned char* pixels, const unsigned& size)
{
	delete[] pixels_;

	pixels_ = nullptr;
	height_ = width_ = 0;

	auto format = FreeImage_GetFileTypeFromMemory(reinterpret_cast<FIMEMORY*>(const_cast<unsigned char*>(pixels)), size);
	if (format == FIF_UNKNOWN)
	{
		throw UnknownFormatException();
	}

	auto image = FreeImage_LoadFromMemory(format, reinterpret_cast<FIMEMORY*>(const_cast<unsigned char*>(pixels)), size);

	if (!image)
	{
		throw OpenFileException();
	}

	load_to_memory(image);

	FreeImage_Unload(image);
}

void gl::util::Image::load(const std::string& file_name)
{
	delete[] pixels_;
	width_ = height_ = 0;

	std::ifstream file;
	file.exceptions(std::ios::failbit | std::ios::badbit);
	file.open(file_name);
	file.close();

	auto format = FreeImage_GetFileType(file_name.c_str(), 0);
	if (format == FIF_UNKNOWN)
		throw UnknownFormatException();

	auto image = FreeImage_Load(format, file_name.c_str());
	if (!image)
		throw OpenFileException();

	load_to_memory(image);

	FreeImage_Unload(image);
}

void gl::util::Image::save(const std::string& file_name, image_type::image_type_t type)
{
	auto handler = FreeImage_Allocate(width_, height_, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK);

	for (size_t i = 0; i < width_; ++i)
		for (size_t j = 0; j < height_; ++j)
		{
			RGBQUAD color;

			auto pixel = get_pixel(i, j);
			color.rgbRed = static_cast<BYTE>(pixel.r * 255);
			color.rgbGreen = static_cast<BYTE>(pixel.g * 255);
			color.rgbBlue = static_cast<BYTE>(pixel.b * 255);
			color.rgbReserved = static_cast<BYTE>(pixel.a * 255);

			FreeImage_SetPixelColor(handler, i, j, &color);
		}

	FreeImage_Save(convert(type), handler, file_name.c_str(), 0);
	FreeImage_Unload(handler);
}

unsigned gl::util::Image::width() const
{
	return width_;
}

unsigned gl::util::Image::height() const
{
	return height_;
}

sf::Vector2u gl::util::Image::size() const
{
	return sf::Vector2u(width_, height_);
}

const glm::vec4* gl::util::Image::get_pixels() const
{
	return pixels_;
}

glm::vec4& gl::util::Image::get_pixel(const sf::Vector2u& position)
{
	return get_pixel(position.x, position.y);
}

glm::vec4 gl::util::Image::get_pixel(const sf::Vector2u& position) const
{
	return get_pixel(position.x, position.y);
}

void gl::util::Image::set_pixel(const sf::Vector2u& position, const glm::vec4& color)
{
	set_pixel(position.x, position.y, color);
}

glm::vec4& gl::util::Image::get_pixel(const unsigned& x, const unsigned& y)
{
	assert(x < width_);
	assert(y < height_);

	return pixels_[x + y * width_];
}

glm::vec4 gl::util::Image::get_pixel(const unsigned& x, const unsigned& y) const
{
	assert(x < width_);
	assert(y < height_);
	
	return pixels_[x + y * width_];
}

void gl::util::Image::set_pixel(const unsigned& x, const unsigned& y, const glm::vec4& color)
{
	assert(x < width_);
	assert(y < height_);
		
	pixels_[x + y * width_] = color;
}

glm::vec4 gl::util::Image::operator()(const unsigned& x, const unsigned& y) const
{
	return get_pixel(x, y);
}

glm::vec4& gl::util::Image::operator()(const unsigned& x, const unsigned& y)
{
	return get_pixel(x, y);
}

glm::vec4 gl::util::Image::translate_color(const unsigned char& r, const unsigned char& g, const unsigned char& b,
                                           const unsigned char& a)
{
	return glm::vec4(static_cast<float>(r) / 255.f, static_cast<float>(g) / 255.f, static_cast<float>(b) / 255.f,
	                 static_cast<float>(a) / 255.f);
}

glm::vec4 gl::util::Image::translate_color(const unsigned char color[4])
{
	return glm::vec4(static_cast<float>(color[0]) / 255.f, static_cast<float>(color[1]) / 255.f,
	                 static_cast<float>(color[2]) / 255.f, static_cast<float>(color[3]) / 255.f);

}

void gl::util::Image::load_to_memory(void* image_file)
{
	auto image = reinterpret_cast<FIBITMAP*>(image_file);
	image = FreeImage_ConvertTo32Bits(image);

	width_ = FreeImage_GetWidth(image);
	height_ = FreeImage_GetHeight(image);

	auto pixels = reinterpret_cast<char*>(FreeImage_GetBits(image));

	pixels_ = new glm::vec4[width_ * height_];

	for (size_t i = 0; i < width_ * height_; ++i)
		pixels_[i] = translate_color(pixels[i * 4 + FI_RGBA_RED], pixels[i * 4 + FI_RGBA_GREEN], pixels[i * 4 + FI_RGBA_BLUE],
		                             pixels[i * 4 + FI_RGBA_ALPHA]);

	FreeImage_Unload(image);
}
