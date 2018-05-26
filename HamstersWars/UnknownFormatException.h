#pragma once
#include <exception>
#include <string>

namespace gl::util
{
	class UnknownFormatException : public std::exception
	{
	public:
		UnknownFormatException() : message_("Unknow image format")
		{
			
		}

		char const* what() const override
		{
			return message_.c_str();
		}
	private:
		std::string message_;
	};
}