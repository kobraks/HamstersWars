#pragma once
#include <gl/glew.h>
#include <memory>
#include <glm/glm.hpp>
#include "Image.h"

namespace gl
{
	namespace internal_format
	{
		enum internal_format_t
		{
			Compressed_Red = GL_COMPRESSED_RED,
			Compressed_Red_RGTC1 = GL_COMPRESSED_RED_RGTC1,
			Compressed_RG = GL_COMPRESSED_RG,
			Compressed_RGB = GL_COMPRESSED_RGB,
			Compressed_RGBA = GL_COMPRESSED_RGBA,
			Compressed_RG_RGTC2 = GL_COMPRESSED_RG_RGTC2,
			Compressed_Signed_Red_RGTC1 = GL_COMPRESSED_SIGNED_RED_RGTC1,
			Compressed_Signed_RG_RGTC2 = GL_COMPRESSED_SIGNED_RG_RGTC2,
			Compressed_SRGB = GL_COMPRESSED_SRGB,
			Depth_Stencil = GL_DEPTH_STENCIL,
			Depth24_Stencil8 = GL_DEPTH24_STENCIL8,
			Depth32F_Stencil8 = GL_DEPTH32F_STENCIL8,
			Depth_Component = GL_DEPTH_COMPONENT,
			Depth_Component16 = GL_DEPTH_COMPONENT16,
			Depth_Component24 = GL_DEPTH_COMPONENT24,
			Depth_Component32F = GL_DEPTH_COMPONENT32F,
			R16F = GL_R16F,
			R16I = GL_R16I,
			R16_SNorm = GL_R16_SNORM,
			R16UI = GL_R16UI,
			R32F = GL_R32F,
			R32I = GL_R32I,
			R32UI = GL_R32UI,
			R3_G3_B2 = GL_R3_G3_B2,
			R8 = GL_R8,
			R8I = GL_R8I,
			R8_SNorm = GL_R8_SNORM,
			R8UI = GL_R8UI,
			Red = GL_RED,
			RG = GL_RG,
			RG16 = GL_RG16,
			RG16F = GL_RG16F,
			RG16_SNorm = GL_RG16_SNORM,
			RG32F = GL_RG32F,
			RG32I = GL_RG32I,
			RG32UI = GL_RG32UI,
			RG8 = GL_RG8,
			RG8I = GL_RG8I,
			RG8_SNorm = GL_RG8_SNORM,
			RG8UI = GL_RG8UI,
			RGB = GL_RGB,
			RGB10 = GL_RGB10,
			RGB10A2 = GL_RGB10_A2,
			RGB12 = GL_RGB12,
			RGB16 = GL_RGB16,
			RGB16F = GL_RGB16F,
			RGB16I = GL_RGB16I,
			RGB16UI = GL_RGB16UI,
			RGB32F = GL_RGB32F,
			RGB32I = GL_RGB32I,
			RGB32UI = GL_RGB32UI,
			RGB4 = GL_RGB4,
			RGB5 = GL_RGB5,
			RGB5_A1 = GL_RGB5_A1,
			RGB8 = GL_RGB8,
			RGB8I = GL_RGB8I,
			RGB8UI = GL_RGB8UI,
			RGB9_E5 = GL_RGB9_E5,
			RGBA = GL_RGBA,
			RGBA12 = GL_RGBA12,
			RGBA16 = GL_RGBA16,
			RGBA16F = GL_RGBA16F,
			RGBA16I = GL_RGBA16I,
			RGBA16UI = GL_RGBA16UI,
			RGBA2 = GL_RGBA2,
			RGBA32F = GL_RGBA32F,
			RGBA32I = GL_RGBA32I,
			RGBA32UI = GL_RGBA32UI,
			RGBA4 = GL_RGBA4,
			RGBA8 = GL_RGBA8,
			RGBA8UI = GL_RGBA8UI,
			SRGB8 = GL_SRGB8,
			SRGB8A8 = GL_SRGB8_ALPHA8,
			SRGBA = GL_SRGB_ALPHA
		};
	}

	namespace format
	{
		enum format_t
		{
			Red = GL_RED,
			RGB = GL_RGB,
			BGR = GL_BGR,
			RGBA = GL_RGBA,
			BGRA = GL_BGRA
		};
	}

	namespace data_type
	{
		enum data_type_t
		{
			Byte = GL_BYTE,
			UnsignedByte = GL_UNSIGNED_BYTE,
			Short = GL_SHORT,
			UnsignedShort = GL_UNSIGNED_SHORT,
			Int = GL_INT,
			UnsignedInt = GL_UNSIGNED_INT,
			Float = GL_FLOAT,
			Double = GL_DOUBLE,

			UnsignedByte332 = GL_UNSIGNED_BYTE_3_3_2,
			UnsignedByte233Rev = GL_UNSIGNED_BYTE_2_3_3_REV,
			UnsignedShort565 = GL_UNSIGNED_SHORT_5_6_5,
			UnsignedShort565Rev = GL_UNSIGNED_SHORT_5_6_5,
			UnsignedShort4444 = GL_UNSIGNED_SHORT_4_4_4_4,
			UnsignedShort4444Rev = GL_UNSIGNED_SHORT_4_4_4_4_REV,
			UnsignedShort5551 = GL_UNSIGNED_SHORT_5_5_5_1,
			UnsignedShort1555Rev = GL_UNSIGNED_SHORT_1_5_5_5_REV,
			UnsignedInt8888 = GL_UNSIGNED_INT_8_8_8_8,
			UnsignedInt8888Rev = GL_UNSIGNED_INT_8_8_8_8_REV,
			UnsignedInt101010102 = GL_UNSIGNED_INT_10_10_10_2
		};
	}

	namespace warpping
	{
		enum warpping_t
		{
			Clamp_Edge = GL_CLAMP_TO_EDGE,
			Clamp_Border = GL_CLAMP_TO_BORDER,
			Repeat = GL_REPEAT,
			Mirrored_Repeat = GL_MIRRORED_REPEAT
		};
	}

	namespace filter
	{
		enum filter_t
		{
			Nearest = GL_NEAREST,
			Linear = GL_LINEAR,
			Nearest_Mipmap_Nearest = GL_NEAREST_MIPMAP_NEAREST,
			Linear_Mipmap_Nearest = GL_LINEAR_MIPMAP_NEAREST,
			Nearest_Mipmap_Linear = GL_NEAREST_MIPMAP_LINEAR,
			Linear_Mipmap_Linear = GL_LINEAR_MIPMAP_LINEAR
		};
	}

	class Texture
	{
	public:
		Texture();
		Texture(const Texture&) = default;
		Texture(const GLubyte* pixels, unsigned int width, unsigned int height,
		        internal_format::internal_format_t internal_format = internal_format::RGBA);
		explicit Texture(const util::Image& image, internal_format::internal_format_t internal_format = internal_format::RGBA);

		operator GLuint() const;
		Texture& operator=(const Texture&) = default;

		void image_2D(const GLvoid* data, data_type::data_type_t type, format::format_t format, unsigned int width,
		              unsigned int height, internal_format::internal_format_t internal_format);

		void set_warpping(warpping::warpping_t s);
		void set_warpiing(warpping::warpping_t s, warpping::warpping_t t);
		void set_warpiing(warpping::warpping_t s, warpping::warpping_t t, warpping::warpping_t r);

		void set_filters(filter::filter_t min, filter::filter_t mag);
		void set_border_color(const glm::vec4& color);

		void generate_mip_maps();

	private:
		std::shared_ptr<GLuint> obj_;

	};
}

