#include "stdafx.h"
#include "GameException.h"
#include <cstdarg>
#include <utility>

game::exception::GameException::GameException(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	message_ = get_string(format, args);
	va_end(args);
}

game::exception::GameException::GameException(std::string message) : message_(std::move(message))
{
}

char const* game::exception::GameException::what() const
{
	return message_.c_str();
}

std::string game::exception::GameException::get_string(const char* format, va_list args)
{
	va_list arg_copy;
	va_copy(arg_copy, args);

	auto lenght = vsnprintf(nullptr, 0, format, args);

	if (lenght <= 0)
		return "";

	auto string = new char[lenght + 1];
	vsnprintf(string, lenght + 1, format, arg_copy);
	std::string result = string;

	delete[] string;
	return result;
}

