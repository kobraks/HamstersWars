#include "Utils.h"
#include <algorithm>
#include "UnknownTableElementException.h"

std::string game::utils::to_upper_copy(const std::string& str)
{
	auto result = str;
	to_upper(result);

	return result;
}

std::string& game::utils::to_upper(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);

	return str;
}

std::string game::utils::to_lower_copy(const std::string& str)
{
	auto result = str;
	to_lower(result);

	return result;
}

std::string& game::utils::to_lower(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);

	return str;
}

gl::Vector3D game::utils::get_vector3d(const LuaTable& vector)
{
	assert(vector.isTable());

	gl::Vector3D result;
	for (auto el : vector)
	{
		auto key = el.key<std::string>();

		if (key.length() != 1)
			throw exception::UnknownTableElementException();

		switch (to_upper_copy(key)[0])
		{
		case '1':
		case 'X':
			result.x = el.value<float>();
			break;
		case '2':
		case 'Y':
			result.y = el.value<float>();
			break;
		case '3':
		case 'Z':
			result.z = el.value<float>();
			break;
		default:
			throw exception::UnknownTableElementException();
		}
	}
	return result;
}

gl::Vector2D game::utils::get_vector2d(const LuaTable& vector)
{
	assert(vector.isTable());

	gl::Vector2D result;

	for (auto el : vector)
	{
		auto key = el.key<std::string>();
		if (key.length() != 1)
			throw exception::UnknownTableElementException();

		switch (to_upper_copy(key)[0])
		{
		case '1':
		case 'X':
			result.x = el.value<float>();
			break;
		case '2':
		case 'Y':
			result.y = el.value<float>();
			break;
		default:
			throw exception::UnknownTableElementException();
		}
	}
	return result;
}

gl::Color game::utils::get_color(const LuaTable& vector)
{
	assert(vector.isTable());

	gl::Color result;
	for (auto el : vector)
	{
		auto key = to_upper_copy(el.key<std::string>());
		auto value = el.value<float>();

		if (key == "1" || key == "R" || key == "RED")
			result.r = value;
		else if (key == "2" || key == "G" || key == "GREEN")
			result.g = value;
		else if (key == "3" || key == "B" || key == "BLUE")
			result.b = value;
		else if (key == "4" || key == "A" || key == "ALPHA")
			result.a = value;
		else
			throw exception::UnknownTableElementException();
	}
	return result;
}

bool game::utils::table_contains(const LuaTable& table, const std::string& key)
{
	assert(table.isTable());
	std::string upper_key = to_upper_copy(key);

	for (auto element : table)
	{
		if (to_upper_copy(element.key<std::string>()) == upper_key)
			return true;
	}

	return false;
}

bool game::utils::get_table_if_contains(LuaIntf::LuaRef& element, const LuaTable& table, std::string& key)
{
	assert(table.isTable());
	std::string upper_key = to_upper_copy(key);

	for (auto el : table)
		if (to_upper_copy(el.key<std::string>()) == upper_key)
		{
			element = el.value<LuaIntf::LuaRef>();
			return true;
		}

	return false;
}

bool game::utils::equals(const std::string& str1, const std::string& str2)
{
	auto s1 = to_upper_copy(str1);
	auto s2 = to_upper_copy(str2);

	return s1 == s2;
}
