#pragma once

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include <GL/glew.h>
#include <vector>
#include <memory>

#include "Material.h"
#include "BoundingBox.h"
#include "Transformable.h"
#include "Vector3D.h"
#include "Vector2D.h"

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
		typedef gl::Vector3D verticle_type;
		typedef gl::Vector3D normal_type;
		typedef gl::Vector2D texcoords_type;
		typedef std::vector<verticle_type> verticles_table_type;

		explicit Mesh(const aiMesh& mesh);
		Mesh(const Mesh& mesh);
		~Mesh();

		Mesh& operator= (const Mesh& mesh);

		void set_material(const Material& material);
		Material& get_material();
		Material get_material() const;

		void draw() const;

		const BoundingBox& get_bounding_box() const;
		const std::vector<gl::Vector3D>& get_verticles() const;

		gl::Vector3D size() const;
	private:
		gl::VertexBuffer* vertex_buffer_;
		gl::VertexBuffer* color_buffer_;
		gl::VertexBuffer* normals_buffer_;
		gl::VertexBuffer* texcoords_buffer_;
		gl::VertexBuffer* index_buffer_;

		gl::VertexArray* vao_;
		Material material_;
		std::shared_ptr<verticles_table_type> vertices_;
		size_t element_count_;

		mutable Transform old_transfrom_;
		mutable BoundingBox box_;

		gl::Vector3D size_;
	};

}
