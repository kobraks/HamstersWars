#include "SceneManager.h"
#include "Lua/lua.hpp"
#include "lua/LuaIntf.h"
#include "EntityLoader.h"

#include "Log.h"

game::SceneManager::SceneManager()
{
}


game::SceneManager::~SceneManager()
{
	delete handler_;
}

void game::SceneManager::initialize(const gl::Program& program, shader_behavior behavior)
{
	auto instance = get_instance();

	instance->shader_behavior_ = behavior;
	instance->shader_ = program;

	delete instance->handler_;
	instance->handler_ = new CollisionHandler(instance);
	instance->draw_bounding_boxes_ = false;
}

void game::SceneManager::draw()
{
	/*shader_.use();

	for (auto model : models_)
	{
		if (shader_behavior_)
			shader_behavior_(shader_, model);
		else
			model->draw();
	}*/
}

void game::SceneManager::update()
{
	get_instance()->destroy();

	get_instance()->handler_->update();
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

		auto iter = std::find(entities_.begin(), entities_.end(), entity);

		if (iter != entities_.end())
			entities_.erase(iter);
	}
}
