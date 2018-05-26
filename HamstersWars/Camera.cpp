#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>


gl::Camera::Camera() : height_(0), width_(0), fov_(0)
{
}

gl::Camera::Camera(const float& width, const float& height) : height_(height), width_(width), fov_(0)
{
}

gl::Camera::Camera(const float& width, const float& height, const glm::vec3& position, const glm::vec3& target,
	const glm::vec3& up, const float& fov) : height_(height), width_(width), fov_(fov), position_(position),
	target_(target), up_(up)
{
	projection_ = glm::perspective(glm::radians(fov), width / height, .01f, 100.f);
}

void gl::Camera::set_height(const float& height)
{
	height_ = height;

	if (height_ != 0)
		projection_ = glm::perspective(glm::radians(fov_), width_ / height_, .01f, 100.f);
}

void gl::Camera::set_width(const float& width)
{
	width_ = width;

	if (height_ != 0)
		projection_ = glm::perspective(glm::radians(fov_), width_ / height_, .01f, 100.f);
}

glm::mat4 gl::Camera::get_projection() const
{
	return projection_;
}

glm::mat4 gl::Camera::get_view() const
{
	return glm::lookAt(position_, target_, up_);
}

glm::vec3 gl::Camera::get_position() const
{
	return position_;
}

glm::vec3 gl::Camera::get_up() const
{
	return up_;
}

glm::vec3 gl::Camera::get_target() const
{
	return target_;
}

void gl::Camera::set_fov(const float& fov)
{
	fov_ = fov;

	projection_ = glm::perspective(glm::radians(fov_), width_ / height_, .01f, 100.f);
}

void gl::Camera::set_up(const glm::vec3& up)
{
	up_ = up;
}

void gl::Camera::set_position(const glm::vec3& position)
{
	position_ = position;
}

void gl::Camera::set_target(const glm::vec3& target)
{
	target_ = target;
}
