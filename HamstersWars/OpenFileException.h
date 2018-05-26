#include <exception>
#include <string>

namespace gl::util
{
	class OpenFileException : public std::exception
	{
	public:
		OpenFileException() : message_("Unable to open image file")
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
