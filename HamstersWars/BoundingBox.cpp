#include "BoundingBox.h"

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Defines.h"
#include <iostream>

model::BoundingBox::BoundingBox() : buffer_(nullptr)
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

model::BoundingBox::BoundingBox(const std::vector<glm::vec3>& verticles, const glm::mat4& model_matrix) : BoundingBox()
{
	calculate(verticles, model_matrix);
}

model::BoundingBox::BoundingBox(const BoundingBox& box) : BoundingBox()
{
	for (size_t i = 0; i < 8; i++)
		vertex_[i] = box.vertex_[i];

	vao_->bind();

	buffer_ = std::make_shared<gl::VertexBuffer>(vertex_, sizeof(glm::vec3) * 8, gl::buffer_usage::StaticDraw);
	vao_->bind_attribute(VERTEX_INDEX, *buffer_, gl::type::Float, 3, 0, NULL);
	glBindVertexArray(0);
}

model::BoundingBox& model::BoundingBox::operator=(const BoundingBox& box)
{
	for (size_t i = 0; i < 8; i++)
		vertex_[i] = box.vertex_[i];

	vao_->bind();

	buffer_ = std::make_shared<gl::VertexBuffer>(vertex_, sizeof(glm::vec3) * 8, gl::buffer_usage::StaticDraw);
	vao_->bind_attribute(VERTEX_INDEX, *buffer_, gl::type::Float, 3, 0, NULL);
	glBindVertexArray(0);

	return *this;
}


void model::BoundingBox::calculate(const std::vector<glm::vec3>& verticles, const glm::mat4& model_matrix)
{
	auto ver = model_matrix * glm::vec4(verticles[0], 1.f);

	min_ = glm::vec3(ver.x, ver.y, ver.z);
	max_ = glm::vec3(ver.x, ver.y, ver.z);

	for (auto vertex : verticles)
	{
		glm::vec4 ver = model_matrix * glm::vec4(vertex, 1.f);

		if (min_.x > ver.x)
			min_.x = ver.x;

		if (min_.y > ver.y)
			min_.y = ver.y;

		if (min_.z > ver.z)
			min_.z = ver.z;


		if (max_.x < ver.x)
			max_.x = ver.x;

		if (max_.y < ver.y)
			max_.y = ver.y;

		if (max_.z < ver.z)
			max_.z = ver.z;
	}

	auto size = size_ = max_ - min_;
	center_ = glm::vec3((max_.x + min_.x) / 2, (max_.y + min_.y) / 2, (max_.z + min_.z) / 2);
	size /= 2;

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

bool model::BoundingBox::intersect(const BoundingBox& box) const
{
	return max_.x > box.min_.x &&
		min_.x < box.max_.x &&
		max_.y > box.min_.y &&
		min_.y < box.max_.y &&
		max_.z > box.min_.z &&
		min_.z < box.max_.z;

}

bool model::BoundingBox::intersect(const glm::vec3& point) const
{
	return point.x > min_.x &&
		point.x < max_.x &&
		point.y > min_.y &&
		point.y < max_.y &&
		point.z > min_.z &&
		point.z < max_.z;
}

void model::BoundingBox::draw()
{
	vao_->bind();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *indexes_);
	
	glLineWidth(2);

	glDrawElements(GL_LINES, element_count_, GL_UNSIGNED_INT, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

glm::mat4 model::BoundingBox::get_model_matrix() const
{
	return glm::mat4(1.f);
}

glm::vec3 model::BoundingBox::size() const
{
	return size_;
}

glm::vec3 model::BoundingBox::center() const
{
	return center_;
}
