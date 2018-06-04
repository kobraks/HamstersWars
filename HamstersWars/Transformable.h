#pragma once
#include <glm/glm.hpp>
#include "Transform.h"

namespace game
{
	class Transformable
	{
	public:
		Transformable();

		virtual ~Transformable() = default;

		void set_position(const float& x, const float& y, const float& z);
		void set_position(const glm::vec3& position);

		void set_rotation(const float& x, const float& y, const float& z);
		void set_rotation(const glm::vec3& rotation);

		void set_scale(const float& x, const float& y, const float& z);
		void set_scale(const glm::vec3& scale);

		void set_origin(const float& x, const float& y, const float& z);
		void set_origin(const glm::vec3& origin);

		glm::vec3 get_position() const;
		glm::vec3 get_rotation() const;
		glm::vec3 get_scale() const;
		glm::vec3 get_origin() const;

		void move(const float& offsetx, const float& offsety, const float& offsetz);
		void move(const glm::vec3& offset);

		void rotate(const float& x, const float& y, const float& z);
		void rotate(const glm::vec3& rotation);

		void scale(const glm::vec3& scale);
		void scale(const float& x, const float& y, const float& z);

		const Transform& get_transform() const;
		const Transform& get_inverse_transform() const;

	private:
		glm::vec3 position_ = glm::vec3(0);
		glm::vec3 scale_ = glm::vec3(1.f);
		glm::vec3 rotate_ = glm::vec3(0);
		glm::vec3 origin_ = glm::vec3(0);

		mutable Transform transform_;
		mutable Transform inverted_transform_;

		mutable bool transform_needs_update_ = true;
		mutable bool inverted_transform_needs_update_ = true;
	};
}

