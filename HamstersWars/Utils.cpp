#include "Utils.h"
#include <algorithm>
#include <regex>
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

std::string game::utils::get_path(const LuaIntf::LuaRef& path)
{
	if (path.isTable())
	{
		for (auto element : path)
			if (equals(element.key<std::string>(), "path"))
				return element.value<std::string>();
	}
	else
		return path.toValue<std::string>();

	return "";
}

bool game::utils::is_path(const LuaIntf::LuaRef& lua_ref)
{
	if (lua_ref.isTable())
	{
		for (auto element : lua_ref)
			if (equals(element.key<std::string>(), "lua_ref"))
				return true;
	}
	else
	{
		if (!lua_ref.toValue<std::string>().empty())
			return true;
	}

	return false;
}

bool game::utils::is_local_path(const std::string& path)
{
	std::regex regex("^.x:\\|/");

	return !std::regex_search(path, regex);
}

gl::Vector3D game::utils::get_greater_values(const gl::Vector3D& left, const gl::Vector3D& right)
{
	gl::Vector3D result = left;

	if (result.x < right.x)
		result.x = right.x;

	if (result.y < right.y)
		result.y = right.y;

	if (result.z < right.z)
		result.z = right.z;

	return result;
}

gl::Vector3D game::utils::get_lesser_values(const gl::Vector3D& left, const gl::Vector3D& right)
{
	gl::Vector3D result = left;

	if (result.x > right.x)
		result.x = right.x;

	if (result.y > right.y)
		result.y = right.y;

	if (result.z > right.z)
		result.z = right.z;

	return result;
}

gl::Vector2D game::utils::get_greater_values(const gl::Vector2D& left, const gl::Vector2D& right)
{
	gl::Vector2D result = left;

	if (result.x < right.x)
		result.x = right.x;

	if (result.y < right.y)
		result.y = right.y;

	return result;
}

gl::Vector2D game::utils::get_lesser_values(const gl::Vector2D& left, const gl::Vector2D& right)
{
	gl::Vector2D result = left;

	if (result.x > right.x)
		result.x = right.x;

	if (result.y > right.y)
		result.y = right.y;

	return result;
}
