#include "EntityScriptHandler.h"
#include "Log.h"

#include "SceneManager.h"

game::script::EntityScriptHandler::EntityScriptHandler(std::shared_ptr<Entity> entity) : entity_(entity)
{
}


game::script::EntityScriptHandler::~EntityScriptHandler()
{
}

float game::script::EntityScriptHandler::get_elapsed_time()
{
	return 1.f;
}

void game::script::EntityScriptHandler::rotate(float angle, const glm::vec3& axis)
{
}

void game::script::EntityScriptHandler::translate(const glm::vec3& axis)
{
}

void game::script::EntityScriptHandler::scale(const glm::vec3& axis)
{
}

void game::script::EntityScriptHandler::destroy()
{
}

bool game::script::EntityScriptHandler::is_colliding()
{
	return SceneManager::get_collision_handler()->has_collision(entity_);
}

std::vector<game::script::EntityScriptHandler> game::script::EntityScriptHandler::get_colliders()
{
	auto colliders = SceneManager::get_collision_handler()->get_colliders(entity_);

	return std::vector<game::script::EntityScriptHandler>();
}

void game::script::EntityScriptHandler::print_log(const std::string& log_level, const std::string& log)
{
	Log::level() = Log::log_level_from_string(log_level);
	Log::print("%s", log.c_str());
}