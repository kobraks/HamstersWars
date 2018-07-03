#pragma once
#include <GL/glew.h>
#include <string>
#include <exception>

namespace gl::exception
{
	class GlewException : public std::exception
	{
	public:
		explicit GlewException(GLenum error_num)
		{
			message_ = reinterpret_cast<char>(glewGetErrorString(error_num));
		}

		char const* what() const override
		{
			return message_.c_str();
		}
	private:
		std::string message_;
	};
}