#pragma once
#include "Component.h"

namespace game
{
	namespace component
	{
		class GraphicComponent : public interfaces::Component
		{
		public:
			GraphicComponent();
			~GraphicComponent() = default;

			void parse_table(LuaIntf::LuaRef table) override;
			Component* clone() const override;
		};
	}
}

