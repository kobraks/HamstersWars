#include "Model.h"
#include <utility>
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.h"


inline void set_greater_value(glm::vec3& value, const glm::vec3& right)
{
	if (value.x < right.x)
		value.x = right.x;

	if (value.y < right.y)
		value.y = right.y;

	if (value.z < right.z)
		value.z = right.z;
}

game::model::Model::Model(std::vector<std::shared_ptr<Mesh>> meshes) : meshes_(std::move(meshes))
{
	for (auto mesh : meshes)
	{
		set_greater_value(size_, mesh->size());
	}
}

game::model::Model::Model() = default;

game::model::Model::Model(const Model& model)
{
	meshes_.clear();
	meshes_.reserve(model.meshes_.size());
	size_ = model.size_;
	size_needs_update_ = model.size_needs_update_;

	for (auto mesh : model.meshes_)
		meshes_.push_back(mesh);
}

game::model::Model& game::model::Model::operator=(const Model& model)
{
	meshes_.clear();
	meshes_.reserve(model.meshes_.size());
	size_ = model.size_;
	size_needs_update_ = model.size_needs_update_;

	for (auto mesh : model.meshes_)
		meshes_.push_back(mesh);

	return *this;
}

game::model::Model::~Model()
{
}

void game::model::Model::draw_mesh(unsigned mesh) const
{
	if (mesh > meshes_.size())
		return;

	meshes_[mesh]->draw();

}

std::shared_ptr<game::model::Mesh> game::model::Model::get_mesh(unsigned mesh) const
{
	if (mesh > meshes_.size())
		return nullptr;

	return meshes_[mesh];
}

size_t game::model::Model::count() const
{
	return meshes_.size();
}

void game::model::Model::add_mesh(const Mesh& mesh)
{
	meshes_.push_back(std::make_shared<Mesh>(mesh));

	set_greater_value(size_, mesh.size());
	size_needs_update_ = false;
}

bool game::model::Model::colide(const Model& model)
{
	for (auto mesh : meshes_)
		for (auto sub_mesh : model.meshes_)
			if (mesh->get_bounding_box().intersect(sub_mesh->get_bounding_box()))
				return true;

	return false;
}

glm::vec3 game::model::Model::size() const
{
	if (size_needs_update_)
	{
		glm::mat4 scale_matrix = glm::scale(glm::mat4(1.f), get_scale());
		glm::vec4 size = glm::vec4(size_, 1.f);

		size = scale_matrix * size;

		size_ = glm::vec3(size.x, size.y, size.z);
		size_needs_update_ = false;
	}

	return size_;
}

void game::model::Model::draw() const
{
	for (auto& mesh : meshes_)
		mesh->draw();
}
