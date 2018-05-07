#pragma once
#include <GL/glew.h>

namespace model
{
	class Texture2d
	{
	public:
		Texture2d(const unsigned& width, const unsigned& height, const GLuint& texture_id);
		~Texture2d();

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

		GLuint texture_id_;

	};
}

