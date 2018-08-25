#include "Mesh.h"
#include "Log.h"

#include <glm/gtc/matrix_transform.hpp>

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Defines.h"
#include "Utils.h"

float color = 0;
#define COLOR_INC 0.1f

#pragma region utils
/**
 * \brief converts assimp vector to glm vector
 * \param vector a 2D assimp vector
 * \return glm::vec2 which is converted vector from assimp
 */
inline glm::vec2 convert(const aiVector2D& vector)
{
	return glm::vec2(vector.x, vector.y);
}

/**
 * \brief converts assimp vector to glm vector
 * \param vector a 3D assimp vector
 * \return glm::vec3 which is converted vector from assimp
 */
inline glm::vec3 convert(const aiVector3D& vector)
{
	return glm::vec3(vector.x, vector.y, vector.z);
}

/**
 * \brief converts assimp color to glm
 * \param color color from assimp
 * \return glm::vec4 which is converted color from assimp
 */
inline glm::vec4 convert(const aiColor4D& color)
{
	return glm::vec4(color.r, color.g, color.b, color.a);
}
#pragma endregion

game::model::Mesh::Mesh(const aiMesh& mesh)
{
	LOG(LOG_DEBUG1, "Vertex count %u", mesh.mNumVertices);
	LOG(LOG_DEBUG1, "Faces count %u", mesh.mNumFaces * 3);

	element_count_ = mesh.mNumFaces * 3;

	vertices_ = std::make_shared<verticles_table_type>();

	vertices_->reserve(mesh.mNumVertices);
	auto colors = new gl::Color[mesh.mNumVertices];
	auto texcoords = new texcoords_type[mesh.mNumVertices];
	auto normals = new normal_type[mesh.mNumVertices];
	auto indices = new GLuint[mesh.mNumFaces * 3];

	gl::Vector3D min, max;

	min = max = convert(mesh.mVertices[0]);

	if (mesh.HasNormals())
	{
		LOG(LOG_WARNING, "Mesh has not normal vectors")
	}

	for (size_t i = 0; i <  mesh.mNumVertices; ++i)
	{
		auto vertex = convert(mesh.mVertices[i]);

		min = utils::get_lesser_values(min, vertex);
		max = utils::get_greater_values(max, vertex);

		vertices_->push_back(vertex);

		if (mesh.HasVertexColors(0))
			colors[i] = convert(mesh.mColors[0][i]);
		else
			colors[i] = glm::vec4(1.f);

		if (mesh.HasTextureCoords(0))
			texcoords[i] = glm::vec2(mesh.mTextureCoords[0][i].x, mesh.mTextureCoords[0][i].y);

		if (mesh.HasNormals())
			normals[i] = convert(mesh.mNormals[i]);
		else
			normals[i] = glm::vec3(0.f, 1.f, 0.f);
	}

	if (mesh.HasFaces())
		for (size_t i = 0; i < mesh.mNumFaces; ++i)
		{
			auto& face = mesh.mFaces[i];

			for (size_t j = 0; j < 3; ++j)
				indices[i * 3 + j] = face.mIndices[j];
		}


	vao_ = new gl::VertexArray();
	vao_->bind();

	vertex_buffer_ = new gl::VertexBuffer(vertices_->data(), sizeof(verticle_type) * vertices_->size(), gl::buffer_usage::StaticDraw);
	color_buffer_ = new gl::VertexBuffer(colors, sizeof(gl::Color) * mesh.mNumVertices, gl::buffer_usage::StaticDraw);
	texcoords_buffer_ = new gl::VertexBuffer(texcoords, sizeof(texcoords_type) * mesh.mNumVertices, gl::buffer_usage::StaticDraw);
	normals_buffer_ = new gl::VertexBuffer(normals, sizeof(normal_type) * mesh.mNumVertices, gl::buffer_usage::StaticDraw);
	
	index_buffer_ = new gl::VertexBuffer();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *index_buffer_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh.mNumFaces * 3, indices, GL_STATIC_DRAW);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(4);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	vao_->bind_attribute(VERTEX_INDEX, *vertex_buffer_, gl::type::Float, 3, 0, 0);
	vao_->bind_attribute(COLOR_INDEX, *color_buffer_, gl::type::Float, 4, 0, 0);
	vao_->bind_attribute(TEXCOORD_INDEX, *texcoords_buffer_, gl::type::Float, 2, 0, 0);
	vao_->bind_attribute(NORMAL_INDEX, *normals_buffer_, gl::type::Float, 3, 0, 0);

	glBindVertexArray(0);

	delete[] colors;
	delete[] texcoords;
	delete[] normals;
	delete[] indices;

	gl::Vector3D origin = glm::vec3((min.x + max.x) / 2, (min.y + max.y) / 2, (min.z + max.z) / 2);

	LOG(LOG_DEBUG1, "Origin point: x=%f, y=%f, z=%f", origin.x, origin.y, origin.z);
	// glm::vec3 center = glm::vec3((min_x + max_x) / 2, (min_y + max_y) / 2, (min_z + max_z) / 2);
	set_origin(origin);
	size_ = max - min;
}

