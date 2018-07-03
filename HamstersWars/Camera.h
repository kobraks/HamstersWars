#pragma once
#include <glm/glm.hpp>
#include "Vector3D.h"

namespace gl
{
	class Camera
	{
	public:
		Camera();
		Camera(const float& width, const float& height);
		Camera(const float& width, const float& height, const Vector3D& position, const Vector3D& target, const Vector3D& up, const float& fov);

		void set_height(const float& height);
		void set_width(const float& width);

		glm::mat4 get_projection() const;
		glm::mat4 get_view() const;

		Vector3D get_position() const;
		Vector3D get_up() const;
		Vector3D get_target() const;

		void set_fov(const float& fov);
		void set_up(const Vector3D& up);

		void set_position(const Vector3D& position);
		void set_target(const Vector3D& target);

		void move(const Vector3D& offset);
		void rotate(const Vector3D& axis);
	private:
		Vector3D position_;
		Vector3D target_;
		Vector3D up_;
		Vector3D world_up_;

		mutable bool view_needs_update_ = true;
		mutable glm::mat4 view_;

		mutable bool projection_needs_update_ = true;
		mutable glm::mat4 projection_;

		float height_;
		float width_;
		float fov_;
	};

}
