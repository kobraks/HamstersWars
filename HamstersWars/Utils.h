#pragma once
#include <Lua/lua.hpp>
#include <Lua/LuaIntf.h>
#include <string>
#include "Vector3D.h"
#include "Vector2D.h"
#include "Color.h"

namespace game::utils
{
	std::string to_upper_copy(const std::string& str);
	std::string& to_upper(std::string& str);

	std::string to_lower_copy(const std::string& str);
	std::string& to_lower(std::string& str);

	gl::Vector3D get_vector3d(const LuaIntf::LuaRef& vector);
	gl::Vector2D get_vector2d(const LuaIntf::LuaRef& vector);
	gl::Color get_color(const LuaIntf::LuaRef& vector);

	bool table_contains(const LuaIntf::LuaRef& table, const std::string& key);
}

