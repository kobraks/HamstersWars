#pragma once
#include "Component.h"

namespace game
{
	namespace entity
	{
		class Entity;
		class EntityScriptHandler;
	}
	namespace component
	{
		class ScriptComponent : public interfaces::Component
		{
		public:
			ScriptComponent();
			~ScriptComponent();

			void parse_table(LuaIntf::LuaRef table) override;
			Component* clone() const override;

			void update();
			void create();
			void copy();
			void destroy();

			void set_entity(entity::Entity* entity);

		private:
			entity::Entity* entity_ = nullptr;
			entity::EntityScriptHandler* handler_ = nullptr;
		};
	}
}

