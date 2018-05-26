#pragma once
#include <GL/glew.h>
#include "Texture.h"

namespace model
{
	class Texture2d
	{
	public:
		Texture2d(const unsigned& width, const unsigned& height, const gl::Texture& texture);

		operator GLuint() const;

		unsigned get_width() const
		{
			return width_;
		}

		unsigned get_height() const
		{
			return height_;
		}

		void bind();
	private:
		unsigned width_;
		unsigned height_;

		gl::Texture texture_;

	};
}

