#include "stdafx.h"
#include "BoundingBox.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

inline void set_less_value(gl::Vector3D& value, const gl::Vector3D& right)
{
	if (value.x > right.x)
		value.x = right.x;

	if (value.y > right.y)
		value.y = right.y;

	if (value.z > right.z)
		value.z = right.z;
}

inline void set_greater_value(gl::Vector3D& value, const gl::Vector3D& right)
{
	if (value.x < right.x)
		value.x = right.x;

	if (value.y < right.y)
		value.y = right.y;

	if (value.z < right.z)
		value.z = right.z;
}

game::model::BoundingBox::BoundingBox() : buffer_(nullptr)
{
	GLuint indices[][2] = {
		{ 0, 1 },
		{ 0, 2 },
		{ 1, 3 },
		{ 2, 3 },
		{ 0, 3 },
		{ 1, 2 },

		{ 4, 5 },
		{ 4, 6 },
		{ 5, 7 },
		{ 6, 7 },
		{ 4, 7 },
		{ 5, 6 },

		{ 0, 4 },
		{ 1, 5 },
		{ 2, 6 },
		{ 3, 7 },

		{ 3, 6 },
		{ 7, 2 },
		{ 1, 4 },
		{ 5, 0 },

		{ 1, 7 },
		{ 3, 5 },

		{ 0, 6 },
		{ 4, 2 }
	};
	element_count_ = sizeof(indices) / sizeof(GLuint);

	vao_ = std::make_shared<gl::VertexArray>();
	vao_->bind();

	indexes_ = std::make_shared<gl::VertexBuffer>();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *indexes_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

game::model::BoundingBox::BoundingBox(const std::vector<gl::Vector3D>& verticles, const Transform& transform) : BoundingBox()
{
	calculate(verticles, transform);
}

game::model::BoundingBox::BoundingBox(const BoundingBox& box) : BoundingBox()
{
	for (size_t i = 0; i < 8; i++)
		vertex_[i] = box.vertex_[i];

	vao_->bind();

	buffer_ = std::make_shared<gl::VertexBuffer>(vertex_, sizeof(glm::vec3) * 8, gl::buffer_usage::StaticDraw);
	vao_->bind_attribute(VERTEX_INDEX, *buffer_, gl::type::Float, 3, 0, NULL);
	glBindVertexArray(0);
}

game::model::BoundingBox& game::model::BoundingBox::operator=(const BoundingBox& box)
{
	for (size_t i = 0; i < 8; i++)
		vertex_[i] = box.vertex_[i];

	vao_->bind();

	buffer_ = std::make_shared<gl::VertexBuffer>(vertex_, sizeof(glm::vec3) * 8, gl::buffer_usage::StaticDraw);
	vao_->bind_attribute(VERTEX_INDEX, *buffer_, gl::type::Float, 3, 0, NULL);
	glBindVertexArray(0);

	return *this;
}


void game::model::BoundingBox::calculate(const std::vector<gl::Vector3D>& verticles, const Transform& transform)
{
	min_ = max_ = transform.transfrom_point(verticles[0]);

	for (auto vertex : verticles)
	{
		auto transformed = transform.transfrom_point(vertex);

		set_less_value(min_, transformed);
		set_greater_value(max_, transformed);
	}

	auto size = size_ = max_ - min_;
	center_ = glm::vec3((max_.x + min_.x) / 2, (max_.y + min_.y) / 2, (max_.z + min_.z) / 2);
	
	size.x /= 2;
	size.y /= 2;
	size.z /= 2;
	//size /= 2;

	vertex_[0] = glm::vec3(center_.x - size.x, center_.y - size.y, center_.z + size.z);
	vertex_[1] = glm::vec3(center_.x - size.x, center_.y + size.y, center_.z + size.z);
	vertex_[2] = glm::vec3(center_.x + size.x, center_.y - size.y, center_.z + size.z);
	vertex_[3] = glm::vec3(center_.x + size.x, center_.y + size.y, center_.z + size.z);

	vertex_[4] = glm::vec3(center_.x - size.x, center_.y - size.y, center_.z - size.z);
	vertex_[5] = glm::vec3(center_.x - size.x, center_.y + size.y, center_.z - size.z);
	vertex_[6] = glm::vec3(center_.x + size.x, center_.y - size.y, center_.z - size.z);
	vertex_[7] = glm::vec3(center_.x + size.x, center_.y + size.y, center_.z - size.z);

	vao_->bind();
	buffer_ = std::make_shared<gl::VertexBuffer>(vertex_, sizeof(glm::vec3) * 8, gl::buffer_usage::StaticDraw);

	vao_->bind_attribute(VERTEX_INDEX, *buffer_, gl::type::Float, 3, 0, NULL);
	glBindVertexArray(0);
}

bool game::model::BoundingBox::intersect(const BoundingBox& box) const
{
	return max_.x > box.min_.x &&
		min_.x < box.max_.x &&
		max_.y > box.min_.y &&
		min_.y < box.max_.y &&
		max_.z > box.min_.z &&
		min_.z < box.max_.z;

}

bool game::model::BoundingBox::intersect(const gl::Vector3D& point) const
{
	return point.x > min_.x &&
		point.x < max_.x &&
		point.y > min_.y &&
		point.y < max_.y &&
		point.z > min_.z &&
		point.z < max_.z;
}

void game::model::BoundingBox::draw()
{
	vao_->bind();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *indexes_);
	
	glLineWidth(2);

	glDrawElements(GL_LINES, element_count_, GL_UNSIGNED_INT, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

game::Transform game::model::BoundingBox::get_model_matrix() const
{
	return Transform::get_indentity();
}

gl::Vector3D game::model::BoundingBox::size() const
{
	return size_;
}

gl::Vector3D game::model::BoundingBox::center() const
{
	return center_;
}
