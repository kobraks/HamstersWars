#pragma once
#include "Entity.h"


#include "Register.h"

namespace game::entity
{
	class EntityScriptHandler : public lua::Register
	{
	public:
		EntityScriptHandler();
		explicit EntityScriptHandler(Entity* entity);

		void add_property(const property_id_type& property_id, LuaIntf::LuaRef lua);
		LuaIntf::LuaRef get_property(const property_id_type& property_id);

		void clone();
		void destroy();
	protected:
		void register_class(LuaIntf::LuaBinding& binding) const override;

	private:
		entity::Entity* entity_ = nullptr;
	};
}
