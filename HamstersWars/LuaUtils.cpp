#include "LuaUtils.h"


gl::Vector3D game::lua::utils::get_vector3d(LuaIntf::LuaRef vector)
{
	gl::Vector3D result;

	assert(vector.isTable());

	size_t i = 0;
	for (auto el : vector)
	{
		result[i] = el.value<float>();
		i++;
	}
	return result;
}
