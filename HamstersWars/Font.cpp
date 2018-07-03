#include "Font.h"
#include <freetype/ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_BITMAP_H
#include FT_STROKER_H

#include "Log.h"
#include "Texture.h"

template <typename T, typename U>
inline T reinterpret(const U& input)
{
	T output;
	std::memcpy(&output, &input, sizeof(U));
	return output;
}

unsigned long int combine(float outlineThickness, bool bold, unsigned int codePoint)
{
	return (static_cast<unsigned long int >(reinterpret<unsigned int>(outlineThickness)) << 32) | (static_cast<unsigned long int>(bold) << 31) | codePoint;
}

game::Font::Font() : max_glpyhs_(5)
{
}

void game::Font::load_from_file(const std::string& file_name)
{
	clean();
	FT_Library library;
	if(FT_Init_FreeType(&library))
	{
		//TODO errors
	}

	FT_Face face;
	if(FT_New_Face(library, file_name.c_str(), 0, &face))
	{
		//TODO errors
	}

	FT_Stroker stroker;
	if(FT_Stroker_New(library, &stroker))
	{
		FT_Done_Face(face);
		//TODO errors
	}

	if (FT_Select_Charmap(face, FT_ENCODING_UNICODE))
	{
		FT_Stroker_Done(stroker);
		FT_Done_Face(face);
	}

	face_ = face;
	library_ = library;
	stroker_ = stroker;

	info_.family = face->family_name ? face->family_name : std::string();
}

void game::Font::load_from_memory(const void* data, const size_t& size)
{
	clean();

	FT_Library library;
	if (FT_Init_FreeType(&library))
	{
		//TODO errors
	}

	FT_Face face;
	if (FT_New_Memory_Face(library, static_cast<const FT_Byte*>(data), size, 0, &face))
	{
		//TODO errors
	}

	FT_Stroker stroker;
	if (FT_Stroker_New(library, &stroker))
	{
		FT_Done_Face(face);
		//TODO errors
	}

	if (FT_Select_Charmap(face, FT_ENCODING_UNICODE))
	{
		FT_Stroker_Done(stroker);
		FT_Done_Face(face);
	}

	face_ = face;
	library_ = library;
	stroker_ = stroker;

	info_.family = face->family_name ? face->family_name : std::string();
}

const game::Glyph& game::Font::get_glyph(const unsigned& code_point, const unsigned& character_size, bool bold,
	const float& outline_thickness) const
{
	glyph_table& glyphs = pages_[character_size].glyphs;

	auto key = combine(outline_thickness, bold, code_point);

	auto it = glyphs.find(key);
	if (it != glyphs.end())
		return it->second;
	else
	{
		auto glyph = load_glyph(code_point, character_size, bold, outline_thickness);
		return glyphs.insert(std::make_pair(key, glyph)).first->second;
	}
}

float game::Font::get_kerning(const unsigned& first, const unsigned& secound, const unsigned& character_size) const
{
	if (first == 0 || secound == 0)
		return 0.f;

	FT_Face face = static_cast<FT_Face>(face_);

	if (face && FT_HAS_KERNING(face) && set_current_size(character_size))
	{
		FT_UInt index1 = FT_Get_Char_Index(face, first);
		FT_UInt index2 = FT_Get_Char_Index(face, secound);

		FT_Vector kerning;
		FT_Get_Kerning(face, index1, index2, FT_KERNING_DEFAULT, &kerning);

		if (!FT_IS_SCALABLE(face))
			return static_cast<float>(kerning.x);

		return static_cast<float>(kerning.x) / static_cast<float>(1 << 6);
	}
	else
		return 0.f;


}

float game::Font::get_line_spacing(const unsigned& character_size) const
{
	FT_Face face = static_cast<FT_Face>(face_);

	if (face && set_current_size(character_size))
		return static_cast<float>(face->size->metrics.height) / static_cast<float>(1 << 6);
	else
		return 0.f;
}

float game::Font::get_underline_position(const unsigned& character_size) const
{
	FT_Face face = static_cast<FT_Face>(face_);

	if (face && set_current_size(character_size))
	{
		if (!FT_IS_SCALABLE(face))
			return character_size / 10.f;

		return -static_cast<float>(FT_MulFix(face->underline_position, face->size->metrics.y_scale)) / static_cast<float>(1 << 6);
	}
	else
		return 0.f;
}

