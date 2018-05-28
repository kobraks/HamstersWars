#include "TextureLoader.h"

#include "Image.h"
#include "Texture2D.h"
#include "Log.h"

model::Texture2d* model::TextureLoader::load_texture(const std::string& file)
{
	gl::util::Image image;
	try
	{
		image.load(file);
	}
	catch(std::exception& ex)
	{
		Log::level() = Log::log_error;
		Log::print("Unable to load %s, with exception %s", file.c_str(), ex.what());
	}

	Log::level() = Log::log_info;
	Log::print("Image size: %u, %u", image.width(), image.height());

	auto texture = textures_[file] = gl::Texture(image);

	return new Texture2d(image.width(), image.height(), texture);
}

std::map<std::string, gl::Texture> model::TextureLoader::textures_ = std::map<std::string, gl::Texture>();

