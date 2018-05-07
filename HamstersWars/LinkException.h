#pragma once
#include <exception>
#include <string>

namespace gl::exception
{
	class LinkException : public std::exception
	{
	public:
		LinkException(const std::string& str) : mes_ (str)
		{

		}


		char const* what() const override
		{
			return mes_.c_str();
		}
	private:
		std::string mes_;
	};
}