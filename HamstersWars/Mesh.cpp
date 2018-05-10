#include "Mesh.h"
#include "Log.h"

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Defines.h"

float color = 0;
#define COLOR_INC 0.1f


model::Mesh::Mesh(const aiMesh& mesh) : rotate_angle_(0), scale_vec_(1.f, 1.f, 1.f)
{
	vertex_count_ = mesh.mNumVertices;
	element_count_ = mesh.mNumFaces * 3;
	indler_ = 0;

	vertices_ = new GLfloat[vertex_count_ * 3];
	auto colors = new GLfloat[vertex_count_ * 4];
	auto texcoords = new GLfloat[vertex_count_ * 2];
	auto normals = new GLfloat[vertex_count_ * 3];
	auto indices = new GLuint[vertex_count_];

	if (mesh.HasNormals())
	{
		Log::level() = Log::log_warning;
		Log::print("Model has not normal vectors");
	}

	for (size_t i = 0; i < vertex_count_; ++i)
	{
		vertices_[i * 3 + 0] = mesh.mVertices[i].x;
		vertices_[i * 3 + 1] = mesh.mVertices[i].y;
		vertices_[i * 3 + 2] = mesh.mVertices[i].z;

		/*colors[i * 4 + 0] = color;
		colors[i * 4 + 1] = color;
		colors[i * 4 + 2] = color;
		colors[i * 4 + 3] = 1.f;

		if (color <= 1.f)
			color += COLOR_INC;
		else
			color = 0;*/

		if (mesh.HasVertexColors(0))
		{
			colors[i * 4 + 0] = mesh.mColors[0][i].r;
			colors[i * 4 + 1] = mesh.mColors[0][1].g;
			colors[i * 4 + 2] = mesh.mColors[0][i].b;
			colors[i * 4 + 3] = mesh.mColors[0][i].a;
		}
		else
		{
			colors[i * 4 + 0] = 1.f;
			colors[i * 4 + 1] = 1.f;
			colors[i * 4 + 2] = 1.f;
			colors[i * 4 + 3] = 1.f;
		}

		if (mesh.HasTextureCoords(0))
		{
			texcoords[i * 2 + 0] = mesh.mTextureCoords[0][i].x;
			texcoords[i * 2 + 1] = mesh.mTextureCoords[0][i].y;
		}

		if (mesh.HasNormals())
		{
			normals[i * 3 + 0] = mesh.mNormals[0].x;
			normals[i * 3 + 1] = mesh.mNormals[0].y;
			normals[i * 3 + 2] = mesh.mNormals[0].z;
		}
		else
		{
			normals[i * 3 + 0] = 0;
			normals[i * 3 + 0] = 1;
			normals[i * 3 + 0] = 0;
		}
	}

	if (mesh.HasFaces())
		for (size_t i = 0; i < mesh.mNumFaces; ++i)
		{
			indices[i * 3 + 0] = mesh.mFaces[i].mIndices[0];
			indices[i * 3 + 1] = mesh.mFaces[i].mIndices[1];
			indices[i * 3 + 2] = mesh.mFaces[i].mIndices[2];
		}


	vao_ = new gl::VertexArray();
	vao_->bind();

	vertex_buffer_ = new gl::VertexBuffer(vertices_, sizeof(GLfloat) * vertex_count_ * 3, gl::buffer_usage::StaticDraw);
	color_buffer_ = new gl::VertexBuffer(colors, sizeof(GLfloat) * vertex_count_ * 4, gl::buffer_usage::StaticDraw);
	texcoords_buffer_ = new gl::VertexBuffer(texcoords, sizeof(GLfloat) * vertex_count_ * 2, gl::buffer_usage::StaticDraw);
	normals_buffer_ = new gl::VertexBuffer(normals, sizeof(GLfloat) * vertex_count_ * 3, gl::buffer_usage::StaticDraw);
	
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
	//glEnable(GL_TEXTURE_2D);
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//glEnableClientState(GL_COLOR_ARRAY);
	//glPushMatrix();
	//testing_->bind();

	//glVertexPointer(3, GL_FLOAT, sizeof(GLfloat) * 9, 0);
	//glTexCoordPointer(2, GL_FLOAT, sizeof(GLfloat) * 9, (GLvoid*)(sizeof(GLfloat) * 7));
	//glColorPointer(4, GL_FLOAT, sizeof(GLfloat) * 9, (GLvoid*)(sizeof(GLfloat) * 3));
	vao_->bind();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *index_buffer_);
	//vertex_buffer_->bind();
	if (material_.texture)
		material_.texture->bind();


	
	
	//glDrawArrays(GL_TRIANGLES, 0, element_count_);
	glDrawElements(GL_TRIANGLES, element_count_, GL_UNSIGNED_INT, NULL);

	auto error = glGetError();

	if(error != GL_NO_ERROR)
	{
		Log::level() = Log::log_error;
		Log::print("%s", glewGetErrorString(error));

	}

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	//glDisableClientState(GL_COLOR_ARRAY);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//glDisableClientState(GL_VERTEX_ARRAY);
	//glDisable(GL_TEXTURE_2D);
	//glPopMatrix();
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
