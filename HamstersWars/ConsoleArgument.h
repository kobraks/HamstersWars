#pragma once
#include "stdafx.h"

namespace game
{
	class ConsoleArgument
	{
		const std::string name_;
		std::string value_;
	public:
		ConsoleArgument(const std::string& name, const std::string& value = "");

		template<class Type>
		ConsoleArgument(const std::string& name, const Type& value);

		const std::string& name() const;

		template<class Type>
		Type value() const;

		template<>
		std::string value<std::string>() const
		{
			return value_;
		}

		template<>
		bool value<bool>() const
		{
			if (value_ == "1" || utils::to_upper_copy(value_) == "TRUE")
				return true;

			if (value_ == "0" || utils::to_upper_copy(value_) == "FALSE")
				return false;

			assert(false && "Unable to convert value to string");
			throw std::runtime_error("Unable to convert string to value");
		}

		template<class Type>
		void value(const Type& value);

		template<>
		void value(const std::string& value)
		{
			value_ = value;
		}

		friend std::ostream& operator<<(std::ostream& out, const ConsoleArgument& console_argument);
		explicit operator std::string() const;

		template<class Type>
		ConsoleArgument& operator=(const Type& value);

		template<>
		ConsoleArgument& operator=(const std::string& value)
		{
			value_ = value;
			return *this;
		}
	};

	template <class Type>
	ConsoleArgument::ConsoleArgument(const std::string& name, const Type& value) : name_(name), utils::to_string<Type>(value)
	{
	}

	template <class Type>
	void ConsoleArgument::value(const Type& value)
	{
		value_ = utils::to_string<Type>(value);
	}

	template <class Type>
	ConsoleArgument& ConsoleArgument::operator=(const Type& value)
	{
		value_ = utils::to_string<Type>(value);
		return *this;
	}

	template <class Type>
	Type ConsoleArgument::value() const
	{
		return utils::convert<Type>(value_);
	}
}

