#include "stdafx.h"
#include "ConsoleArgument.h"

namespace game
{
	ConsoleArgument::ConsoleArgument(const std::string& name, const std::string& value) : name_(name), value_(value)
	{
	}

	const std::string& ConsoleArgument::name() const
	{
		return name_;
	}

	ConsoleArgument::operator std::string() const
	{
		std::stringstream string;
		string << *this;

		return string.str();
	}

	std::ostream& operator<<(std::ostream& out, const ConsoleArgument& console_argument)
	{
		out << console_argument.name_;
		out << "=";
		out << console_argument.value_;

		return out;
	}
}
