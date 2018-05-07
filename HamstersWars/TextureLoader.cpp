#include "TextureLoader.h"
#include <FreeImage/FreeImage.h>

#include "Texture2D.h"
#include "Log.h"

model::Texture2d* model::TextureLoader::load_texture(const std::string& file)
{
	auto format = FreeImage_GetFileType(file.c_str(), 0);
	if (format == FIF_UNKNOWN)
	{
		Log::level() = Log::log_error;
		Log::print("Wrong image format or file does not exists %s", file.c_str());

		return nullptr;
	}

	auto image = FreeImage_Load(format, file.c_str());
	if (!image)
	{
		Log::level() = Log::log_error;
		Log::print("Wrong image format or file does not exists %s", file.c_str());

		return nullptr;
	}

	image = FreeImage_ConvertTo32Bits(image);

	auto width = FreeImage_GetWidth(image);
	auto height = FreeImage_GetHeight(image);

	Log::print("size of image %s is: %u x %u", file.c_str(), width, height);

	auto pixels = reinterpret_cast<char*>(FreeImage_GetBits(image));
	auto texture = new GLubyte[4 * width * height];

	for (unsigned i = 0; i < width * height; ++i)
	{
		texture[i * 4] = pixels[i * 4 + 2];
		texture[i * 4 + 1] = pixels[i * 4 + 1];
		texture[i * 4 + 2] = pixels[i * 4];
		texture[i * 4 + 3] = pixels[i * 4 + 3];
	}

	GLuint texture_id;

	glGenTextures(1, &texture_id);

	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, static_cast<GLvoid*>(texture));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glGenerateMipmap(GL_TEXTURE_2D);

	delete[] texture;
	FreeImage_Unload(image);

	return new Texture2d(width, height, texture_id);
}

