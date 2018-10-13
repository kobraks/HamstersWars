#pragma once
#include "TProperty.h"
#include "IProperty.h"
#include "Utils.h"

namespace game
{
	class PropertyManager
	{
	public:
		typedef std::unordered_map<property_id_type, interfaces::IProperty*> property_list_type;

		class iterator
		{
		public:
			typedef iterator self_type;
			typedef interfaces::IProperty value_type;
			typedef value_type*& reference;
			typedef value_type* pointer;
			typedef std::forward_iterator_tag iterator_category;
			typedef std::ptrdiff_t difference_type;

			explicit iterator(property_list_type::iterator iterator);
			self_type& operator++();

			reference operator*() const;
			pointer operator->() const;

			bool operator==(const self_type& right) const;
			bool operator!=(const self_type& right) const;
		private:
			property_list_type::iterator current_;
		};

		class const_iterator
		{
		public:
			typedef const_iterator self_type;
			typedef interfaces::IProperty value_type;
			typedef value_type*& reference;
			typedef value_type* pointer;
			typedef std::forward_iterator_tag iterator_category;
			typedef std::ptrdiff_t difference_type;

			explicit const_iterator(property_list_type::const_iterator iterator);
			self_type& operator++();

			const value_type* operator*()const;
			const pointer operator->() const;

			bool operator==(const self_type& right) const;
			bool operator!=(const self_type& right) const;
		private:
			property_list_type::const_iterator current_;
		};

		PropertyManager();
		PropertyManager(PropertyManager& other);
		PropertyManager& operator=(const PropertyManager& other);
		~PropertyManager();

		template<class Type>
		Type get(const property_id_type& property_id) const;

		template<class Type>
		interfaces::TProperty<Type> get_property(const property_id_type& property_id) const;

		template<class Type>
		void set(const property_id_type& property_id, Type value);

		template<class Type>
		void add(const property_id_type& property_id, Type value);

		bool contains(const property_id_type& property_id) const;
		void add(interfaces::IProperty* property);

		void clear();
		void remove(const property_id_type& property_id);

		void clone(const PropertyManager& manager);

		iterator find(const property_id_type& property_id);
		const_iterator find(const property_id_type& property_id) const;

		iterator begin();
		const_iterator begin() const;

		iterator end();
		const_iterator end() const;
	private:
		property_list_type list_;

		template<class Type>
		property_list_type::iterator find(const property_id_type& property_id);

		template<class Type>
		property_list_type::const_iterator find(const property_id_type& property_id) const;

		template<class Type>
		void add_property(interfaces::TProperty<Type> property);
	};

	template <class Type>
	Type PropertyManager::get(const property_id_type& property_id) const
	{
		return get_property<Type>(property_id).get_value();
	}

	template <class Type>
	interfaces::TProperty<Type> PropertyManager::get_property(const property_id_type& property_id) const
	{
		auto iterator = find<Type>(property_id);

		if (iterator != list_.end())
			return *static_cast<interfaces::TProperty<Type>*>(iterator->second);

		LOG(LOG_ERROR, "Unable to find property with id: %s", property_id.c_str());
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
			add_property<Type>(interfaces::TProperty<Type>(property_id, value));
		else
			LOG(LOG_WARNING, "Property already exists");
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
	std::unordered_map<std::basic_string<char>, interfaces::IProperty*>::const_iterator PropertyManager::find(
		const property_id_type& property_id) const
	{
		auto iterator = list_.find(utils::to_upper_copy(utils::trim_copy(property_id)));
		if (iterator != list_.end() && iterator->second->type()->name() != GET_TYPE_NAME(Type))
			iterator = list_.end();

		return iterator;
	}

	template <class Type>
	void PropertyManager::add_property(interfaces::TProperty<Type> property)
	{
		list_[utils::to_upper_copy(utils::trim_copy(property.id()))] = property.clone();
	}
}

