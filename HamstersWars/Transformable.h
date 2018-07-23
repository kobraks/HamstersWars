#pragma once
#include <glm/glm.hpp>
#include "Vector3D.h"
#include "Transform.h"

namespace game
{
	class Transformable
	{
	public:
		Transformable() = default;
		Transformable(const Transformable& transform) = default;
		Transformable(Transformable&& transform) = default;
		Transformable& operator=(const Transformable&) = default;
		Transformable& operator=(Transformable&&) = default;

		virtual ~Transformable() = default;

		void set_position(const float& x, const float& y, const float& z);
		void set_position(const gl::Vector3D& position);

		void set_rotation(const float& x, const float& y, const float& z);
		void set_rotation(const gl::Vector3D& rotation);

		void set_scale(const float& x, const float& y, const float& z);
		void set_scale(const gl::Vector3D& scale);

		void set_origin(const float& x, const float& y, const float& z);
		void set_origin(const gl::Vector3D& origin);

		gl::Vector3D get_position() const;
		gl::Vector3D get_rotation() const;
		gl::Vector3D get_scale() const;
		gl::Vector3D get_origin() const;

		void move(const float& offsetx, const float& offsety, const float& offsetz);
		void move(const gl::Vector3D& offset);

		void rotate(const float& x, const float& y, const float& z);
		void rotate(const gl::Vector3D& rotation);

		void scale(const gl::Vector3D& scale);
		void scale(const float& x, const float& y, const float& z);

		const Transform& get_transform() const;
		const Transform& get_inverse_transform() const;

	private:
		gl::Vector3D position_ = gl::Vector3D(0.f);
		gl::Vector3D scale_ = gl::Vector3D(1.f);
		gl::Vector3D rotate_ = gl::Vector3D(0);
		gl::Vector3D origin_ = gl::Vector3D(0);

		mutable Transform transform_;
		mutable Transform inverted_transform_;

		mutable bool transform_needs_update_ = true;
		mutable bool inverted_transform_needs_update_ = true;
	};
}

