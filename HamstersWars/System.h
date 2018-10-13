#pragma once
#include <SFML/Window/Event.hpp>
#include <deque>

#include "Types.h"


namespace game
{
	namespace entity
	{
		class Entity;
	}
}

namespace game::interfaces
{
	class System
	{
	public:
		typedef entity::Entity* entity_pointer_type;
		typedef std::unordered_map<entity_id_type, entity_pointer_type> entity_list_type;
		explicit System(const system_id_type& system_id);
		virtual ~System();

		const system_id_type& id() const;
		const entity_id_type add_entity(entity_pointer_type entity);
		virtual void populate_entity(entity_pointer_type entity) = 0;
		bool contains_entity(const entity_id_type& entity_id);
		
		void drop_entity(const entity_id_type& entity_id);
		void drop_all_entities();

		virtual void handle_events(sf::Event event);
		virtual void update() = 0;
		virtual void update_variables(const float& elapsed_time) = 0;
		virtual void draw() = 0;
	protected:
		entity_list_type entity_list_;

		virtual void handle_entity_init(entity_pointer_type entity) = 0;
		virtual void handle_entity_cleanup(entity_pointer_type entity);
	private:
		const system_id_type id_;

		void drop_entity(entity_list_type::iterator iterator);
	};
}
