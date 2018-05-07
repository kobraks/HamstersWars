#include "Mesh.h"
#include "Log.h"

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Defines.h"

model::Mesh::Mesh(const aiMesh& mesh) : rotate_angle_(0), scale_vec_(1.f, 1.f, 1.f)
{
	vertex_count_ = mesh.mNumVertices;
	vertices_ = new GLfloat[vertex_count_ * 3];

	auto test = new GLfloat[vertex_count_ * 9];

	inditer_ = 0;

	auto indices = new GLint[vertex_count_];
	auto colors = new GLfloat[vertex_count_ * 4];
	auto normals = new GLfloat[vertex_count_ * 3];
	auto texcoords = new GLfloat[vertex_count_ * 3];

	if (!mesh.HasNormals())
	{
		Log::level() = Log::log_warning;
		Log::print("mesh does not contain normal vectors");
	}

	for (unsigned i = 0; i < vertex_count_; ++i)
	{
		vertices_[i * 3] = mesh.mVertices[i].x;
		vertices_[i * 3 + 1] = mesh.mVertices[i].y;
		vertices_[i * 3 + 2] = mesh.mVertices[i].z;

		test[i * 9] = mesh.mVertices[i].x;
		test[i * 9 + 1] = mesh.mVertices[i].y;
		test[i * 9 + 2] = mesh.mVertices[i].z;

		if (i < mesh.mNumFaces)
			for (unsigned j = 0; j < mesh.mFaces->mNumIndices; ++j)
				indices[inditer_++] = mesh.mFaces[i].mIndices[j];

		if (mesh.HasVertexColors(i))
		{
			colors[i * 4] = mesh.mColors[i]->r;
			colors[i * 4 + 1] = mesh.mColors[i]->g;
			colors[i * 4 + 2] = mesh.mColors[i]->b;
			colors[i * 4 + 3] = mesh.mColors[i]->a;

			test[i * 9 + 3] = mesh.mColors[i]->r;
			test[i * 9 + 4] = mesh.mColors[i]->g;
			test[i * 9 + 5] = mesh.mColors[i]->b;
			test[i * 9 + 6] = mesh.mColors[i]->a;
		}
		else
		{
			colors[i * 4] = 1.f;
			colors[i * 4 + 1] = 1.f;
			colors[i * 4 + 2] = 1.f;
			colors[i * 4 + 3] = 1.f;

			test[i * 9 + 3] = 1.f;
			test[i * 9 + 4] = 1.f;
			test[i * 9 + 5] = 1.f;
			test[i * 9 + 6] = 1.f;
		}

		if (mesh.HasTextureCoords(0))
		{
			texcoords[i * 2] = mesh.mTextureCoords[0][i].x;
			texcoords[i * 2 + 1] = mesh.mTextureCoords[0][i].y;

			test[i * 9 + 7] = mesh.mTextureCoords[0][i].x;
			test[i * 9 + 8] = mesh.mTextureCoords[0][i].y;
		}
		else
		{
			texcoords[i * 2] = 0;
			texcoords[i * 2 + 1] = 0;

			test[i * 9 + 7] = 0;
			test[i * 9 + 8] = 0;
		}

		if (mesh.HasNormals())
		{
			normals[i * 3] = mesh.mNormals[i].x;
			normals[i * 3 + 1] = mesh.mNormals[i].y;
			normals[i * 3 + 2] = mesh.mNormals[i].z;
		}
		else
		{
			normals[i * 3] = 0;
			normals[i * 3 + 1] = 1;
			normals[i * 3 + 2] = 0;
		}
	}

	testing_ = new gl::VertexBuffer(test, sizeof(GLfloat) * vertex_count_ * 9, gl::buffer_usage::StaticDraw);
	delete[] test;

	vao_ = new gl::VertexArray();
	vao_->bind();

	vertex_buffer_ = new gl::VertexBuffer(vertices_, sizeof(GLfloat) * vertex_count_ * 3, gl::buffer_usage::StaticDraw);

	index_buffer_ = new gl::VertexBuffer();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *index_buffer_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * inditer_, indices, GL_STATIC_DRAW);

	color_buffer_ = new gl::VertexBuffer(colors, sizeof(GLfloat) * vertex_count_ * 4, gl::buffer_usage::StaticDraw);
	texcoords_buffer_ = new gl::VertexBuffer(texcoords, sizeof(GLfloat) * vertex_count_ * 2, gl::buffer_usage::StaticDraw);
	normals_buffer_ = new gl::VertexBuffer(normals, sizeof(GLfloat) * vertex_count_ * 3, gl::buffer_usage::StaticDraw);
	

	vao_->bind_attribute(static_cast<GLuint>(VERTEX_INDEX), *vertex_buffer_, gl::type::Float, 3, 0, NULL);
	vao_->bind_attribute(static_cast<GLuint>(COLOR_INDEX), *color_buffer_, gl::type::Float, 4, 0, NULL);
	vao_->bind_attribute(static_cast<GLuint>(TEXCOORD_INDEX), *texcoords_buffer_, gl::type::Float, 2, 0, NULL);
	vao_->bind_attribute(static_cast<GLuint>(NORMAL_INDEX), *normals_buffer_, gl::type::Float, 3, 0, NULL);
	glBindVertexArray(0);

	delete[] indices;
	delete[] colors;
	delete[] normals;
	delete[] texcoords;
}

model::Mesh::Mesh(const Mesh& mesh)
{
	vertex_count_ = mesh.vertex_count_;
	vertices_ = new GLfloat[vertex_count_ * 3];

	for (unsigned int i = 0; i < vertex_count_; i++)
		vertices_[i] = mesh.vertices_[i];
}

model::Mesh::~Mesh()
{
	vao_->bind();
	delete vertex_buffer_;
	delete color_buffer_;
	delete texcoords_buffer_;
	delete normals_buffer_;
	glBindVertexArray(0);

	delete vao_;
	delete[] vertices_;
}

void model::Mesh::set_material(Material material)
{
	material_ = material;
}

model::Material& model::Mesh::get_material()
{
	return material_;
}

void model::Mesh::draw() const
{
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glDepthRange(0, 1);
	testing_->bind();

	glVertexPointer(3, GL_FLOAT, sizeof(GLfloat) * 9, 0);
	glTexCoordPointer(2, GL_FLOAT, sizeof(GLfloat) * 9, (GLvoid*)(sizeof(GLfloat) * 7));
	glColorPointer(4, GL_FLOAT, sizeof(GLfloat) * 9, (GLvoid*)(sizeof(GLfloat) * 3));
	//vao_->bind();
	if (material_.texture)
		material_.texture->bind();

	

	glDrawArrays(GL_TRIANGLES, 0, inditer_);
	//glDrawElements(GL_TRIANGLES, inditer_, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_TEXTURE_2D);
}

void model::Mesh::translate(const glm::vec3& axis)
{
}

void model::Mesh::translate(const float& x, const float& y, const float& z)
{
	translate(glm::vec3(x, y, z));
}

void model::Mesh::rotate(const float& angle, glm::vec3 axis)
{
}

void model::Mesh::rotate(const float& angle, const float& x, const float& y, const float& z)
{
	rotate(angle, glm::vec3(x, y, z));
}

void model::Mesh::scale(const glm::vec3& scale)
{
}

void model::Mesh::scale(const float& x, const float& y, const float& z)
{
	scale(glm::vec3(x, y, z));
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