game::model::Mesh::Mesh(const Mesh& mesh)
	: Transformable(mesh)
{
	vao_ = new gl::VertexArray(*mesh.vao_);

	vertex_buffer_ = new gl::VertexBuffer(*mesh.vertex_buffer_);
	color_buffer_ = new gl::VertexBuffer(*mesh.color_buffer_);
	texcoords_buffer_ = new gl::VertexBuffer(*mesh.texcoords_buffer_);
	normals_buffer_ = new gl::VertexBuffer(*mesh.texcoords_buffer_);
	index_buffer_ = new gl::VertexBuffer(*mesh.index_buffer_);

	element_count_ = mesh.element_count_;

	vertices_ = mesh.vertices_;

	material_ = mesh.material_;
}

game::model::Mesh::~Mesh()
{
	vao_->bind();
	delete vertex_buffer_;
	delete color_buffer_;
	delete texcoords_buffer_;
	delete normals_buffer_;
	delete index_buffer_;
	glBindVertexArray(0);

	delete vao_;
}

game::model::Mesh& game::model::Mesh::operator=(const Mesh& mesh)
{
	vao_ = new gl::VertexArray(*mesh.vao_);

	vertex_buffer_ = new gl::VertexBuffer(*mesh.vertex_buffer_);
	color_buffer_ = new gl::VertexBuffer(*mesh.color_buffer_);
	texcoords_buffer_ = new gl::VertexBuffer(*mesh.texcoords_buffer_);
	normals_buffer_ = new gl::VertexBuffer(*mesh.texcoords_buffer_);
	index_buffer_ = new gl::VertexBuffer(*mesh.index_buffer_);

	element_count_ = mesh.element_count_;

	vertices_ = mesh.vertices_;

	material_ = mesh.material_;

	return *this;
}

void game::model::Mesh::set_material(const Material& material)
{
	material_ = material;
}

game::model::Material& game::model::Mesh::get_material()
{
	return material_;
}

game::model::Material game::model::Mesh::get_material() const
{
	return material_;
}

void game::model::Mesh::draw() const
{
	vao_->bind();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *index_buffer_);
	glBindTexture(GL_TEXTURE_2D, *material_.texture);

	glDrawElements(GL_TRIANGLES, element_count_, GL_UNSIGNED_INT, NULL);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

const game::model::BoundingBox& game::model::Mesh::get_bounding_box() const
{
	if (old_transfrom_ != get_transform())
	{
		box_ = BoundingBox(*vertices_, get_transform());
		old_transfrom_= get_transform();
	}

	return box_;
}

const std::vector<gl::Vector3D>& game::model::Mesh::get_verticles() const
{
	return *vertices_;
}

gl::Vector3D game::model::Mesh::size() const
{
	return size_;
}
