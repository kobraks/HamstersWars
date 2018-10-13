#include "stdafx.h"
#include "GraphicComponent.h"


namespace game::component
{
	GraphicComponent::GraphicComponent() : Component(GRAPHIC_COMPONENT)
	{

	}

	void GraphicComponent::parse_table(LuaIntf::LuaRef table)
	{

	}

	interfaces::Component* GraphicComponent::clone() const
	{
		auto component = new GraphicComponent();

		component->properties = properties;
		return component;
	}
}
