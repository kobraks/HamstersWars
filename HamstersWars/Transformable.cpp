#include "Transformable.h"
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

std::ostream& operator << (std::ostream& out, const glm::mat4& mat)
{
	for (size_t i = 0; i < 4; ++i)
	{
		for (size_t j = 0; j < 4; ++j)
		{
			out << mat[j][i] << " | ";
		}

		out << std::endl;
	}

	return out;
}

void game::Transformable::set_position(const float& x, const float& y, const float& z)
{
	position_ = glm::vec3(x, y, z);

	transform_needs_update_ = true;
	inverted_transform_needs_update_ = true;
}

void game::Transformable::set_position(const glm::vec3& position)
{
	set_position(position.x, position.y, position.z);
}

void game::Transformable::set_rotation(const float& x, const float& y, const float& z)
{
	rotate_ = glm::vec3(x, y, z);

	transform_needs_update_ = true;
	inverted_transform_needs_update_ = true;
}

void game::Transformable::set_rotation(const glm::vec3& rotation)
{
	set_rotation(rotation.x, rotation.y, rotation.z);
}

void game::Transformable::set_scale(const float& factor_x, const float& factor_y, const float& factor_z)
{
	scale_ = glm::vec3(factor_x, factor_y, factor_z);

	transform_needs_update_ = true;
	inverted_transform_needs_update_ = true;
}

void game::Transformable::set_scale(const glm::vec3& factor)
{
	set_scale(factor.x, factor.y, factor.z);
}

void game::Transformable::set_origin(const float& x, const float& y, const float& z)
{
	origin_ = glm::vec3(x, y, z);

	transform_needs_update_ = true;
	inverted_transform_needs_update_ = true;
}

void game::Transformable::set_origin(const glm::vec3& origin)
{
	set_origin(origin.x, origin.y, origin.z);
}

glm::vec3 game::Transformable::get_position() const
{
	return position_;
}

glm::vec3 game::Transformable::get_rotation() const
{
	return rotate_;
}

glm::vec3 game::Transformable::get_scale() const
{
	return scale_;
}

glm::vec3 game::Transformable::get_origin() const
{
	return origin_;
}

void game::Transformable::move(const float& offset_x, const float& offset_y, const float& offset_z)
{
	set_position(position_.x + offset_x, position_.y + offset_y, position_.z + offset_z);
}

void game::Transformable::move(const glm::vec3& offset)
{
	set_position(position_.x + offset.x, position_.y + offset.y, position_.z + offset.z);
}

void game::Transformable::rotate(const float& x, const float& y, const float& z)
{
	set_rotation(rotate_.x + x, rotate_.y + y, rotate_.z + z);
}

void game::Transformable::rotate(const glm::vec3& rotation)
{
	set_rotation(rotate_.x + rotation.x, rotate_.y + rotation.y, rotate_.z + rotation.z);
}

void game::Transformable::scale(const glm::vec3& factor)
{
	set_scale(scale_.x * factor.x, scale_.y * factor.y, scale_.z * factor.z);
}

void game::Transformable::scale(const float& factor_x, const float& factor_y, const float& factor_z)
{
	set_scale(scale_.x * factor_x, scale_.y * factor_y, scale_.z * factor_z);
}

const game::Transform& game::Transformable::get_transform() const
{
	if (transform_needs_update_)
	{
		auto transform_matrix = glm::translate(glm::mat4(1.f), position_);
		transform_matrix = glm::scale(transform_matrix, scale_);

		auto rotation_matrix = glm::mat4(1.f);
		rotation_matrix = glm::translate(rotation_matrix, origin_);
		rotation_matrix = glm::rotate(rotation_matrix, glm::radians(rotate_.x), glm::vec3(1, 0, 0));
		rotation_matrix = glm::rotate(rotation_matrix, glm::radians(rotate_.y), glm::vec3(0, 1, 0));
		rotation_matrix = glm::rotate(rotation_matrix, glm::radians(rotate_.z), glm::vec3(0, 0, 1));
		rotation_matrix = glm::translate(rotation_matrix, -origin_);

		transform_matrix *= rotation_matrix;

		transform_ = Transform(transform_matrix);
		transform_needs_update_ = false;
	}

	return transform_;
}

const game::Transform& game::Transformable::get_inverse_transform() const
{
	if (inverted_transform_needs_update_)
	{
		inverted_transform_ = get_transform().get_inverse();
		inverted_transform_needs_update_ = false;
	}

	return inverted_transform_;
}
