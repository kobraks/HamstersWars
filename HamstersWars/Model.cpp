#include "Model.h"

#include "Mesh.h"

model::Model::Model(std::vector<Mesh*> meshes) : meshes_(meshes)
{

}

model::Model::~Model()
{
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

size_t model::Model::size() const
{
	return meshes_.size();
}

void model::Model::draw() const
{
	for (auto& mesh : meshes_)
		mesh->draw();
}
