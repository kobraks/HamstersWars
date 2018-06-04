#include "TextureLoader.h"

#include "Image.h"
#include "Log.h"

std::shared_ptr<model::Texture2d> model::TextureLoader::load_texture(const std::string& file)
{
	auto iter = textures_.find(file);
	if (iter != textures_.end())
	{
		Log::level() = Log::log_info;
		Log::print("Texture loaded before file_name = %s", file.c_str());
		return iter->second;
	}
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

	return textures_[file] = std::make_shared<model::Texture2d>(image.width(), image.height(), std::make_shared<gl::Texture>(image));
}

std::unordered_map<std::string, std::shared_ptr<model::Texture2d>> model::TextureLoader::textures_ = std::unordered_map<
	std::string, std::shared_ptr<model::Texture2d>>();

