#pragma once
#include <exception>
#include <string>

namespace gl
{
	namespace exception
	{
		class CompileException : public std::exception
		{
		public:
			CompileException(const std::string& str) : msg_(str)
			{

			}


			char const* what() const override
			{
				return msg_.c_str();
			}
		private:
			std::string msg_;
		};
	}
}