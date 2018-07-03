#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>


gl::Camera::Camera() : height_(0), width_(0), fov_(0)
{
}

gl::Camera::Camera(const float& width, const float& height) : height_(height), width_(width), fov_(0)
{
}

gl::Camera::Camera(const float& width, const float& height, const Vector3D& position, const Vector3D& target,
	const Vector3D& up, const float& fov) : height_(height), width_(width), fov_(fov), position_(position),
	target_(target), up_(up), world_up_(up)
{
	//projection_ = glm::perspective(glm::radians(fov), width / height, .01f, 100.f);
}

void gl::Camera::set_height(const float& height)
{
	height_ = height;
}

void gl::Camera::set_width(const float& width)
{
	width_ = width;
}

glm::mat4 gl::Camera::get_projection() const
{
	if (projection_needs_update_)
	{
		projection_needs_update_ = false;
		projection_ = glm::perspective(glm::radians(fov_), 4.0f / 3.0f, .1f, 100.f);
	}
	return projection_;
}

glm::mat4 gl::Camera::get_view() const
{
	if (view_needs_update_)
	{
		view_needs_update_ = false;

		view_ = glm::lookAt(glm::vec3(position_), glm::vec3(target_), glm::vec3(up_));
	}

	return view_;
}

gl::Vector3D gl::Camera::get_position() const
{
	return position_;
}

gl::Vector3D gl::Camera::get_up() const
{
	return up_;
}

gl::Vector3D gl::Camera::get_target() const
{
	return target_;
}

void gl::Camera::set_fov(const float& fov)
{
	fov_ = fov;

	projection_needs_update_ = true;
}

void gl::Camera::set_up(const Vector3D& up)
{
	up_ = up;

	view_needs_update_ = true;
}

void gl::Camera::set_position(const Vector3D& position)
{
	position_ = position;

	view_needs_update_ = true;
}

void gl::Camera::set_target(const Vector3D& target)
{
	target_ = target;

	view_needs_update_ = true;
}

void gl::Camera::move(const Vector3D& offset)
{
	position_ += offset;

	view_needs_update_ = true;
}

void gl::Camera::rotate(const Vector3D& axis)
{
	view_needs_update_ = true;

	/*
	glm::mat4 mat_roll = glm::mat4(1.f);
	glm::mat4 mat_pitch = glm::mat4(1.f);
	glm::mat4 mat_yaw = glm::mat4(1.f);

	mat_roll = glm::rotate(mat_roll, glm::radians(axis.z), glm::vec3(0, 0, 1.f));
	mat_pitch = glm::rotate(mat_pitch, glm::radians(axis.x), glm::vec3(1.f, 0, 0));
	mat_yaw = glm::rotate(mat_yaw, glm::radians(axis.y), glm::vec3(0, 1.f, 0));

	glm::mat4 rotate = mat_roll * mat_pitch * mat_yaw;
	*/
}
