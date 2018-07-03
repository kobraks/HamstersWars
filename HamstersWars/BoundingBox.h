#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <gl/glew.h>
#include "Transform.h"

namespace gl
{
	class VertexBuffer;
	class VertexArray;
}

namespace game::model
{
	class BoundingBox
	{
	public:
		BoundingBox();
		BoundingBox(const std::vector<glm::vec3>& verticles, const Transform& transform);
		BoundingBox(const BoundingBox& box);

		BoundingBox& operator=(const BoundingBox& box);

		bool intersect(const BoundingBox& box) const;
		bool intersect(const glm::vec3& point) const;

		void draw();
		glm::mat4 get_model_matrix() const;

		glm::vec3 size() const;
		glm::vec3 center() const;

	private:
		glm::vec3 vertex_[8];

		glm::vec3 min_;
		glm::vec3 max_;
		glm::vec3 size_;
		glm::vec3 center_;

		size_t element_count_;

		std::shared_ptr<gl::VertexBuffer> buffer_;
		std::shared_ptr<gl::VertexArray> vao_;
		std::shared_ptr<gl::VertexBuffer> indexes_;
		void calculate(const std::vector<glm::vec3>& verticles, const Transform& transform);
	};
}
