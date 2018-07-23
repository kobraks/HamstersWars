#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <Lua/lua.hpp>
#include <Lua/LuaIntf.h>
#include <unordered_map>

#include "Component.h"
#include "Model.h"
#include "Transformable.h"
#include "Drawable.h"

namespace game::component
{
	class GraphicComponent : public Component, public game::Transformable, public Drawable
	{
	public:
		GraphicComponent(std::shared_ptr<Entity> owner, const LuaIntf::LuaRef& table);
	
		GraphicComponent(const GraphicComponent&) = default;
		GraphicComponent(GraphicComponent&&) = default;

		GraphicComponent& operator=(const GraphicComponent&) = default;
		GraphicComponent& operator=(GraphicComponent&&) = default;

		~GraphicComponent();

		bool drawable() const;
		void set_drawable(const bool& enable = true);

		std::shared_ptr<model::Model> get_model() const;
		void draw(gl::Program& program, game::Transform& transofrm) override;

		void load(const std::string& file);

		void force_texture(const std::string& file_name);
		void force_texture(const int& id, const std::string& file_name);
		Component* copy() const override;

		void clear_loaded_models();
	private:
		std::shared_ptr<model::Model> model_;
		bool drawable_;

		void draw_forced_texture();

		static std::unordered_map<std::string, std::shared_ptr<model::Model>> models_;

		void parse_table(const LuaIntf::LuaRef& table);
		void parse_mesh(const LuaIntf::LuaRef& table);
	};
}

