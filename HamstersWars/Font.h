#pragma once
#include <string>
#include "Glyph.h"
#include "Texture.h"
#include "Rectangle.h"
#include <unordered_map>
#include <vector>


namespace game
{
	class Font
	{
	public:
		struct Info
		{
			std::string family;
		};

		Font();
		Font(const Font& copy);

		Font& operator=(const Font& copy);

		void load_from_file(const std::string& file_name);
		void load_from_memory(const void* data, const size_t& size);
		const Glyph& get_glyph(const unsigned int& code_point, const unsigned int& character_size, bool bold, const float& outline_thickness = 0) const;
		float get_kerning(const unsigned int& first, const unsigned int& secound, const unsigned int& character_size) const;
		float get_line_spacing(const unsigned int& character_size) const;
		float get_underline_position(const unsigned int& character_size) const;
		float get_underline_thickness(const unsigned int& character_size) const;

		const gl::Texture& get_texture(unsigned int character_size) const;

		const Info& get_info() const;

		~Font();

	private:
		void clean();

		struct Row
		{
			Row(unsigned int row_top, unsigned int row_height) : width(0), top(row_top), height(row_height)
			{
				
			}

			unsigned int width;
			unsigned int top;
			unsigned int height;
		};

		typedef std::unordered_map<unsigned long int, Glyph> glyph_table;

		struct Page
		{
			Page();

			glyph_table glyphs;
			gl::Texture texture;
			unsigned int next_row;
			std::vector<Row> rows;
		};

		typedef std::unordered_map<unsigned int, Page> page_table;

		const size_t max_glpyhs_;
		gl::Texture image_;

		void* library_;
		void* face_;
		void* stream_rec_;
		void* stroker_;
		int* ref_count_;
		Info info_;

		mutable page_table pages_;
		mutable std::vector<glm::vec4> pixel_buffer_;


		Glyph load_glyph(unsigned int code_point, unsigned int character_size, bool bold, float outline_thickness) const;
		IntRect find_glyph_rect(Page& page, unsigned int widht, unsigned int height) const;
		bool set_current_size(unsigned int character_size) const;

	};
}

