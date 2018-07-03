#pragma once
#include <string>
#include <glm/glm.hpp>

#include "Font.h"
#include "Drawable.h"
#include "Transformable2d.h"

namespace game
{
	enum style
	{
		Regular = 0,
		Bold = 1,
		italic = 1 << 1,
		underlined = 1 << 2,
		strike_throught = 1 << 3
	};

	class Text : public Drawable, public Transformable2d
	{
	public:
		Text();
		Text(const std::string& string, const Font& font, const unsigned int& character_size = 30u);
		~Text();

		void set_string(const std::string& string);
		void set_font(const Font& font);
		
		void set_character_size(const unsigned int& size);
		void set_line_spacing(const float& separating_factor);
		void set_letter_spacing(const float spacing_factor);
		
		void set_style(const unsigned int& style);
		
		void set_fill_color(const glm::vec4& color);
		void set_out_line_color(const glm::vec4& color);
		void set_outline_thickness(const float& thickness);

		const std::string& get_string() const;
		const Font* get_font() const;

		unsigned int get_character_size() const;
		float get_letter_spacing() const;
		float get_line_spacing() const;
		
		unsigned int get_style() const;

		const glm::vec4& get_fill_color() const;
		const glm::vec4& get_outline_color() const;
		float get_outline_thickness() const;

		glm::vec2 find_character_pos(const size_t& index) const;


	};

}
