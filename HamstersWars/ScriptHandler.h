#pragma once
#include <memory>
#include <string>
#include <vector>
#include <map>

#include <Lua/lua.hpp>
#include <Lua/LuaIntf.h>
#include "Component.h"
#include "Register.h"

namespace LuaIntf
{
	LUA_USING_LIST_TYPE(std::vector)
	LUA_USING_MAP_TYPE(std::map)
}

namespace game::component
{
	class ScriptHandler : public Component, protected lua::Register
	{
	public:
		explicit ScriptHandler(std::shared_ptr<Entity> owner);
		ScriptHandler(std::shared_ptr<Entity> owner, const std::string& update);
		ScriptHandler(std::shared_ptr<Entity> owner, const std::string& update, const std::string& create);
		ScriptHandler(std::shared_ptr<Entity> owner, const std::string& update, const std::string& create, const std::string& destroy);
		ScriptHandler(std::shared_ptr<Entity> owner, const std::string& update, const std::string& create,
		              const std::string& destroy, const std::string& copy);
		ScriptHandler(std::shared_ptr<Entity> entity, const LuaIntf::LuaRef& component_table);

		void set_on_update(const std::string& code);
		void set_on_update(const LuaIntf::LuaRef& function);
		void set_on_destroy(const std::string& code);
		void set_on_destroy(const LuaIntf::LuaRef& function);
		void set_on_copy(const std::string& code);
		void set_on_copy(const LuaIntf::LuaRef& function);
		void set_on_create(const std::string& code);
		void set_on_create(const LuaIntf::LuaRef& function);

		Component* copy() const override;

		void update();
		void on_destroy();
		void on_create();
		void on_copy();

	protected:
		void register_clas(LuaIntf::LuaContext& context) const override;

	private:
		ScriptHandler();
		LuaIntf::LuaRef update_;
		LuaIntf::LuaRef create_;
		LuaIntf::LuaRef destroy_;
		LuaIntf::LuaRef copy_;

		void parse_table(const LuaIntf::LuaRef& table);
		void run_function(LuaIntf::LuaRef& function) const;

		static bool is_function(const LuaIntf::LuaRef& function);
	};

}