float game::Font::get_underline_thickness(const unsigned& character_size) const
{
	FT_Face face = static_cast<FT_Face>(face_);

	if (face && set_current_size(character_size))
	{
		if (!FT_IS_SCALABLE(face))
			return character_size / 14.f;

		return static_cast<float>(FT_MulFix(face->underline_thickness, face->size->metrics.y_scale)) / static_cast<float>(1 << 6);
	}
	else
		return 0.f;
}

const gl::Texture& game::Font::get_texture(unsigned character_size) const
{
	return pages_[character_size].texture;
}


const game::Font::Info& game::Font::get_info() const
{
	return info_;
}

game::Font::~Font()
{
}

void game::Font::clean()
{
	if (ref_count_)
	{
		(*ref_count_)--;

		if (!(*ref_count_))
		{
			delete ref_count_;

			if (stroker_)
				FT_Stroker_Done(static_cast<FT_Stroker>(stroker_));

			if (face_)
				FT_Done_Face(static_cast<FT_Face>(face_));

			if (stream_rec_)
				delete static_cast<FT_StreamRec*>(stream_rec_);

			if (library_)
				FT_Done_FreeType(static_cast<FT_Library>(library_));
		}
	}

	library_ = nullptr;
	face_ = nullptr;
	stroker_ = nullptr;
	stream_rec_ = nullptr;
	ref_count_ = nullptr;
	pages_.clear();

	std::vector<glm::vec4>().swap(pixel_buffer_);
}

game::Font::Page::Page() : next_row(3)
{
	gl::util::Image image();
}

game::Glyph game::Font::load_glyph(unsigned code_point, unsigned character_size, bool bold,
	float outline_thickness) const
{
	Glyph glyph;

	FT_Face face = static_cast<FT_Face>(face_);
	if (!face)
		return glyph;


	if (!set_current_size(character_size))
		return glyph;

	FT_Int32 flags = FT_LOAD_TARGET_NORMAL | FT_LOAD_FORCE_AUTOHINT;
	if (outline_thickness != 0)
		flags |= FT_LOAD_NO_BITMAP;

	if (FT_Load_Char(face, code_point, flags))
		return glyph;

	FT_Glyph glyph_desc;
	if (FT_Get_Glyph(face->glyph, &glyph_desc))
		return glyph;

	FT_Pos weight = 1 << 6;
	bool outline = (glyph_desc->format == FT_GLYPH_FORMAT_OUTLINE);

	if (outline)
	{
		if(bold)
		{
			FT_OutlineGlyph outline_glyph = reinterpret_cast<FT_OutlineGlyph>(glyph_desc);
			FT_Outline_Embolden(&outline_glyph->outline, weight);
		}

		if (outline_thickness != 0)
		{
			FT_Stroker stroker = static_cast<FT_Stroker>(stroker_);

			FT_Stroker_Set(stroker, static_cast<FT_Fixed>(outline_thickness * static_cast<float>(1 << 6)), FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);
			FT_Glyph_Stroke(&glyph_desc, stroker, true);
		}
	}

	FT_Glyph_To_Bitmap(&glyph_desc, FT_RENDER_MODE_NORMAL, 0, 1);
	FT_Bitmap& bitmap = reinterpret_cast<FT_BitmapGlyph>(glyph_desc)->bitmap;

	if (!outline)
	{
		if (bold)
			FT_Bitmap_Embolden(static_cast<FT_Library>(library_), &bitmap, weight, weight);
		if (outline_thickness != 0)
		{
			//TODO error
		}
	}

	glyph.advace = static_cast<float>(face->glyph->metrics.horiAdvance) / static_cast<float>(1 << 6);
	if (bold)
		glyph.advace += static_cast<float>(weight) / static_cast<float>(1 << 6);

	int width = bitmap.width;
	int height = bitmap.rows;

	if ((width > 0) && (height > 0 ))
	{
		const unsigned int padding = 1;

		width += 2 * padding;
		height += 2 * padding;

		Page& page = pages_[character_size];
		glyph.texture_rect = find_glyph_rect(page, width, height);

		glyph.texture_rect.left += padding;
		glyph.texture_rect.top += padding;
		glyph.texture_rect.width -= 2 * padding; //WIDTH
		glyph.texture_rect.height -= 2 * padding; //HEIGHT

		glyph.bounds[0] = static_cast<float>(face->glyph->metrics.horiBearingX) / static_cast<float>(1 << 6);
		glyph.bounds[1] = -static_cast<float>(face->glyph->metrics.horiBearingY) / static_cast<float>(1 << 6);
		glyph.bounds[2] = static_cast<float>(face->glyph->metrics.width) / static_cast<float>(1 << 6) + outline_thickness * 2;
		glyph.bounds[3] = static_cast<float>(face->glyph->metrics.height) / static_cast<float>(1 << 6) + outline_thickness * 2;

		pixel_buffer_.resize(width * height);

		glm::vec4* current = &pixel_buffer_[0];
		glm::vec4* end = current + width * height;

		if (current != end)
		{
			(*current++) = glm::vec4(1, 1, 1, 0);
		}

		const unsigned char* pixels = bitmap.buffer;
		if (bitmap.pixel_mode == FT_PIXEL_MODE_MONO)
		{
			int k = 0;

			for (unsigned int i = padding; i < height - padding; i++)
			{
				for (unsigned int j = padding; j < width - padding; j++)
				{
					if (k >= 4)
						k = 0;

					size_t index = j + i * width;
					unsigned char tmp = ((pixels[(j - padding) / 8]) & (1 << (7 - ((j - padding) % 8)))) ? 255 : 0;
					float color = static_cast<float>(tmp) / 255.f;

					pixel_buffer_[index + 3][k++] = color;
				}
				pixels += bitmap.pitch;
			}
		}
		else
		{
			int k = 0;
			for (unsigned int i = padding; i < height - padding; i++)
			{
				for (unsigned int j = padding; j < width - padding; j++)
				{
					if (k >= 4)
						k = 0;

					size_t index = j + i * width;

					unsigned char tmp = pixels[j - padding];
					float color = static_cast<float>(tmp) / 255.f;

					pixel_buffer_[index + 3][k++] = color;
				}
				pixels += bitmap.pitch;
			}
		}

		unsigned int x = glyph.texture_rect.left - padding;
		unsigned int y = glyph.texture_rect.top - padding;
		unsigned int w = glyph.texture_rect.width + 2 * padding;
		unsigned int h = glyph.texture_rect.height + 2 * padding;

		page.texture.update(&pixel_buffer_[0], w, h, x, y);
	}

	FT_Done_Glyph(glyph_desc);

	return glyph;
}

