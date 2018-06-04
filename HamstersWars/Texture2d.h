#pragma once
#include <GL/glew.h>
#include <memory>
#include "Texture.h"

namespace model
{
	class Texture2d
	{
	public:
		Texture2d(const unsigned& width, const unsigned& height, std::shared_ptr<gl::Texture> texture);

		operator GLuint() const;

		unsigned get_width() const
		{
			return width_;
		}

		unsigned get_height() const
		{
			return height_;
		}

		std::shared_ptr<gl::Texture> get_texture() const
		{
			return texture_;
		}

		void bind();
	private:
		unsigned width_;
		unsigned height_;

		std::shared_ptr<gl::Texture> texture_;

	};
}

