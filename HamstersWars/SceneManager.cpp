#include "stdafx.h"
#include "SceneManager.h"
#include "EntityLoader.h"

#include <algorithm>

#include "Log.h"
#include "ScriptHandler.h"
#include "GraphicComponent.h"

game::SceneManager::SceneManager()
{
}


game::SceneManager::~SceneManager()
{
}

void game::SceneManager::add(const Entity& entity)
{
	auto pos = std::find(entities_.begin(), entities_.end(), entity);

	if (pos == entities_.end())
		entities_.push_back(entity);
}

void game::SceneManager::remove(const Entity& entity)
{
	to_remove_.push(entity);
}

void game::SceneManager::clone(const Entity& entity)
{
	auto clone = Entity(entity->copy());
	add(clone);
}

void game::SceneManager::load_entity(const std::string& entity_file, const std::string& entity_name)
{
	add(EntityLoader::load_entity(entity_name, entity_file));
}

void game::SceneManager::load_entittes(const std::string& entities_file)
{
	entities_ = EntityLoader::load(entities_file);
}

void game::SceneManager::register_class(LuaIntf::LuaBinding& binding) const
{
}

void game::SceneManager::draw(gl::Program& program, game::Transform& transform)
{
	for (auto entity : entities_)
	{
		auto graphic = entity->get_component<component::GraphicComponent>();

		if (graphic)
			graphic->draw(program, transform);
	}
}

void game::SceneManager::update(const float& time_step)
{
	remove();

	for (auto entity : entities_)
	{
		auto handler = entity->get_component<component::ScriptHandler>();

		if (handler)
			handler->on_update();
	}
}

void game::SceneManager::remove()
{
	while(!to_remove_.empty())
	{
		auto entity = to_remove_.pop();
		auto handler = entity->get_component<component::ScriptHandler>();

		if (handler)
			handler->on_destroy();

		auto pos = std::find(entities_.begin(), entities_.end(), entity);
		entities_.erase(pos);
	}
}

/*void game::SceneManager::initialize(const gl::Program& program, shader_behavior behavior)
{
	auto instance = get_instance();

	instance->shader_behavior_ = behavior;
	instance->shader_ = program;

	delete instance->handler_;
	instance->handler_ = new CollisionHandler(instance);
	instance->draw_bounding_boxes_ = false;
}*/

/*
void game::SceneManager::draw()
{
	auto instance = get_instance();

	instance->shader_.use();

	for (auto entity : instance->entities_)
	{
		auto component = entity->get_component<component::GraphicComponent>();
		//if (component)
			//component->draw();
	}

	/*shader_.use();

	for (auto model : models_)
	{
		if (shader_behavior_)
			shader_behavior_(shader_, model);
		else
			model->draw();
	}#1#
}

void game::SceneManager::update()
{
	get_instance()->destroy();

	get_instance()->handler_->update();

	for (auto entity : get_instance()->entities_)
	{
		auto script = entity->get_component<component::ScriptHandler>();
		if (script)
			script->on_update();
	}
}

void game::SceneManager::set_shader(const gl::Program& program)
{
	get_instance()->shader_ = program;
}

void game::SceneManager::set_shader_behavior(shader_behavior& behavior)
{
	get_instance()->shader_behavior_ = behavior;
}

void game::SceneManager::load_from_file(const std::string& file_name)
{
	get_instance()->entities_ = EntityLoader::load(file_name);
}

std::vector<std::shared_ptr<game::Entity>> game::SceneManager::get_entites()
{
	return get_instance()->entities_;
}

void game::SceneManager::destroy(const std::shared_ptr<Entity> entity)
{
	get_instance()->to_destroy_.push(entity);
}

game::CollisionHandler* game::SceneManager::get_collision_handler()
{
	return get_instance()->handler_;
}

void game::SceneManager::draw_bounding_boxes(bool enable)
{
	get_instance()->draw_bounding_boxes_ = enable;
}

game::SceneManager* game::SceneManager::get_instance()
{
	static SceneManager instance;

	return &instance;
}

void game::SceneManager::destroy()
{
	while(!to_destroy_.empty())
	{
		auto entity = to_destroy_.top();
		to_destroy_.pop();

		entity->get_component<component::ScriptHandler>()->on_destroy();

		auto iter = std::find(entities_.begin(), entities_.end(), entity);

		if (iter != entities_.end())
			entities_.erase(iter);
	}
}
*/
