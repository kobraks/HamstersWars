#pragma once
#include <glm/glm.hpp>

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

		Transform& combine(const Transform& transform);
		
		Transform& rotate(const float& x, const float& y, const float& z);
		Transform& rotate(const glm::vec3& rotate);

		Transform& scale(const float& x, const float& y, const float& z);
		Transform& scale(const glm::vec3& scale);

		Transform& translate(const float& x, const float& y, const float& z);
		Transform& translate(const glm::vec3& position);

		glm::vec3 transfrom_point(const float& x, const float& y, const float& z) const;
		glm::vec3 transfrom_point(const glm::vec3& point) const;

		Transform get_inverse() const;

		glm::mat4 get_matrix() const;
	
		friend Transform operator*(const Transform& left, const Transform& right);
		friend Transform& operator*= (Transform& left, const Transform& right);
		friend glm::vec3 operator*(const Transform& left, const glm::vec3& right);

		friend bool operator==(const Transform& left, const Transform& right);
		friend bool operator!=(const Transform& left, const Transform& right);

		static Transform get_indentity();
	private:
		glm::mat4 matrix_;
	};
}

