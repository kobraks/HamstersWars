#include "stdafx.h"
#include "Model.h"
#include <utility>
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.h"


inline void set_greater_value(gl::Vector3D& value, const gl::Vector3D& right)
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

game::model::Model::ModelIterator::ModelIterator(table_type::iterator iterator) : current_(iterator)
{
}

game::model::Model::ModelIterator::self_type game::model::Model::ModelIterator::operator++()
{
	++current_;
	return *this;
}

game::model::Model::ModelIterator::self_type game::model::Model::ModelIterator::operator++(int i)
{
	current_ += i;
	return *this;
}

game::model::Model::ModelIterator::self_type game::model::Model::ModelIterator::operator--()
{
	--current_;
	return *this;
}

game::model::Model::ModelIterator::self_type game::model::Model::ModelIterator::operator--(int i)
{
	current_ -= i;
	return *this;
}

game::model::Model::ModelIterator::reference game::model::Model::ModelIterator::operator*() const
{
	return *(*current_);
}

game::model::Model::ModelIterator::pointer game::model::Model::ModelIterator::operator->() const
{
	return &*(*current_);
}

bool game::model::Model::ModelIterator::operator==(const self_type& right) const
{
	return current_ == right.current_;
}

bool game::model::Model::ModelIterator::operator!=(const self_type& right) const
{
	return current_ != right.current_;
}

game::model::Model::ConstModelIterator::ConstModelIterator(table_type::const_iterator iterator) : current_(iterator)
{
}

game::model::Model::ConstModelIterator::self_type game::model::Model::ConstModelIterator::operator++()
{
	++current_;
	return *this;
}

game::model::Model::ConstModelIterator::self_type game::model::Model::ConstModelIterator::operator++(int i)
{
	current_ += i;
	return *this;
}

game::model::Model::ConstModelIterator::self_type game::model::Model::ConstModelIterator::operator--()
{
	--current_;
	return *this;
}

game::model::Model::ConstModelIterator::self_type game::model::Model::ConstModelIterator::operator--(int i)
{
	current_ -= i;
	return *this;
}

game::model::Model::ConstModelIterator::reference game::model::Model::ConstModelIterator::operator*() const
{
	return *(*current_);
}

const game::model::Model::ConstModelIterator::pointer game::model::Model::ConstModelIterator::operator->() const
{
	return &*(*current_);
}

bool game::model::Model::ConstModelIterator::operator==(const self_type& right) const
{
	return current_ == right.current_;
}

bool game::model::Model::ConstModelIterator::operator!=(const self_type& right) const
{
	return current_ != right.current_;
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

game::model::Model::ModelIterator game::model::Model::begin() noexcept
{
	return ModelIterator(meshes_.begin());
}

game::model::Model::ModelIterator game::model::Model::end() noexcept
{
	return ModelIterator(meshes_.end());
}

game::model::Model::ConstModelIterator game::model::Model::begin() const noexcept
{
	return ConstModelIterator(meshes_.begin());
}

game::model::Model::ConstModelIterator game::model::Model::end() const noexcept
{
	return ConstModelIterator(meshes_.end());
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

gl::Vector3D game::model::Model::size() const
{
	if (size_needs_update_)
	{
		glm::mat4 scale_matrix = glm::scale(glm::mat4(1.f), static_cast<glm::vec3>(get_scale()));
		glm::vec4 size = glm::vec4(static_cast<glm::vec3>(size_), 1.f);

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
