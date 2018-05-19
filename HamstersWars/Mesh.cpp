#include "Mesh.h"
#include "Log.h"

#include <glm/gtc/matrix_transform.hpp>

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Defines.h"

float color = 0;
#define COLOR_INC 0.1f


model::Mesh::Mesh(const aiMesh& mesh) : model_matrix_(glm::mat4(1.f))
{
	element_count_ = mesh.mNumFaces * 3;

	vertices_ = std::make_shared<std::vector<glm::vec3>>();

	vertices_->reserve(mesh.mNumVertices);
	auto colors = new glm::vec4[mesh.mNumVertices];
	auto texcoords = new glm::vec2[mesh.mNumVertices];
	auto normals = new glm::vec3[mesh.mNumVertices];
	auto indices = new GLuint[mesh.mNumFaces * 3];

	if (mesh.HasNormals())
	{
		Log::level() = Log::log_warning;
		Log::print("Model has not normal vectors");
	}

	for (size_t i = 0; i <  mesh.mNumVertices; ++i)
	{
		vertices_->push_back(glm::vec3(mesh.mVertices[i].x, mesh.mVertices[i].y, mesh.mVertices[i].z));

		if (mesh.HasVertexColors(0))
			colors[i] = glm::vec4(mesh.mColors[0][i].r, mesh.mColors[0][1].g, mesh.mColors[0][1].b, mesh.mColors[0][1].a);
		else
			colors[i] = glm::vec4(1, 1, 1, 1);

		if (mesh.HasTextureCoords(0))
			texcoords[i] = glm::vec2(mesh.mTextureCoords[0][i].x, mesh.mTextureCoords[0][i].y);

		if (mesh.HasNormals())
			normals[i] = glm::vec3(mesh.mNormals[0].x, mesh.mNormals[0].y, mesh.mNormals[0].z);
		else
			normals[i] = glm::vec3(0, 1, 0);
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

	vertex_buffer_ = new gl::VertexBuffer(vertices_->data(), sizeof(glm::vec3) * vertices_->size(), gl::buffer_usage::StaticDraw);
	color_buffer_ = new gl::VertexBuffer(colors, sizeof(glm::vec4) * mesh.mNumVertices, gl::buffer_usage::StaticDraw);
	texcoords_buffer_ = new gl::VertexBuffer(texcoords, sizeof(glm::vec2) * mesh.mNumVertices, gl::buffer_usage::StaticDraw);
	normals_buffer_ = new gl::VertexBuffer(normals, sizeof(glm::vec3) * mesh.mNumVertices, gl::buffer_usage::StaticDraw);
	
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

	box_ = new BoundingBox(*vertices_, model_matrix_);

	delete[] colors;
	delete[] texcoords;
	delete[] normals;
	delete[] indices;
}

model::Mesh::Mesh(const Mesh& mesh)
{
	vao_ = new gl::VertexArray(*mesh.vao_);

	vertex_buffer_ = new gl::VertexBuffer(*mesh.vertex_buffer_);
	color_buffer_ = new gl::VertexBuffer(*mesh.color_buffer_);
	texcoords_buffer_ = new gl::VertexBuffer(*mesh.texcoords_buffer_);
	normals_buffer_ = new gl::VertexBuffer(*mesh.texcoords_buffer_);
	index_buffer_ = new gl::VertexBuffer(*mesh.index_buffer_);

	element_count_ = mesh.element_count_;
	model_matrix_ = mesh.model_matrix_;

	vertices_ = mesh.vertices_;
	box_ = new BoundingBox(*mesh.box_);

	material_ = mesh.material_;
}

model::Mesh::~Mesh()
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

model::Mesh& model::Mesh::operator=(const Mesh& mesh)
{
	vao_ = new gl::VertexArray(*mesh.vao_);

	vertex_buffer_ = new gl::VertexBuffer(*mesh.vertex_buffer_);
	color_buffer_ = new gl::VertexBuffer(*mesh.color_buffer_);
	texcoords_buffer_ = new gl::VertexBuffer(*mesh.texcoords_buffer_);
	normals_buffer_ = new gl::VertexBuffer(*mesh.texcoords_buffer_);
	index_buffer_ = new gl::VertexBuffer(*mesh.index_buffer_);

	element_count_ = mesh.element_count_;
	model_matrix_ = mesh.model_matrix_;

	vertices_ = mesh.vertices_;
	box_ = new BoundingBox(*mesh.box_);

	material_ = mesh.material_;

	return *this;
}

void model::Mesh::set_material(const Material& material)
{
	material_ = material;
}

model::Material& model::Mesh::get_material()
{
	return material_;
}

void model::Mesh::draw() const
{
	vao_->bind();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *index_buffer_);
	if (material_.texture)
		material_.texture->bind();

	glDrawElements(GL_TRIANGLES, element_count_, GL_UNSIGNED_INT, NULL);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void model::Mesh::translate(const glm::vec3& axis)
{
	model_matrix_ *= glm::translate(glm::mat4(1.f), axis);

	box_->calculate(*vertices_, model_matrix_);
}

void model::Mesh::translate(const float& x, const float& y, const float& z)
{
	translate(glm::vec3(x, y, z));
}

void model::Mesh::rotate(const float& angle, glm::vec3 axis)
{
	model_matrix_ *= glm::rotate(glm::mat4(1.f), glm::radians(angle), axis);

	box_->calculate(*vertices_, model_matrix_);
}

void model::Mesh::rotate(const float& angle, const float& x, const float& y, const float& z)
{
	rotate(angle, glm::vec3(x, y, z));
}

void model::Mesh::scale(const glm::vec3& scale)
{
	model_matrix_ *= glm::scale(glm::mat4(1.f), scale);

	box_->calculate(*vertices_, model_matrix_);
}

void model::Mesh::scale(const float& x, const float& y, const float& z)
{
	scale(glm::vec3(x, y, z));
}

void model::Mesh::set_model_matrix(const glm::mat4& matrix)
{
	model_matrix_ = matrix;

	box_->calculate(*vertices_, model_matrix_);
}

glm::mat4 model::Mesh::get_model_matrix() const
{
	return model_matrix_;
}

model::BoundingBox* model::Mesh::bounding_box() const
{
	return box_;
}

glm::mat4x4 model::Mesh::convert(const aiMatrix4x4& matrix)
{
	glm::mat4x4 result(1.f);

	result[0] = glm::vec4(matrix.a1, matrix.b1, matrix.c1, matrix.d1);
	result[1] = glm::vec4(matrix.a2, matrix.b2, matrix.c2, matrix.d2);
	result[2] = glm::vec4(matrix.a3, matrix.b3, matrix.c3, matrix.d3);
	result[3] = glm::vec4(matrix.a4, matrix.b4, matrix.c4, matrix.d4);

	return result;
}
