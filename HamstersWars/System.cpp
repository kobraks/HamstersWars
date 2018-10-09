#include "stdafx.h"
#include "System.h"
#include "Entity.h"
#include "Log.h"

namespace game::interfaces
{
	System::System(const system_id_type& system_id) : id_(system_id)
	{
	}

	System::~System()
	{
		drop_all_entities();
	}

	const system_id_type& System::id() const
	{
		return id_;
	}

	const entity_id_type& System::add_entity(entity_pointer_type entity)
	{
		if (!entity)
		{
			LOG(LOG_DEBUG, "NULL Pointer provided");
			return 0;
		}

		if (!contains_entity(entity->id()))
		{
			entity_list_[entity->id()] = entity;

			LOG(LOG_DEBUG, "Adding new entity to system");
			return entity->id();
		}

		return 0;
	}

	bool System::contains_entity(const entity_id_type& entity_id)
	{
		return entity_list_.find(entity_id) != entity_list_.end();
	}

	void System::drop_entity(const entity_id_type& entity_id)
	{
		entity_list_type::iterator iter = entity_list_.find(entity_id);

		if (iter != entity_list_.end())
			drop_entity(iter);
	}

	void System::drop_all_entities()
	{
		auto iter = entity_list_.begin();

		while (iter != entity_list_.end())
			drop_entity(iter);

		entity_list_.clear();
	}

	void System::handle_events(sf::Event event)
	{
	}

	void System::handle_entity_cleanup(entity_pointer_type entity)
	{
	}

	void System::drop_entity(entity_list_type::iterator iterator)
	{
		if (iterator == entity_list_.end())
			return;

		auto entity = iterator->second;

		entity_list_.erase(iterator);

		handle_entity_cleanup(entity);
		if (entity->has_system(id()))
			entity->drop_system(id());
	}
}
