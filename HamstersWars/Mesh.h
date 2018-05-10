#pragma once

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "Texture2d.h"
#include "Material.h"
#include "glm/glm.hpp"

#include <GL/glew.h>
#include <GL/freeglut.h>

namespace gl
{
	class VertexBuffer;
	class VertexArray;
}

namespace model
{
	class Mesh
	{
	public:
		explicit Mesh(const aiMesh& mesh);
		Mesh(const Mesh& mesh);
		~Mesh();

		void set_material(Material material);
		Material& get_material();

		void draw() const;

		void translate(const glm::vec3& axis);
		void translate(const float& x, const float& y, const float& z);
		void rotate(const float& angle, glm::vec3 axis);
		void rotate(const float& angle, const float& x, const float& y, const float& z);
		void scale(const glm::vec3& scale);
		void scale(const float& x, const float& y, const float& z);
	private:
		gl::VertexBuffer* vertex_buffer_;
		gl::VertexBuffer* color_buffer_;
		gl::VertexBuffer* normals_buffer_;
		gl::VertexBuffer* texcoords_buffer_;
		gl::VertexBuffer* index_buffer_;

		gl::VertexArray* vao_;

		float rotate_angle_;
		glm::vec3 rotate_vec_;
		glm::vec3 translate_vec_;
		glm::vec3 scale_vec_;


		Material material_;
		GLfloat* vertices_;
		size_t vertex_count_;
		size_t element_count_;
		GLuint indler_;

		static glm::mat4x4 convert(const aiMatrix4x4& matrix);
	};

}
