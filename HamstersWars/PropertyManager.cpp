#include "stdafx.h"
#include "PropertyManager.h"

namespace game
{
	PropertyManager::iterator::iterator(property_list_type::iterator iterator) : current_(iterator)
	{
	}

	PropertyManager::iterator::self_type& PropertyManager::iterator::operator++()
	{
		++current_;
		return *this;
	}

	PropertyManager::iterator::reference PropertyManager::iterator::operator*() const
	{
		return current_->second;
	}

	PropertyManager::iterator::pointer PropertyManager::iterator::operator->() const
	{
		return current_->second;
	}

	bool PropertyManager::iterator::operator==(const self_type& right) const
	{
		return current_ == right.current_;
	}

	bool PropertyManager::iterator::operator!=(const self_type& right) const
	{
		return current_ != right.current_;
	}

	PropertyManager::const_iterator::const_iterator(property_list_type::const_iterator iterator) : current_(iterator)
	{
	}

	PropertyManager::const_iterator::self_type& PropertyManager::const_iterator::operator++()
	{
		++current_;
		return *this;
	}

	const PropertyManager::const_iterator::value_type* PropertyManager::const_iterator::operator*() const
	{
		return current_->second;
	}

	const PropertyManager::const_iterator::pointer PropertyManager::const_iterator::operator->() const
	{
		return current_->second;
	}

	bool PropertyManager::const_iterator::operator==(const self_type& right) const
	{
		return current_ == right.current_;
	}

	bool PropertyManager::const_iterator::operator!=(const self_type& right) const
	{
		return current_ != right.current_;
	}

	PropertyManager::PropertyManager() = default;

	PropertyManager::PropertyManager(PropertyManager& other)
	{
		for (auto pair : other.list_)
			add(pair.second->clone());
	}

	PropertyManager& PropertyManager::operator=(const PropertyManager& other)
	{
		clone(other);

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

	void PropertyManager::remove(const property_id_type& property_id)
	{
		property_list_type::iterator iterator = list_.find(utils::to_upper_copy(utils::trim_copy(property_id)));

		if (iterator != list_.end())
		{
			LOG(LOG_INFO, "Removing %s property", property_id.c_str());

			delete iterator->second;
			list_.erase(iterator);
		}
	}

	void PropertyManager::clone(const PropertyManager& manager)
	{
		for (auto pair : manager.list_)
		{
			if (!contains(pair.first))
				add(pair.second->clone());
		}
	}

	PropertyManager::iterator PropertyManager::find(const property_id_type& property_id)
	{
		return iterator(list_.find(utils::to_upper_copy(utils::trim_copy(property_id))));
	}

	PropertyManager::const_iterator PropertyManager::find(const property_id_type& property_id) const
	{
		return const_iterator(list_.find(utils::to_upper_copy(utils::trim_copy(property_id))));
	}

	PropertyManager::iterator PropertyManager::begin()
	{
		return iterator(list_.begin());
	}

	PropertyManager::const_iterator PropertyManager::begin() const
	{
		return const_iterator(list_.begin());
	}

	PropertyManager::iterator PropertyManager::end()
	{
		return iterator(list_.end());
	}

	PropertyManager::const_iterator PropertyManager::end() const
	{
		return const_iterator(list_.end());
	}
}
