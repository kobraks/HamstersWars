#include "stdafx.h"
#include "PropertyManager.h"

namespace game
{
	PropertyManager::PropertyManager() = default;

	PropertyManager::PropertyManager(PropertyManager& other)
	{
		for (auto pair : other.list_)
			add(pair.second->clone());
	}

	PropertyManager& PropertyManager::operator=(const PropertyManager& other)
	{
		for (auto pair : other.list_)
			add(pair.second->clone());

		return *this;
	}

	PropertyManager::~PropertyManager()
	{
		clear();
	}

	bool PropertyManager::contains(const property_id_type& property_id) const
	{
		return list_.find(utils::to_upper_copy(utils::trim_copy(property_id))) != list_.end();
	}

	void PropertyManager::add(interfaces::IProperty* property)
	{
		auto name = utils::to_upper_copy(utils::trim_copy(property->id()));
		if (list_.find(name) == list_.end())
			list_[name] = property;
	}

	void PropertyManager::clear()
	{
		for (auto pair : list_)
			delete pair.second;

		list_.clear();
	}
}
