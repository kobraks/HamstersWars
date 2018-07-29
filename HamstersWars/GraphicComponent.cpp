#include "GraphicComponent.h"
#include "Entity.h"
#include "ModelLoader.h"
#include "Utils.h"
#include "UnknownTableElementException.h"
#include "UnableToFindSpecifedTableKeyException.h"
#include "Log.h"

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
	Log::level() = Log::log_info;
	Log::print("Attempting to load file: %s", file.c_str());

	auto path = file;
	if (utils::is_local_path(file))
	{
		path.insert(0, MODELS_PATH);

		Log::print("Detected as local path so model will be loaded from: %s", path.c_str());

	}

	auto model = models_.find(path);


	if (model != models_.end())
	{
		model_ = model->second;

		Log::print("Model was loaded before");
	}
	else
	{
		auto model = model::ModelLoader::load(path);
		
		if (!model)
			return;

		models_.insert(std::pair<std::string, std::shared_ptr<model::Model>>(path, model));
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

void game::component::GraphicComponent::force_texture(const int& id, const std::string& file_name)
{
}

game::component::Component* game::component::GraphicComponent::copy() const
{
	return new GraphicComponent(*this);
}

std::string game::component::GraphicComponent::get_name() const
{
	return "GraphicComponent";
}

void game::component::GraphicComponent::clear_loaded_models()
{
	models_.clear();
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
			Log::level() = Log::log_info;
			Log::print("Attempting to load model path");
			load(utils::get_path(value));

		}
		else if (key == "VISIBLE")
		{
			Log::level() = Log::log_info;
			Log::print("Attempting to load drawable value");
			drawable_ = element.value<bool>();
			Log::print("Visible value: %s", drawable_ ? "true" : "false");
		}
		else if (value.isTable())
			try
			{
				Log::level() = Log::log_info;
				Log::print("Attempting to load mesh values");
				parse_mesh(value);
			}
			catch(exception::GameException& ex)
			{
				Log::level() = Log::log_error;
				Log::print(ex.what());
			}
		else
		{
			Log::level() = Log::log_warning;
			Log::print("Unable to recognize gived table_type key: %s", element.key<std::string>().c_str());
		}
	}
}

void game::component::GraphicComponent::parse_mesh(const LuaIntf::LuaRef& table)
{
	assert(table.isTable());

	size_t id = 0;
	std::string path;

	bool id_found = false;
	bool path_found = false;

	for (auto element : table)
	{
		if (utils::equals(element.key<std::string>(), "id"))
			id = element.value<size_t>();
		else if (utils::equals(element.key<std::string>(), "texture"))
			path = utils::get_path(element.value());
		else
		{
			Log::level() = Log::log_warning;
			Log::print("Unable to recognize gived table_type key: %s", element.key<std::string>().c_str());
		}

	}

	if (!id_found)
		throw exception::UnableToFindSpecifedTableKeyException("id");
	if (!path_found)
		throw exception::UnableToFindSpecifedTableKeyException("texture");

	force_texture(id, path);
}
