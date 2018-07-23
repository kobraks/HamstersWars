#pragma once
#include <Lua/lua.hpp>
#include <Lua/LuaIntf.h>
#include "Vector3D.h"

namespace game::lua::utils
{
	gl::Vector3D get_vector3d(LuaIntf::LuaRef vector);
}
