#pragma once
#include <string>
#include <map>
#include <memory>

#include "Texture2d.h"

namespace gl
{
	class Texture;
}

namespace model
{
	class TextureLoader
	{
	public:
		TextureLoader() = delete;
		TextureLoader(const TextureLoader&) = delete;

		TextureLoader& operator = (const TextureLoader&) = delete;

		static model::Texture2d* load_texture(const std::string& file);

	private:
		static std::map<std::string, std::shared_ptr<gl::Texture>> textures_;
	};

}
