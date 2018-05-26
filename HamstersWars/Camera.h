#pragma once
#include <glm/glm.hpp>

namespace gl
{
	class Camera
	{
	public:
		Camera();
		Camera(const float& width, const float& height);
		Camera(const float& width, const float& height, const glm::vec3& position, const glm::vec3& target, const glm::vec3& up, const float& fov);

		void set_height(const float& height);
		void set_width(const float& width);

		glm::mat4 get_projection() const;
		glm::mat4 get_view() const;

		glm::vec3 get_position() const;
		glm::vec3 get_up() const;
		glm::vec3 get_target() const;

		void set_fov(const float& fov);
		void set_up(const glm::vec3& up);

		void set_position(const glm::vec3& position);
		void set_target(const glm::vec3& target);

	private:
		glm::vec3 position_;
		glm::vec3 target_;
		glm::vec3 up_;

		glm::mat4 projection_;

		float height_;
		float width_;
		float fov_;
	};

}
