#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

#include <typeindex>

#include "Register.h"
#include "Log.h"

namespace game::lua
{
	class Script
	{
	public:
		typedef std::function<void(LuaIntf::LuaBinding&)> register_function;

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

		template<class T>
		static void register_class(register_function function);

		template<class T>
		static bool registered();

	protected:
		template<class T>
		static void run_register(register_function function);

	private:
		static LuaIntf::LuaContext lua_;
		static std::unordered_map<std::string, LuaIntf::LuaRef*> refs_;
		static std::vector<std::type_index> registered_classes_;
	};

	template <class T>
	void Script::register_class(const Register* reg)
	{
		register_class<T>([reg](LuaIntf::LuaBinding& binding)
		{
			reg->register_class(binding);
		});
	}

	template <class T>
	void Script::register_class(const register_function function)
	{
		if (!registered<T>())
			run_register<T>(function);
	}

	template <class T>
	bool Script::registered()
	{
		return std::find(registered_classes_.begin(), registered_classes_.end(), typeid(T)) != registered_classes_.end();
	}

	template <class T>
	void Script::run_register(register_function function)
	{
		try
		{
			auto binding = LuaIntf::LuaBinding(lua_);
			function(binding);
			registered_classes_.push_back(typeid(T));
		}
		catch (LuaIntf::LuaException& ex)
		{
			LOG(LOG_ERROR, "Lua throws exception: %s", ex.what());
		}
	}
}

#define REGISTER(TYPE, TO_REGISTER) lua::Script::register_class<TYPE>(TO_REGISTER)

#define REGISTER_CLASS_INSTANCE(TYPE, INSTANCE)\
	{\
		TYPE& to_register = INSTANCE;\
		REGISTER(TYPE, &to_register);\
	}

#define REGISTER_CLASS_CONSTRUCTOR(TYPE, PARAM, ...) \
	{\
		TYPE to_register(PARAM, ##__VA_ARGS__); \
		REGISTER(TYPE, &to_register);\
	}

#define REGISTER_CLASS(TYPE) \
	{\
		TYPE to_register; \
		REGISTER(TYPE, &to_register);\
	}

#define REGISTER_FUNCTION(TYPE, FUNCTION) \
	lua::Script::register_class<TYPE>(FUNCTION);

