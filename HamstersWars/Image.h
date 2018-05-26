#pragma once
#include <glm/glm.hpp>
#include <string>

namespace gl::util
{
	namespace image_type
	{
		enum image_type_t
		{
			BMP,
			EXR,
			J2K,
			JP2,
			JPEG,
			JXR,
			PNG,
			PBM,
			PGM,
			PPM,
			TIFF
		};
	}

	class Image
	{
	public:
		Image();
		Image(const unsigned int& width, const unsigned int& height, const glm::vec4& background);
		Image(const unsigned int& width, const unsigned int& height, unsigned char* pixels);
		Image(unsigned char* pixels, const unsigned int& size);
		explicit Image(const std::string& file_name);
		Image(const Image&) = delete;

		~Image();

		Image& operator=(const Image&) = delete;

		void load(unsigned char* pixels, const unsigned int& size);
		void load(const std::string& file_name);

		void save(const std::string& file_name, image_type::image_type_t type);

		unsigned int width() const;
		unsigned int height() const;

		const glm::vec4* get_pixels() const;

		glm::vec4& get_pixel(const unsigned int& x, const unsigned int& y);
		glm::vec4 get_pixel(const unsigned int& x, const unsigned int& y) const;
		void set_pixel(const unsigned int& x, const unsigned int& y, const glm::vec4& color);

		glm::vec4 operator()(const unsigned int& x, const unsigned int& y) const;
		glm::vec4& operator()(const unsigned int& x, const unsigned int& y);

		static glm::vec4 translate_color(const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a);
		static glm::vec4 translate_color(const unsigned char color[4]);
	private:
		glm::vec4* pixels_;
		unsigned int width_;
		unsigned int height_;

		void load_to_memory(void* image_file);
	};
}

