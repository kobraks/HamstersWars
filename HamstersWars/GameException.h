#pragma once
#include <exception>
#include <string>

namespace game::exception
{
	class GameException : public std::exception
	{
	public:
		GameException(const char* format, ...);
		explicit GameException(std::string message);

		char const* what() const override;

	private:
		std::string message_;


		static std::string get_string(const char* format, va_list args);
	};
}

