#include "stdafx.h"
#include "VertexArray.h"

#include "VertexBuffer.h"

gl::VertexArray::VertexArray()
{
	obj_ = std::shared_ptr<GLuint>(new GLuint(0), [=](GLuint* id) { glDeleteVertexArrays(1, id); });

	glGenVertexArrays(1, &*obj_);
}

gl::VertexArray::operator unsigned() const
{
	return *obj_;
}

void gl::VertexArray::bind_attribute(const Attribute& attribute, const VertexBuffer& buffer, type::type_t type,
	const unsigned& count, const unsigned& stride, intptr_t offset) const
{
	bind();
	buffer.bind();
	glVertexAttribPointer(attribute, count, type, GL_FALSE, stride, reinterpret_cast<const GLvoid*>(offset));
	glEnableVertexAttribArray(attribute);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void gl::VertexArray::bind_elements(const VertexBuffer& elements) const
{
	bind();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elements);
	glBindVertexArray(0);

}

void gl::VertexArray::bind_transfrom_feedback(const unsigned& index, const VertexBuffer& buffer) const
{
	bind();

	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, index, buffer);
	glBindVertexArray(0);

}

void gl::VertexArray::bind() const
{
	glBindVertexArray(*obj_);
}
