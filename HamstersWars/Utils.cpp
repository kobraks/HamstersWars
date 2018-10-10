#include "stdafx.h"
#include "Utils.h"

namespace game::utils
{
	std::string to_upper_copy(std::string str)
	{
		return to_upper(str);
	}

	std::string& to_upper(std::string& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), ::toupper);

		return str;
	}

	std::string to_lower_copy(std::string str)
	{
		return to_lower(str);
	}

	std::string& to_lower(std::string& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);

		return str;
	}

	gl::Vector3D get_vector3d(const LuaTable& vector)
	{
		assert(vector.isTable());

		gl::Vector3D result(1.f);
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

	gl::Vector2D get_vector2d(const LuaTable& vector)
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

	gl::Color get_color(const LuaTable& vector)
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

	bool table_contains(const LuaTable& table, const std::string& key)
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

	bool get_table_if_contains(LuaIntf::LuaRef& element, const LuaTable& table, std::string& key)
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

	bool equals(const std::string& str1, const std::string& str2)
	{
		auto s1 = to_upper_copy(str1);
		auto s2 = to_upper_copy(str2);

		return s1 == s2;
	}

	std::string get_path(const LuaIntf::LuaRef& path)
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

	bool is_path(const LuaIntf::LuaRef& lua_ref)
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

	bool is_local_path(const std::string& path)
	{
		const std::regex regex("^.x:\\|/");

		return !std::regex_search(path, regex);
	}

	gl::Vector3D get_greater_values(const gl::Vector3D& left, const gl::Vector3D& right)
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

	gl::Vector3D get_lesser_values(const gl::Vector3D& left, const gl::Vector3D& right)
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

	gl::Vector2D get_greater_values(const gl::Vector2D& left, const gl::Vector2D& right)
	{
		gl::Vector2D result = left;

		if (result.x < right.x)
			result.x = right.x;

		if (result.y < right.y)
			result.y = right.y;

		return result;
	}

	gl::Vector2D get_lesser_values(const gl::Vector2D& left, const gl::Vector2D& right)
	{
		gl::Vector2D result = left;

		if (result.x > right.x)
			result.x = right.x;

		if (result.y > right.y)
			result.y = right.y;

		return result;
	}

	std::string& l_trim(std::string& string)
	{
		string.erase(string.begin(), std::find_if(string.begin(), string.end(), [](int ch)
		{
			return !std::isspace(ch);
		}));

		return string;
	}

	std::string& r_trim(std::string& string)
	{
		string.erase(std::find_if(string.rbegin(), string.rend(), [](int ch)
		{
			return !std::isspace(ch);
		}).base(), string.end());

		return string;
	}

	std::string& trim(std::string& string)
	{
		l_trim(string);
		r_trim(string);

		return string;
	}

	std::string l_trim_copy(std::string string)
	{
		return l_trim(string);
	}

	std::string r_trim_copy(std::string string)
	{
		return r_trim(string);
	}

	std::string trim_copy(std::string string)
	{
		return trim(string);
	}

	std::string get_string(const char* format, va_list args)
	{
		va_list args_copy;
		va_copy(args_copy, args);

		const auto length = vsnprintf(nullptr, 0, format, args);

		if (length <= 0)
			return "";

		const auto string = new char[length + 1];
		vsnprintf(string, length + 1, format, args_copy);
		std::string result = string;

		delete[] string;
		return result;
	}

	std::string print_to_string(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		auto string = get_string(format, args);
		va_end(args);

		return string;
	}
}