game::IntRect game::Font::find_glyph_rect(Page& page, unsigned width, unsigned height) const
{
	Row* row = nullptr;
	float best_ratio = 0;

	for (std::vector<Row>::iterator it = page.rows.begin(); it != page.rows.end() && !row; ++it)
	{
		float ratio = static_cast<float>(height) / it->height;

		if ((ratio < 0.7f) || (ratio > 1.f))
			continue;

		if (width > page.texture.size().x - it->width)
			continue;
			

		if (ratio < best_ratio)
			continue;

		row = &*it;
		best_ratio = ratio;
	}

	if (!row)
	{
		int row_height = height + height / 10;
		while ((page.next_row + row_height >= page.texture.size().y) || width >= page.texture.size().x)
		{
			unsigned int texture_width = page.texture.width();
			unsigned int texture_height = page.texture.height();

			if((texture_width * 2 <= gl::Texture::get_maximum_size()) && (texture_height * 2 <= gl::Texture::get_maximum_size()))
			{
				gl::util::Image texture(texture_width * 2, texture_height * 2);
				//page.texture.swap(texture);
			}
			else
			{
				Log::level() = Log::log_error;
				Log::print("Failed to add a new character to the font: the maximum texture size has been reached");
				return IntRect(0, 0, 2, 2);
			}
		}

		page.rows.push_back(Row(page.next_row, row_height));
		page.next_row += row_height;
		row = &page.rows.back();
	}

	IntRect rect(row->width, row->top, width, height);
	row->width += width;

	return rect;
}

bool game::Font::set_current_size(unsigned character_size) const
{
	FT_Face face = static_cast<FT_Face>(face_);
	FT_UShort current_size = face->size->metrics.x_ppem;

	if (current_size != character_size)
	{
		FT_Error result = FT_Set_Pixel_Sizes(face, 0, character_size);

		if (result == FT_Err_Invalid_Pixel_Size)
		{
			Log::level() = Log::log_error;
			Log::print("Failed to set bitmap font size to %u", character_size);

			std::string str = "Available sizes are: ";

			for (size_t i = 0; i < face->num_fixed_sizes; ++i)
				str += std::to_string(face->available_sizes[i].height) + " ";

			Log::level() = Log::log_info;
			Log::print("%s", str.c_str());
		}

		return result == FT_Err_Ok;
	}

	return true;
}
