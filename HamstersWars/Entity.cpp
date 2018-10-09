#include "stdafx.h"
#include "Entity.h"
#include "System.h"

namespace game
{
	Entity::Entity() : id_(get_id())
	{
	}

	Entity::Entity(const std::string name) : id_(get_id()), name_(name)
	{
	}

	Entity::Entity(const Entity& other) : id_(get_id())
	{
		name_ = other.name_;

		for(auto pair : other.systems_)
		{
			auto system = pair.second;
			add_system(system);
			system->add_entity(this);
		}
	}

	Entity& Entity::operator=(const Entity& other)
	{
		drop_all_systems();
		name_ = other.name_;
		systems_.reserve(other.systems_.size());

		for (auto pair : other.systems_)
		{
			auto system = pair.second;
			add_system(system);
			system->add_entity(this);
		}

		return *this;
	}

	Entity::~Entity()
	{
		drop_all_systems();
	}

	const entity_id_type& Entity::id() const
	{
		return id_;
	}

	void Entity::add_system(interfaces::System* system)
	{
		if (!has_system(system->id()))
		{
			systems_[utils::to_upper_copy(utils::trim_copy(system->id()))] = system;
		}
	}

	void Entity::drop_system(const system_id_type& system_id)
	{
		system_list_type::iterator pair = systems_.find(utils::to_upper_copy(utils::trim_copy(system_id)));

		if (pair != systems_.end())
			erase_system(pair);
	}

	void Entity::drop_all_systems()
	{
		auto pair = systems_.begin();

		while (pair != systems_.end())
			erase_system(pair);

		systems_.clear();
	}

	bool Entity::has_system(const system_id_type& system_id)
	{
		return systems_.find(utils::to_upper_copy(utils::trim_copy(system_id))) != systems_.end();
	}

	void Entity::set_name(const std::string& name)
	{
		name_ = name;
	}

	std::string Entity::get_name() const
	{
		return name_;
	}

	void Entity::erase_system(system_list_type::iterator pair)
	{
		auto system = pair->second;

		systems_.erase(pair);

		if (system->contains_entity(id_))
			system->drop_entity(id_);
	}
}
