#include "VertexBuffer.h"



gl::VertexBuffer::VertexBuffer()
{
	obj_ = std::shared_ptr<GLuint>(new GLuint(0), [=](GLuint* id) { glDeleteBuffers(1, id); });

	glGenBuffers(1, &*obj_);
}

gl::VertexBuffer::VertexBuffer(const void* data, const size_t& lenght, buffer_usage::buffer_usage_t usage) : VertexBuffer()
{
	this->data(data, lenght, usage);
}

void gl::VertexBuffer::data(const void* data, const size_t& lenght, buffer_usage::buffer_usage_t usage) const
{
	bind();
	glBufferData(GL_ARRAY_BUFFER, lenght, data, usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void gl::VertexBuffer::sub_data(const void* data, const size_t& offset, const size_t& lenght) const
{
	bind();
	glBufferSubData(GL_ARRAY_BUFFER, offset, lenght, data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void gl::VertexBuffer::get_sub_data(void* data, const size_t& offset, const size_t& lenght) const
{
	bind();
	glGetBufferSubData(GL_ARRAY_BUFFER, offset, lenght, data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void gl::VertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, *obj_);
}

void gl::VertexBuffer::copy_sub_data(VertexBuffer& destination, intptr_t source_offset, intptr_t dest_offset,
	uintptr_t size)
{
	glCopyBufferSubData(*this, destination, source_offset, dest_offset, size);
}

gl::VertexBuffer::operator unsigned() const
{
	return *obj_;
}
