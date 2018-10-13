#include "stdafx.h"
#include "Component.h"


namespace game::interfaces
{
	Component::Component(const component_id_type& component_id) : id_(component_id)
	{
	}

	const component_id_type& Component::id() const
	{
		return id_;
	}
}
