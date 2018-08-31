#pragma once
#include <functional>
#include <vector>
#include <stack>
#include <string>
#include <memory>

#include "Program.h"
#include "Model.h"
#include "Entity.h"
#include "CollisionHandler.h"
#include "Types.h"
#include "Register.h"
#include "Drawable.h"
#include "Updatable.h"
#include "Stack.h"

namespace game
{
	class SceneManager : public lua::Register, public Drawable, public Updatable
	{
	public:
		typedef std::shared_ptr<Entity> Entity;
		typedef std::vector<Entity> entity_list;
		typedef Stack<Entity> removal_list;

		SceneManager();
		~SceneManager();

		void add(const Entity& entity);
		void remove(const Entity& entity);
		void clone(const Entity& entity);

		void load_entity(const std::string& entity_file, const std::string& entity_name);
		void load_entittes(const std::string& entities_file);

	protected:
		void register_class(LuaIntf::LuaBinding& binding) const override;

	public:
		void draw(gl::Program& program, game::Transform& transform) override;
		void update(const float& time_step) override;

	private:
		entity_list entities_;
		removal_list to_remove_;

		void remove();
	};
}

