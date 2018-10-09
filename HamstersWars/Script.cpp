#include "stdafx.h"
#include "Script.h"

LuaIntf::LuaContext game::lua::Script::lua_;
std::unordered_map<std::string, LuaIntf::LuaRef*> game::lua::Script::refs_;
std::vector<std::type_index> game::lua::Script::registered_classes_;

game::lua::Script::~Script()
{
	for (auto iter : refs_)
	{
		delete iter.second;
	}
}

LuaIntf::LuaRef* game::lua::Script::lua_ref(const std::string& name)
{
	if (refs_.find(name) != refs_.end())
	{
		LuaIntf::LuaRef* ref = new LuaIntf::LuaRef(lua_, name.c_str());
		refs_[name] = ref;
		return ref;
	}
	else
		return refs_[name];

}

game::lua::Script::operator LuaIntf::LuaContext&() const
{
	return lua_;
}

LuaIntf::LuaContext& game::lua::Script::lua()
{
	return lua_;
}

// game::lua::Script::operator LuaIntf::LuaBinding() const
// {
// 	return LuaIntf::LuaBinding(lua_);
// }

void game::lua::Script::do_file(const std::string& file)
{
	lua_.doFile(file.c_str());
}

void game::lua::Script::do_string(const std::string& code)
{
	lua_.doString(code.c_str());
}

LuaIntf::LuaBinding game::lua::Script::binding()
{
	return LuaIntf::LuaBinding(lua_);
}
