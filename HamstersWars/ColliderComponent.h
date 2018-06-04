#pragma once
#include "Component.h"

namespace game::component
{
	class ColliderComponent : public Component
	{
	public:
		ColliderComponent(std::shared_ptr<Entity> owner);

		Component* copy() const override;
		~ColliderComponent();
	};
}

