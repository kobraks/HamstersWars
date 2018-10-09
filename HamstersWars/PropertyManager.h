#pragma once
#include "TProperty.h"
#include "IProperty.h"
#include "Utils.h"

namespace game
{
	class PropertyManager
	{
	public:
		typedef std::unordered_map<property_id_type, interfaces::IProperty*> list_type;

		PropertyManager();
		PropertyManager(PropertyManager& other);
		PropertyManager& operator=(const PropertyManager& other);
		~PropertyManager();

		template<class Type>
		Type get(const property_id_type& property_id);

		template<class Type>
		interfaces::TProperty<Type>& get_property(const property_id_type& property_id);

		template<class Type>
		void set(const property_id_type& property_id, Type value);

		template<class Type>
		void add(const property_id_type& property_id, Type value);

		bool contains(const property_id_type& property_id) const;
		void add(interfaces::IProperty* property);

		void clear();

	private:
		list_type list_;

		template<class Type>
		list_type::iterator find(const property_id_type& property_id);

		template<class Type>
		void add_property(interfaces::TProperty<Type>& property);
	};

	template <class Type>
	Type PropertyManager::get(const property_id_type& property_id)
	{
		return get_property<Type>(property_id).get_value();
	}

	template <class Type>
	interfaces::TProperty<Type>& PropertyManager::get_property(const property_id_type& property_id)
	{
		auto iterator = find<Type>(property_id);

		if (iterator != list_.end())
			return *static_cast<interfaces::TProperty<Type>*>(iterator->second);

		throw std::runtime_error(std::string("Unable to find property with id: ") + property_id);
	}

	template <class Type>
	void PropertyManager::set(const property_id_type& property_id, Type value)
	{
		auto iterator = find<Type>(property_id);

		if (iterator != list_.end())
			static_cast<interfaces::TProperty<Type>*>(iterator->second)->set_value(value);
		else
			add_property<Type>(interfaces::TProperty<Type>(property_id, value));
	}

	template <class Type>
	void PropertyManager::add(const property_id_type& property_id, Type value)
	{
		if (!contains(property_id))
		{
			add_property<Type>(interfaces::TProperty<Type>(property_id, value));
		}
	}
	
	template <class Type>
	std::unordered_map<std::basic_string<char>, interfaces::IProperty*>::iterator PropertyManager::find(
		const property_id_type& property_id)
	{
		auto iterator = list_.find(utils::to_upper_copy(utils::trim_copy(property_id)));
		if (iterator != list_.end() && iterator->second->type()->name() != GET_TYPE_NAME(Type))
			iterator = list_.end();

		return iterator;
	}

	template <class Type>
	void PropertyManager::add_property(interfaces::TProperty<Type>& property)
	{
		list_[utils::to_upper_copy(utils::trim_copy(property.id()))] = property.clone();
	}
}

