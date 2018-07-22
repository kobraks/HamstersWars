#include "EntityScriptHandler.h"
#include "Log.h"

#include "SceneManager.h"
#include "GraphicComponent.h"

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

void game::script::EntityScriptHandler::rotate(const glm::vec3& axis)
{
	auto component = entity_->get_component<component::GraphicComponent>();

	if (!component)
	{
		Log::level() = Log::log_error;
		Log::print("There is no grahpic component for rotate");
	}
	else
	{
		component->rotate(axis);
	}
}

void game::script::EntityScriptHandler::translate(const glm::vec3& axis)
{
	auto component = entity_->get_component<component::GraphicComponent>();

	if (!component)
	{
		Log::level() = Log::log_error;
		Log::print("There is no grahpic component for translate");
	}
	else
	{
		//component->translate(axis);
	}
}

void game::script::EntityScriptHandler::set_position(const glm::vec3& pos)
{
	translate(pos);
}

void game::script::EntityScriptHandler::move(const glm::vec3& vector)
{
	auto component = entity_->get_component<component::GraphicComponent>();

	if (!component)
	{
		Log::level() = Log::log_error;
		Log::print("There is no grahpic component for move");
	}
	else
	{
		component->move(vector);
	}
}

glm::vec3 game::script::EntityScriptHandler::get_position()
{
	return glm::vec3();
}

void game::script::EntityScriptHandler::scale(const glm::vec3& axis)
{
	auto component = entity_->get_component<component::GraphicComponent>();

	if (!component)
	{
		Log::level() = Log::log_error;
		Log::print("There is no grahpic component for rotate");
	}
	else
	{
		component->scale(axis);
	}
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

	std::vector<game::script::EntityScriptHandler> result;
	result.reserve(colliders.size());

	for (auto collider : colliders)
		result.push_back(EntityScriptHandler(collider));

	return result;
}

void game::script::EntityScriptHandler::set_texture(const std::string& file)
{

}

void game::script::EntityScriptHandler::set_model(const std::string& file)
{
}

void game::script::EntityScriptHandler::print_log(const std::string& log_level, const std::string& log)
{
	Log::level() = Log::log_level_from_string(log_level);
	Log::print("%s", log.c_str());
}

void game::script::EntityScriptHandler::print_error(const std::string& error_message)
{
	Log::write_error(error_message);
}
