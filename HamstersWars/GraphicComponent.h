#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <Lua/LuaIntf.h>

#include "Component.h"
#include "Model.h"
#include "Texture2d.h"
#include "Transformable.h"
#include "Drawable.h"

namespace game::component
{
	class GraphicComponent : public Component
	{
	public:
		GraphicComponent(std::shared_ptr<Entity> owner, const LuaIntf::LuaRef& table);
		~GraphicComponent();

		bool drawable() const;
		void set_drawable(const bool& enable);

		std::shared_ptr<model::Model> get_model() const;

		void load(const std::string& file);
		void draw() const;

		void scale(const glm::vec3& axis);
		void rotate(const float& angle, const glm::vec3& axis);
		void translate(const glm::vec3& axis);

		void force_texture(const std::string& file_name);
		void set_shader_behavior();

		Component* copy() const override;
	private:
		std::shared_ptr<model::Model> model_;
		std::shared_ptr<model::Texture2d> texture_;
		bool drawable_;

		void draw_forced_texture();
	};
}

