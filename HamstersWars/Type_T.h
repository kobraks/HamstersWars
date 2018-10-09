#pragma once
#include <string>
#include "Utils.h"

namespace game
{
	class Type_T
	{
		std::string name_;

	public:
		explicit Type_T(const std::string& type_name) : name_(type_name)
		{
		}

		template<class Type>
		Type_T() : name_(GET_TYPE_NAME(Type))
		{}

		std::string name() const
		{
			return name_;
		}
	};

}