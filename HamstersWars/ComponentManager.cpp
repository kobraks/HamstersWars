#include "stdafx.h"
#include "ComponentManager.h"
#include "Component.h"

namespace game
{
	ComponentManager::ComponentManager(const ComponentManager& manager)
	{
		for (auto component : manager.components_)
			components_[component.first] = component.second->clone();
	}

	ComponentManager& ComponentManager::operator=(const ComponentManager& manager)
	{
		for (auto component : manager.components_)
			components_[component.first] = component.second->clone();

		return *this;
	}

	ComponentManager::~ComponentManager()
	{
		clear();
	}

	void ComponentManager::add(const std::type_index& type_index, component* component)
	{
		auto comp = components_.find(type_index);

		if (comp != components_.end())
			delete comp->second;

		components_[type_index] = component;
	}

	void ComponentManager::clear()
	{
		for (auto component : components_)
			delete component.second;

		components_.clear();
	}
}
