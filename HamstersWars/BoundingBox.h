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
		BoundingBox(const std::vector<gl::Vector3D>& verticles, const Transform& transform);
		BoundingBox(const BoundingBox& box);

		BoundingBox& operator=(const BoundingBox& box);

		bool intersect(const BoundingBox& box) const;
		bool intersect(const gl::Vector3D& point) const;

		void draw();
		Transform get_model_matrix() const;

		gl::Vector3D size() const;
		gl::Vector3D center() const;

	private:
		gl::Vector3D vertex_[8];

		gl::Vector3D min_;
		gl::Vector3D max_;
		gl::Vector3D size_;
		gl::Vector3D center_;

		size_t element_count_;

		std::shared_ptr<gl::VertexBuffer> buffer_;
		std::shared_ptr<gl::VertexArray> vao_;
		std::shared_ptr<gl::VertexBuffer> indexes_;
		void calculate(const std::vector<gl::Vector3D>& verticles, const Transform& transform);
	};
}
