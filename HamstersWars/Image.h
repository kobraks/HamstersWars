#pragma once
#include <glm/glm.hpp>
#include <string>

namespace gl::util
{
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

		unsigned int width() const;
		unsigned int height() const;

		const glm::vec4* get_pixels() const;

		glm::vec4 get_pixels(const unsigned int& x, const unsigned int& y) const;
		void set_pixels(const unsigned int& x, const unsigned int& y, const glm::vec4& color);
	private:
		glm::vec4* pixels_;
		unsigned int width_;
		unsigned int height_;

		void load_to_memory(void* image_file);
	};
}

