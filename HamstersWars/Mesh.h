#pragma once

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>
#include <memory>

#include "Material.h"
#include "Texture2d.h"

#include "BoundingBox.h"

namespace gl
{
	class VertexBuffer;
	class VertexArray;
}

namespace model
{
	class BoundingBox;

	class Mesh
	{
	public:
		explicit Mesh(const aiMesh& mesh);
		Mesh(const Mesh& mesh);
		~Mesh();

		Mesh& operator= (const Mesh& mesh);

		void set_material(const Material& material);
		Material& get_material();

		void draw() const;

		void translate(const glm::vec3& axis);
		void translate(const float& x, const float& y, const float& z);
		void rotate(const float& angle, glm::vec3 axis);
		void rotate(const float& angle, const float& x, const float& y, const float& z);
		void scale(const glm::vec3& scale);
		void scale(const float& x, const float& y, const float& z);

		void set_model_matrix(const glm::mat4& matrix);
		glm::mat4 get_model_matrix() const;

		BoundingBox* bounding_box() const;
	private:
		gl::VertexBuffer* vertex_buffer_;
		gl::VertexBuffer* color_buffer_;
		gl::VertexBuffer* normals_buffer_;
		gl::VertexBuffer* texcoords_buffer_;
		gl::VertexBuffer* index_buffer_;

		gl::VertexArray* vao_;

		BoundingBox* box_;

		Material material_;
		std::shared_ptr<std::vector<glm::vec3>> vertices_;
		size_t element_count_;
		glm::mat4 model_matrix_;
		

		static glm::mat4x4 convert(const aiMatrix4x4& matrix);
	};

}
