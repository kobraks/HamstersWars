#include "CollisionHandler.h"

#include "SceneManager.h"
#include <algorithm>


game::CollisionHandler::CollisionHandler(SceneManager* manager) : manager_(manager)
{
}

game::CollisionHandler::~CollisionHandler()
{
}

void game::CollisionHandler::set_action_on_colision(std::shared_ptr<Entity> entity, const collision_handler& function)
{
	handlers_[entity] = function;
}

bool game::CollisionHandler::has_collision(std::shared_ptr<Entity> entity)
{
	return collisions_.find(entity) != collisions_.end();
}

std::vector<std::shared_ptr<game::Entity>> game::CollisionHandler::get_colliders(std::shared_ptr<Entity> entity)
{
	if (collisions_.find(entity) != collisions_.end())
		return collisions_[entity];
	
	return std::vector<std::shared_ptr<game::Entity>>();
}

std::shared_ptr<game::Entity> game::CollisionHandler::get_first_collider(std::shared_ptr<Entity> entity)
{
	if (collisions_.find(entity) != collisions_.end())
		return collisions_[entity][0];

	return nullptr;
}

void game::CollisionHandler::update()
{
	collisions_.clear();
	auto to_detect = manager_->get_entites();

	for(auto entity : to_detect)
		for (auto entity2 : manager_->get_entites())
			if (entity != entity2)
			{
				//TODO
			}
}


bool game::CollisionHandler::exists_vector(std::shared_ptr<Entity> entity, std::vector<std::shared_ptr<Entity>> entities)
{
	return std::find(entities.begin(), entities.end(), entity) != entities.end();
}

void game::CollisionHandler::add_colision(std::shared_ptr<Entity> entity, std::shared_ptr<Entity> entity2)
{
	run_handler(entity, entity2);

	if(collisions_.find(entity) != collisions_.end())
	{
		if (!exists_vector(entity2, collisions_[entity]))
			collisions_[entity].push_back(entity2);
	}
	else
	{
		collisions_[entity].push_back(entity2);
	}

	if (collisions_.find(entity2) != collisions_.end())
	{
		if (!exists_vector(entity, collisions_[entity2]))
			collisions_[entity2].push_back(entity);
	}
	else
	{
		collisions_[entity2].push_back(entity);
	}
}

bool game::CollisionHandler::exist_handler(std::shared_ptr<Entity> entity)
{
	return handlers_.find(entity) != handlers_.end();
}

void game::CollisionHandler::run_handler(std::shared_ptr<Entity> entity, std::shared_ptr<Entity> entity2)
{
	if (exist_handler(entity))
		handlers_[entity](entity, entity2);
	if (exist_handler(entity2))
		handlers_[entity2](entity2, entity);
}
