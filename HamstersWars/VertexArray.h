#pragma once
#include <gl/glew.h>
#include <memory>


namespace gl
{
	class VertexBuffer;
	typedef GLuint Attribute;

	namespace type
	{
		enum type_t
		{
			Byte = GL_BYTE,
			UnsignedByte = GL_UNSIGNED_BYTE,
			Short = GL_SHORT,
			UnsignedShort = GL_UNSIGNED_SHORT,
			Int = GL_INT,
			UnsignedInt = GL_UNSIGNED_INT,
			Float = GL_FLOAT,
			Double = GL_DOUBLE
		};
	}

	class VertexArray
	{
	public:
		VertexArray();
		VertexArray(const VertexArray& copy) = default;

		VertexArray& operator= (const VertexArray& copy) = default;
		operator GLuint() const;

		void bind_attribute(const Attribute& attribute, const VertexBuffer& buffer, type::type_t type, const unsigned int& count, const unsigned int& stride, intptr_t offset) const;
		void bind_elements(const VertexBuffer& elements) const;

		void bind_transfrom_feedback(const unsigned int& index, const VertexBuffer& buffer) const;

		void bind() const;
	private:
		std::shared_ptr<GLuint> obj_;
	};
}
