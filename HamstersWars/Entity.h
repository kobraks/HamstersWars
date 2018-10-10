#pragma once

#include "Types.h"
#include "PropertyManager.h"
#include "Component.h"

namespace game
{
	namespace interfaces
	{
		class System;
	}

	class Entity
	{
	public:
		typedef std::unordered_map<system_id_type, interfaces::System*> system_list_type;
		PropertyManager properties;

		Entity();
		explicit Entity(const std::string name);
		Entity(const Entity& other);

		Entity& operator=(const Entity& other);
		~Entity();

		const entity_id_type& id() const;
		
		void add_system(interfaces::System* system);

		void drop_system(const system_id_type& system_id);
		void drop_all_systems();

		bool has_system(const system_id_type& system_id);

		void set_name(const std::string& name);
		std::string get_name() const;
	protected:
		system_list_type systems_;

	private:
		static inline entity_id_type next_id_ = 0;
		const entity_id_type id_;

		static const entity_id_type get_id();
		std::string name_;

		void erase_system(system_list_type::iterator pair);
	};
}

