#pragma once
#include <Lua/lua.hpp>
#include <Lua/LuaIntf.h>
#include <string>
#include "Vector3D.h"
#include "Vector2D.h"
#include "Color.h"

namespace game::utils
{
	typedef LuaIntf::LuaRef LuaTable;

	std::string to_upper_copy(const std::string& str);
	std::string& to_upper(std::string& str);

	std::string to_lower_copy(const std::string& str);
	std::string& to_lower(std::string& str);

	gl::Vector3D get_vector3d(const LuaTable& vector);
	gl::Vector2D get_vector2d(const LuaTable& vector);
	gl::Color get_color(const LuaTable& vector);

	bool table_contains(const LuaTable& table, const std::string& key);
	bool get_table_if_contains(LuaIntf::LuaRef& element, const LuaTable& table, std::string& key);

	bool equals(const std::string& str1, const std::string& str2);

	std::string get_path(const LuaIntf::LuaRef& path);
	bool is_local_path(const std::string& path);
}

