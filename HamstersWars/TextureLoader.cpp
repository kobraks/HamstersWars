#include "stdafx.h"
#include "TextureLoader.h"

#include "Image.h"

std::shared_ptr<gl::Texture> game::model::TextureLoader::load_texture(const std::string& file)
{
	LOG(LOG_INFO, "Loading texture %s", file.c_str());
	auto iter = textures_.find(file);
	if (iter != textures_.end())
	{
		LOG(LOG_DEBUG, "Texture loaded before");
		return iter->second;
	}
	gl::util::Image image;
	try
	{
		image.load(file);
	}
	catch(std::exception& ex)
	{
		LOG(LOG_ERROR, "Unable to load file %s, %s", file.c_str(), ex.what());
	}

	LOG(LOG_DEBUG, "Image size %ux%u", image.width(), image.height());

	return textures_[file] = std::make_shared<gl::Texture>(image);
}

std::unordered_map<std::string, std::shared_ptr<gl::Texture>> game::model::TextureLoader::textures_ = std::unordered_map<
	std::string, std::shared_ptr<gl::Texture>>();

