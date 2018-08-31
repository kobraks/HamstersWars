#pragma once
#include "Entity.h"

#include <Lua/lua.hpp>
#include <Lua/LuaIntf.h>
#include <memory>
#include "Register.h"
#include "Vector3D.h"

namespace game::script
{
	class EntityScriptHandler : public lua::Register
	{
	public:
		EntityScriptHandler() = default;
		EntityScriptHandler(std::shared_ptr<Entity> entity);
		~EntityScriptHandler();

		float get_elapsed_time();

		void rotate(const gl::Vector3D& axis);
		void translate(const gl::Vector3D& axis);
		
		void set_position(const gl::Vector3D& pos);
		void move(const gl::Vector3D& vector);

		glm::vec3 get_position();

		void scale(const gl::Vector3D& axis);

		void destroy();

		bool is_colliding();

		std::vector<EntityScriptHandler> get_colliders();

		void set_texture(const std::string& file);
		void set_model(const std::string& file);
	protected:
		void register_class(LuaIntf::LuaBinding& binding) const override;

	private:
		std::shared_ptr<Entity> entity_;
	};
}
