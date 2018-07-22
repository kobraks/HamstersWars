#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include <Lua/Lua.hpp>
#include <Lua/LuaIntf.h>
#include <typeindex>

#include "Register.h"

namespace game::lua
{
	class Script
	{
	public:
		Script() = default;
		~Script();

		operator LuaIntf::LuaContext&() const;
		static LuaIntf::LuaContext& lua();

		static void do_file(const std::string& file);
		static void do_string(const std::string& code);
		static LuaIntf::LuaBinding binding();

		static LuaIntf::LuaRef* lua_ref(const std::string& name);

		template<class T>
		static void register_class(const Register* reg);

	protected:
		template<class T>
		static bool registered();
	private:
		static LuaIntf::LuaContext lua_;
		static std::unordered_map<std::string, LuaIntf::LuaRef*> refs_;
		static std::vector<std::type_index> registered_classes_;
	};

	template <class T>
	void Script::register_class(const Register* reg)
	{
		if (!registered<T>())
		{
			reg->register_clas(lua_);
			registered_classes_.push_back(typeid(T));
		}
	}

	template <class T>
	bool Script::registered()
	{
		return std::find(registered_classes_.begin(), registered_classes_.end(), typeid(T)) != registered_classes_.end();
	}
}

