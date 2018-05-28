#include "EntityScriptHandler.h"
#include "Log.h"


game::script::EntityScriptHandler::EntityScriptHandler(Entity* entity) : entity_(entity)
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

game::script::EntityScriptHandler* game::script::EntityScriptHandler::get_collider()
{
	return &EntityScriptHandler(entity_);
}

void game::script::EntityScriptHandler::print_log(const std::string& log_level, const std::string& log)
{
	Log::level() = Log::log_level_from_string(log_level);
	Log::print("%s", log.c_str());
}
