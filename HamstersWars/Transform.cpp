#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>



game::Transform::Transform() : matrix_(glm::mat4(1.f))
{
}

game::Transform::Transform(const glm::mat4& matrix) : matrix_(matrix)
{
}

game::Transform& game::Transform::combine(const Transform& transform)
{
	matrix_ *= transform.matrix_;

	return *this;
}

game::Transform& game::Transform::rotate(const float& x, const float& y, const float& z)
{
	return rotate(glm::vec3(x, y, z));
}

game::Transform& game::Transform::rotate(const glm::vec3& rotate)
{
	matrix_ = glm::rotate(matrix_, rotate.x, glm::vec3(1, 0, 0));
	matrix_ = glm::rotate(matrix_, rotate.y, glm::vec3(0, 1, 0));
	matrix_ = glm::rotate(matrix_, rotate.z, glm::vec3(0, 0, 1));

	return *this;
}

game::Transform& game::Transform::scale(const float& x, const float& y, const float& z)
{
	return scale(glm::vec3(x, y, z));
}

game::Transform& game::Transform::scale(const glm::vec3& scale)
{
	matrix_ = glm::scale(matrix_, scale);

	return *this;
}

game::Transform& game::Transform::translate(const float& x, const float& y, const float& z)
{
	return translate(glm::vec3(x, y, z));
}

game::Transform& game::Transform::translate(const glm::vec3& position)
{
	matrix_ = glm::translate(matrix_, position);

	return *this;
}

glm::vec3 game::Transform::transfrom_point(const float& x, const float& y, const float& z) const
{
	return transfrom_point(glm::vec3(x, y, z));
}

glm::vec3 game::Transform::transfrom_point(const glm::vec3& point) const
{
	glm::vec4 tmp(point, 1.f);
	tmp = matrix_ * tmp;

	return glm::vec3(tmp.x, tmp.y, tmp.z);
}

game::Transform game::Transform::get_inverse() const
{
	return Transform(glm::inverse(matrix_));
}

glm::mat4 game::Transform::get_matrix() const
{
	return matrix_;
}

game::Transform game::Transform::get_indentity()
{
	return Transform(glm::mat4(1.f));
}

game::Transform game::operator*(const Transform& left, const Transform& right)
{
	return Transform(left).combine(right);
}

game::Transform& game::operator*=(Transform& left, const Transform& right)
{
	return left.combine(right);
}

glm::vec3 game::operator*(const Transform& left, const glm::vec3& right)
{
	return left.transfrom_point(right);
}

bool game::operator==(const Transform& left, const Transform& right)
{
	return left.matrix_ == right.matrix_;
}

bool game::operator!=(const Transform& left, const Transform& right)
{
	return left.matrix_ != right.matrix_;
}
