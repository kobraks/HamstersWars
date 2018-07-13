#include "Script.h"


LuaIntf::LuaContext game::Script::lua_;
std::unordered_map<std::string, LuaIntf::LuaRef*> game::Script::refs_;

game::Script::~Script()
{
	for (auto iter : refs_)
	{
		delete iter.second;
	}
}

LuaIntf::LuaRef* game::Script::lua_ref(const std::string& name)
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

void game::Script::do_string(const std::string& code)
{
	lua_.doString(code.c_str());
}

LuaIntf::LuaBinding game::Script::binding()
{
	return LuaIntf::LuaBinding(lua_);
}
