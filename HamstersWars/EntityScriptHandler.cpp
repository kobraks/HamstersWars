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
		LOG(LOG_WARNING, "%s has no grahpic component to rotate", entity_->get_type().c_str());
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
		LOG(LOG_WARNING, "%s has no grahpic component to rotate", entity_->get_type().c_str());
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
		LOG(LOG_WARNING, "%s has no grahpic component to move", entity_->get_type().c_str());
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
		LOG(LOG_WARNING, "%s has no grahpic component to scale", entity_->get_type().c_str());
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
	LOG(LogLevel::from_string(log), "%s", log.c_str());
}

void game::script::EntityScriptHandler::print_error(const std::string& error_message)
{
	LOG(LOG_ERROR, error_message.c_str());
}
