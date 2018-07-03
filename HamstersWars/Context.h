#pragma once
#include "Program.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Framebuffer.h"

#include <GL/glew.h>
#include <Windows.h>

namespace gl
{
	namespace buffer
	{
		enum buffer_t
		{
			color = GL_COLOR_BUFFER_BIT,
			depth = GL_DEPTH_BUFFER_BIT,
			stencil = GL_STENCIL_BUFFER_BIT
		};

		inline buffer_t operator|(const buffer_t& left, const buffer_t& right)
		{
			return static_cast<buffer_t>(static_cast<int>(left) | static_cast<int>(right));
		}
	}

	namespace primitive
	{
		enum primitive_t
		{
			Triangles = GL_TRIANGLES,
			Linse = GL_LINES,
			Quads = GL_QUADS,
			Points = GL_POINTS
		};
	}

	namespace capability
	{
		enum capability_t
		{
			depth_text = GL_DEPTH_TEST,
			stencil_test = GL_STENCIL_TEST,
			cull_face = GL_CULL_FACE,
			rasterizer_discard = GL_RASTERIZER_DISCARD,
			texture_2d = GL_TEXTURE_2D,
			texture_3d = GL_TEXTURE_3D,
			texture_1d = GL_TEXTURE_1D,
			lighting = GL_LIGHTING,
			light_0 = GL_LIGHT0,
			light_1 = GL_LIGHT1,
			light_2 = GL_LIGHT2,
			light_3 = GL_LIGHT3,
			light_4 = GL_LIGHT4,
			light_5 = GL_LIGHT5,
			light_6 = GL_LIGHT6,
			light_7 = GL_LIGHT7,
		};
	}

	namespace test_function
	{
		enum test_function_t
		{
			Never = GL_NEVER,
			Less = GL_LESS,
			LessEqual = GL_LEQUAL,
			Greater = GL_GREATER,
			GreaterEqual = GL_GEQUAL,
			Equal = GL_EQUAL,
			NotEqual = GL_NOTEQUAL,
			Always = GL_ALWAYS
		};
	}

	namespace stencil_action
	{
		enum stencil_action_t
		{
			Keep = GL_KEEP,
			Zero = GL_ZERO,
			Replace = GL_REPLACE,
			Increase = GL_INCR,
			IncreaseWrap = GL_INCR_WRAP,
			Decrease = GL_DECR,
			DecreaseWrap = GL_DECR_WRAP,
			Invert = GL_INVERT
		};
	}

	class Window;
	class Context
	{
	public:
		~Context();

		void active();
		void set_vertical_sync(const bool& enabled);

		void enable(const capability::capability_t& capability);
		void disable(const capability::capability_t& capability);

		void clear_color(const glm::vec4& color);
		void clear(const buffer::buffer_t& buffers = buffer::color | buffer::depth);

		void use_program(const Program& program);
		
		void bind_texture(const Texture& texture, const GLuint& unit);


		void draw_arrays(const VertexArray& vao, const primitive::primitive_t& mode, const unsigned int& offset,
		                 const unsigned int& verticles);
		void draw_elements(const VertexArray& vao, const primitive::primitive_t& mode, intptr_t offset,
		                   const unsigned int& count, const unsigned int& type);

		void depth_mask(const bool& write_enabled);
		void stencil_mask(const bool& wrtie_enabled);
		void stencil_mask(const unsigned int& mask);

		void stencil_func(const test_function::test_function_t& function, const int& reference, const unsigned int mask = ~0);
		void stencil_op(const stencil_action::stencil_action_t& fail, const stencil_action::stencil_action_t& zfail, const stencil_action::stencil_action_t& pass);

		void bind_framebuffer(const Framebuffer& framebuffer);
		void bind_framebuffer();

		void begin_transform_feedback(const primitive::primitive_t& mode);
		void end_transformation_feedback();

		float time();

		static Context use_existing_context();
	private:
		friend class Window;

		Context();

		bool owned_;
		GLint default_viewport_[4];
		Context(const unsigned char& color, const unsigned char& depth, const unsigned char& stencil, const unsigned int& antialias, HDC dc);

		HDC dc;
		HGLRC context;

		LARGE_INTEGER timeOffset;
	};
}
