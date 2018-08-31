#include "EntityScriptHandler.h"
#include "Log.h"

#include "SceneManager.h"
#include "GraphicComponent.h"

#define ADD_FUNCTION(x) addFunction(#x, &EntityScriptHandler::x)
#define ADD_FUNCTION_PARAMS(x,params) addFunction(#x, &EntityScriptHandler::x, params)

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

void game::script::EntityScriptHandler::rotate(const gl::Vector3D& axis)
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

void game::script::EntityScriptHandler::translate(const gl::Vector3D& axis)
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

void game::script::EntityScriptHandler::set_position(const gl::Vector3D& pos)
{
	translate(pos);
}

void game::script::EntityScriptHandler::move(const gl::Vector3D& vector)
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

void game::script::EntityScriptHandler::scale(const gl::Vector3D& axis)
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

void game::script::EntityScriptHandler::register_class(LuaIntf::LuaBinding& binding) const
{
	LOG(LOG_INFO, "Registering lua handlers");
	using namespace game::script;

	binding
		.beginClass<EntityScriptHandler>("EntityScriptHandler")
		.ADD_FUNCTION(get_elapsed_time)
		.ADD_FUNCTION_PARAMS(scale, LUA_ARGS(glm::vec3))
		.ADD_FUNCTION_PARAMS(translate, LUA_ARGS(glm::vec3))
		.ADD_FUNCTION_PARAMS(rotate, LUA_ARGS(glm::vec3))
		.ADD_FUNCTION_PARAMS(set_position, LUA_ARGS(glm::vec3))
		.ADD_FUNCTION_PARAMS(move, LUA_ARGS(glm::vec3))
		.ADD_FUNCTION(get_position)
		.ADD_FUNCTION(is_colliding)
		.ADD_FUNCTION(destroy)
		.endClass();
}
