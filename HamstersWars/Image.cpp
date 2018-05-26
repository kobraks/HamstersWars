#include "Image.h"
#include <FreeImage/FreeImage.h>
#include <fstream>

#include "OpenFileException.h"
#include "UnknownFormatException.h"
#include "gl/glew.h"

gl::util::Image::Image() : pixels_(nullptr), width_(0), height_(0)
{
}

gl::util::Image::Image(const unsigned& width, const unsigned& height, const glm::vec4& background) : width_(width), height_(height)
{
	pixels_ = new glm::vec4[width * height];

	for (size_t i = 0; i < width*height; ++i)
		pixels_[i] = background;
}

gl::util::Image::Image(const unsigned& width, const unsigned& height, unsigned char* pixels) : width_(width), height_(height)
{
	pixels_ = new glm::vec4[width * height];

	for (size_t i = 0; i < width; ++i)
		for (size_t j = 0; j < height; ++j)
		{
			size_t u = (i + j * width) * sizeof(glm::vec4);
			pixels_[i + j * width] = glm::vec4(pixels[u + 0], pixels[u + 1], pixels[u + 2], pixels[u + 3]);
		}
}

gl::util::Image::Image(unsigned char* pixels, const unsigned& size) : Image()
{
	load(pixels, size);
}

gl::util::Image::Image(const std::string& file_name) : Image()
{
	load(file_name);
}


gl::util::Image::~Image()
{
	delete[] pixels_;
}

void gl::util::Image::load(unsigned char* pixels, const unsigned& size)
{
	delete[] pixels_;

	pixels_ = nullptr;
	height_ = width_ = 0;

	auto format = FreeImage_GetFileTypeFromMemory(reinterpret_cast<FIMEMORY*>(pixels), size);
	if (format == FIF_UNKNOWN)
	{
		throw UnknownFormatException();
	}

	auto image = FreeImage_LoadFromMemory(format, reinterpret_cast<FIMEMORY*>(pixels), size);

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

unsigned gl::util::Image::width() const
{
	return width_;
}

unsigned gl::util::Image::height() const
{
	return height_;
}

const glm::vec4* gl::util::Image::get_pixels() const
{
	return pixels_;
}

glm::vec4 gl::util::Image::get_pixels(const unsigned& x, const unsigned& y) const
{
	if (x < width_ && height_ < y)
		return pixels_[x + y * width_];
	else
		throw std::out_of_range("");
}

void gl::util::Image::set_pixels(const unsigned& x, const unsigned& y, const glm::vec4& color)
{
	if (x < width_ && height_ < y)
		pixels_[x + y * width_] = color;
	else
		throw std::out_of_range("");
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
	{
		GLubyte pixel[4] = { pixels[i * 4 + 2], pixels[i * 4 + 1], pixels[i * 4], pixels[i * 4 + 3] };
		pixels_[i] = glm::vec4((float)pixel[0] / 255, (float)pixel[1] / 255, (float)pixel[2] / 255, (float)pixel[3] / 255);
	}

	FreeImage_Unload(image);
}
