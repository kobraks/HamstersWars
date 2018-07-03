#pragma once
#include <string>
#include <memory>
#include <vector>

#include "Model.h"
#include "Material.h"

namespace game::model
{
	class ModelLoader
	{
	public:
		ModelLoader() = delete;
		ModelLoader(const ModelLoader&) = delete;
		ModelLoader(ModelLoader&&) = delete;

		ModelLoader& operator=(const ModelLoader&) = delete;
		ModelLoader& operator=(ModelLoader&&) = delete;

		static std::shared_ptr<Model> load(const std::string& file_name);

	private:
		static std::shared_ptr<Mesh> process_mesh(const void* scene, const size_t& index);
		static Material process_material(void* mesh, const void* scene);
	};
}

