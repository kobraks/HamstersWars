#pragma once
#include <map>
#include <vector>
#include <functional>
#include "Entity.h"

namespace game
{
	class SceneManager;

	typedef std::function<void(std::shared_ptr<Entity>, std::shared_ptr<Entity>)> collision_handler;

	class CollisionHandler
	{
	public:
		explicit CollisionHandler(SceneManager* manager);
		~CollisionHandler();

		void set_action_on_colision(std::shared_ptr<Entity> entity, const collision_handler& function);

		bool has_collision(std::shared_ptr<Entity> entity);
		std::vector<std::shared_ptr<Entity>> get_colliders(std::shared_ptr<Entity> entity);
		std::shared_ptr<Entity> get_first_collider(std::shared_ptr<Entity> entity);

		void update();

	private:
		std::map<std::shared_ptr<Entity>, collision_handler> handlers_;
		std::map<std::shared_ptr<Entity>, std::vector<std::shared_ptr<Entity>>> collisions_;
		SceneManager* manager_;

		bool exists_vector(std::shared_ptr<Entity> entity, std::vector<std::shared_ptr<Entity>> entities);
		void add_colision(std::shared_ptr<Entity> entity, std::shared_ptr<Entity> entity2);
		bool exist_handler(std::shared_ptr<Entity> entity);
		void run_handler(std::shared_ptr<Entity> entity, std::shared_ptr<Entity> entity2);
	};
}
