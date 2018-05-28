#include "SceneManager.h"
#include "Lua/lua.hpp"
#include "lua/LuaIntf.h"
#include "EntityLoader.h"

#include "Log.h"

game::SceneManager::SceneManager()
{
}

game::SceneManager::SceneManager(const gl::Program& program,
	shader_behavior behavior) : shader_(program), shader_behavior_(behavior), handler_(new CollisionHandler(this))
{
}


game::SceneManager::~SceneManager()
{
	delete handler_;
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
	destroy();

	handler_->update();
}

void game::SceneManager::set_shader(const gl::Program& program)
{
	shader_ = program;
}

void game::SceneManager::set_shader_behavior(shader_behavior& behavior)
{
	shader_behavior_ = behavior;
}

void game::SceneManager::load_from_file(const std::string& file_name)
{
	entities_ = EntityLoader::load(file_name);
}

std::vector<std::shared_ptr<game::Entity>> game::SceneManager::get_entites() const
{
	return entities_;
}

void game::SceneManager::destroy(const std::shared_ptr<Entity> entity)
{
	to_destroy_.push(entity);
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
