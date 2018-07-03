#include "Script.h"


LuaIntf::LuaContext game::Script::lua_;

game::Script::operator LuaIntf::LuaContext&() const
{
	return lua_;
}

LuaIntf::LuaContext& game::Script::lua()
{
	return lua_;
}

// game::Script::operator LuaIntf::LuaBinding() const
// {
// 	return LuaIntf::LuaBinding(lua_);
// }

void game::Script::do_file(const std::string& file)
{
	lua_.doFile(file.c_str());
}

LuaIntf::LuaBinding game::Script::binding()
{
	return LuaIntf::LuaBinding(lua_);
}
