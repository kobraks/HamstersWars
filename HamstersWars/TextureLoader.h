#pragma once
#include "Texture2d.h"
#include <string>

namespace model
{
	class TextureLoader
	{
	public:
		TextureLoader() = delete;
		TextureLoader(const TextureLoader&) = delete;

		TextureLoader& operator = (const TextureLoader&) = delete;

		static model::Texture2d* load_texture(const std::string& file);
	};

}
