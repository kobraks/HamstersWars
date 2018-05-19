#include "Model.h"
#include <utility>
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.h"


model::Model::Model(std::vector<Mesh*> meshes) : meshes_(std::move(meshes))
{

}

model::Model::Model() = default;

model::Model::Model(const Model& model)
{
	meshes_.reserve(model.meshes_.size());

	for (auto mesh : model.meshes_)
		meshes_.push_back(new Mesh(*mesh));
}

model::Model& model::Model::operator=(const Model& model)
{
	for (auto mesh : meshes_)
		delete mesh;
	meshes_.clear();

	meshes_.reserve(model.meshes_.size());

	for (auto mesh : model.meshes_)
		meshes_.push_back(new Mesh(*mesh));

	return *this;
}

model::Model::~Model()
{
	for (auto mesh : meshes_)
		delete mesh;
}

void model::Model::draw_mesh(unsigned mesh) const
{
	if (mesh > meshes_.size())
		return;

	meshes_[mesh]->draw();

}

model::Mesh* model::Model::get_mesh(unsigned mesh) const
{
	if (mesh > meshes_.size())
		return nullptr;

	return meshes_[mesh];
}

size_t model::Model::count() const
{
	return meshes_.size();
}

void model::Model::add_mesh(const Mesh& mesh)
{
	meshes_.push_back(new Mesh(mesh));
}

bool model::Model::colide(const Model& model)
{
	for (auto mesh : meshes_)
		for (auto sub_mesh : model.meshes_)
			if (mesh->bounding_box()->intersect(*sub_mesh->bounding_box()))
				return true;

	return false;
}

void model::Model::translate(const glm::vec3& axis)
{
	for (auto mesh : meshes_)
		mesh->translate(axis);
}

void model::Model::translate(const float& x, const float& y, const float& z)
{
	translate(glm::vec3(x, y, z));
}

void model::Model::rotate(const float& angle, glm::vec3 axis)
{
	for (auto mesh : meshes_)
		mesh->rotate(angle, axis);

}

void model::Model::rotate(const float& angle, const float& x, const float& y, const float& z)
{
	rotate(angle, glm::vec3(x, y, z));
}

void model::Model::scale(const glm::vec3& scale)
{
	for (auto mesh : meshes_)
		mesh->scale(scale);
}

void model::Model::scale(const float& x, const float& y, const float& z)
{
	scale(glm::vec3(x, y, z));
}

void model::Model::set_matrix(const glm::mat4& matrix)
{
	for (auto mesh : meshes_)
		mesh->set_model_matrix(matrix);
}

glm::mat4 model::Model::get_matrix() const
{
	glm::mat4 matrix(1.f);

	for (auto mesh : meshes_)
		matrix *= mesh->get_model_matrix();

	return matrix;
}

void model::Model::draw() const
{
	for (auto& mesh : meshes_)
		mesh->draw();
}
