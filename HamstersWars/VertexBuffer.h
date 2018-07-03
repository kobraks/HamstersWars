#pragma once

#include "gl/glew.h"
#include <memory>

namespace gl
{
	namespace buffer_usage
	{
		enum buffer_usage_t
		{
			StreamDraw = GL_STREAM_DRAW,
			StreamRead = GL_STREAM_READ,
			StreamCopy = GL_STREAM_COPY,
			StaticDraw = GL_STATIC_DRAW,
			StaticRead = GL_STATIC_READ,
			StaticCopy = GL_STATIC_COPY,
			DynamicDraw = GL_DYNAMIC_DRAW,
			DynamicRead = GL_DYNAMIC_READ,
			DynamicCopy = GL_DYNAMIC_COPY
		};
	}

	class VertexBuffer
	{
	public:
		VertexBuffer();
		VertexBuffer(const VertexBuffer& copy) = default;
		VertexBuffer(const void* data, const size_t& lenght, buffer_usage::buffer_usage_t usage);

		operator GLuint() const;
		VertexBuffer& operator=(const VertexBuffer& copy) = default;

		void data(const void* data, const size_t& lenght, buffer_usage::buffer_usage_t usage) const;
		void sub_data(const void* data, const size_t& offset, const size_t& lenght) const;
		void get_sub_data(void* data, const size_t& offset, const size_t& lenght) const;

		void bind() const;

		void copy_sub_data(VertexBuffer& destination, intptr_t source_offset, intptr_t dest_offset, uintptr_t size);
	private:
		std::shared_ptr<GLuint> obj_;
	};

}


