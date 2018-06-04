#pragma once
#include <string>
#include <unordered_map>
#include <memory>

#include "Texture2d.h"

namespace model
{
	class TextureLoader
	{
	public:
		TextureLoader() = delete;
		TextureLoader(const TextureLoader&) = delete;

		TextureLoader& operator = (const TextureLoader&) = delete;

		static std::shared_ptr<Texture2d> load_texture(const std::string& file);

	private:
		static std::unordered_map<std::string, std::shared_ptr<Texture2d>> textures_;
	};

}
