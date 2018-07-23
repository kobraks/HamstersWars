#include "GraphicComponent.h"
#include "Entity.h"
#include "ModelLoader.h"
#include "Utils.h"
#include "UnknownTableElementException.h"

std::unordered_map<std::string, std::shared_ptr<game::model::Model>> game::component::GraphicComponent::models_;


game::component::GraphicComponent::GraphicComponent(std::shared_ptr<Entity> owner, const LuaIntf::LuaRef& table) : Component(owner)
{
	drawable_ = true;
	model_ = nullptr;
	parse_table(table);
}

game::component::GraphicComponent::~GraphicComponent()
{
}

bool game::component::GraphicComponent::drawable() const
{
	return drawable_;
}

void game::component::GraphicComponent::set_drawable(const bool& enable)
{
	drawable_ = enable;
}

std::shared_ptr<game::model::Model> game::component::GraphicComponent::get_model() const
{
	return model_;
}

void game::component::GraphicComponent::draw(gl::Program& program, game::Transform& transform)
{
	if (model_)
	{
		program["Model"] = transform * this->get_transform();
		model_->draw();
	}
}

void game::component::GraphicComponent::load(const std::string& file)
{
	auto model = models_.find(file);


	if (model != models_.end())
		model_ = model->second;
	else
	{
		auto model = model::ModelLoader::load(file);
		models_.insert(std::pair<std::string, std::shared_ptr<model::Model>>(file, model));
		model_ = model;
	}

	set_position(model_->get_position());
	set_origin(model_->get_origin());
	set_rotation(model_->get_rotation());
	set_scale(model_->get_scale());
}

void game::component::GraphicComponent::force_texture(const std::string& file_name)
{
}

game::component::Component* game::component::GraphicComponent::copy() const
{
	return new GraphicComponent(*this);
}

void game::component::GraphicComponent::draw_forced_texture()
{

}

void game::component::GraphicComponent::parse_table(const LuaIntf::LuaRef& table)
{
	assert(table.isTable());

	for (auto element : table)
	{
		auto value = element.value();
		auto key = utils::to_upper_copy(element.key<std::string>());

		if (key == "MODEL")
		{
			if (value.isTable())
			{
				for (auto element : value)
				{
					if (utils::equals(element.key<std::string>(), "path"))
						load(element.value<std::string>());
				}
			}
		}
		else if (key == "drawable")
			drawable_ = element.value<bool>();
		else if (key == "MESH")
		{
			
		}
	}
}
