#pragma once
#include <glm/glm.hpp>
#include "Vector3D.h"

namespace game
{
	/**
	 * \brief Its a proxy for glm::mat4 and its transformations
	 */
	class Transform
	{
	public:
		Transform();
		explicit Transform(const glm::mat4& matrix);
		
		Transform(const Transform&) = default;
		Transform(Transform&&) = default;

		Transform& operator=(const Transform&) = default;
		Transform& operator=(Transform&&) = default;

		operator glm::mat4() const;

		Transform& combine(const Transform& transform);
		
		Transform& rotate(const float& x, const float& y, const float& z);
		Transform& rotate(const gl::Vector3D& rotate);

		Transform& scale(const float& x, const float& y, const float& z);
		Transform& scale(const gl::Vector3D& scale);

		Transform& translate(const float& x, const float& y, const float& z);
		Transform& translate(const gl::Vector3D& position);

		glm::vec3 transfrom_point(const float& x, const float& y, const float& z) const;
		glm::vec3 transfrom_point(const gl::Vector3D& point) const;

		Transform get_inverse() const;

		glm::mat4 get_matrix() const;
	
		friend Transform operator*(const Transform& left, const Transform& right);
		friend Transform& operator*= (Transform& left, const Transform& right);
		friend glm::vec3 operator*(const Transform& left, const gl::Vector3D& right);

		friend bool operator==(const Transform& left, const Transform& right);
		friend bool operator!=(const Transform& left, const Transform& right);

		static Transform get_indentity();
	private:
		glm::mat4 matrix_;
	};
}

