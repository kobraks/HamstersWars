#pragma once

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>
#include <memory>

#include "Material.h"
#include "BoundingBox.h"
#include "Transformable.h"

namespace gl
{
	class VertexBuffer;
	class VertexArray;
}

namespace game::model
{
	class Mesh : public Transformable
	{
	public:
		explicit Mesh(const aiMesh& mesh);
		Mesh(const Mesh& mesh);
		~Mesh();

		Mesh& operator= (const Mesh& mesh);

		void set_material(const Material& material);
		Material& get_material();

		void draw() const;

		const BoundingBox& get_bounding_box() const;
		const std::vector<glm::vec3>& get_verticles() const;

		glm::vec3 size() const;
	private:
		gl::VertexBuffer* vertex_buffer_;
		gl::VertexBuffer* color_buffer_;
		gl::VertexBuffer* normals_buffer_;
		gl::VertexBuffer* texcoords_buffer_;
		gl::VertexBuffer* index_buffer_;

		gl::VertexArray* vao_;
		Material material_;
		std::shared_ptr<std::vector<glm::vec3>> vertices_;
		size_t element_count_;

		mutable Transform old_transfrom_;
		mutable BoundingBox box_;

		glm::vec3 size_;
	};

}
