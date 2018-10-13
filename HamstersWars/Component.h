#pragma once
#include "PropertyManager.h"

namespace game
{
	namespace interfaces
	{
		class Component
		{
		public:
			PropertyManager properties;

			explicit Component(const component_id_type& component_id);
			virtual ~Component() = default;

			const component_id_type& id() const;

			virtual void parse_table(LuaIntf::LuaRef table) = 0;
			virtual Component* clone() const = 0;
		private:
			const component_id_type& id_;
		};
	}
}

