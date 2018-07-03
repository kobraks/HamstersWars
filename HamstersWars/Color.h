#pragma once
#include <glm/glm.hpp>

namespace gl
{
	class Color
	{
	public:
		struct
		{
			union
			{
				struct
				{
					float r, g, b, a;
				};

				struct
				{
					float red, green, blue, alpha;
				};
			};
		};


		Color() : Color(0.f, 0.f, 0.f, 0.f) {}
		Color(const glm::vec3& color) : Color(color.r, color.g, color.b){}
		Color(const glm::vec4& color) : Color(color.r, color.g, color.b, color.a){}
		Color(const float& red, const float& green, const float& blue, const float& alpha = 1.f) : r(red), g(green), b(blue), a(alpha) {}
		Color(const unsigned char& red, const unsigned char& green, const unsigned char& blue) : Color(red / 255.f, green / 255.f, blue / 255.f) {}
		Color(const unsigned char& red, const unsigned char& green, const unsigned char& blue, const unsigned char& alpha) : Color(red / 255.f, green / 255.f, blue / 255.f, alpha / 255.f) {}

		void set_color(const glm::vec4& color)
		{
			set_color(color.r, color.g, color.b, color.a);
		}

		void set_color(const glm::vec3& color)
		{
			set_color(color.r, color.g, color.b);
		}

		void set_color(const unsigned char& red, const unsigned char& green, const unsigned char& blue, const unsigned char& alpha = 255)
		{
			set_color(red / 255.f, green / 255.f, blue / 255.f, alpha / 255.f);
		}

		void set_color(const float& red, const float& green, const float& blue, const float& alpha = 1.f)
		{
			r = red;
			g = green;
			b = blue;
			a = alpha;
		}

		Color& operator=(const glm::vec3& color)
		{
			r = color.r;
			g = color.g;
			b = color.b;
			a = 1.f;

			return *this;
		}

		Color& operator=(const glm::vec4& color)
		{
			r = color.r;
			g = color.g;
			b = color.b;
			a = color.a;

			return *this;
		}

		operator glm::vec3() const
		{
			return glm::vec3(r, g, b);
		}

		operator glm::vec4() const
		{
			return glm::vec4(r, g, b, a);
		}

		void set_red(const float& red)
		{
			r = red;
		}

		void set_blue(const float& blue)
		{
			b = blue;
		}

		void set_green(const float& green)
		{
			g = green;
		}

		void set_alpha(const float& alpha)
		{
			a = alpha;
		}

		void set_red(const unsigned char& red)
		{
			r = red / 255.f;
		}

		void set_blue(const unsigned char& blue)
		{
			b = blue / 255.f;
		}

		void set_green(const unsigned char& green)
		{
			g = green / 255.f;
		}

		void set_alpha(const unsigned char& alpha)
		{
			a = alpha / 255.f;
		}

		void set_r(const float& red)
		{
			r = red;
		}

		void set_b(const float& blue)
		{
			b = blue;
		}

		void set_g(const float& green)
		{
			g = green;
		}

		void set_a(const float& alpha)
		{
			a = alpha;
		}

		void set_r(const unsigned char& red)
		{
			r = red / 255.f;
		}

		void set_b(const unsigned char& blue)
		{
			b = blue / 255.f;
		}

		void set_g(const unsigned char& green)
		{
			g = green / 255.f;
		}

		void set_a(const unsigned char& alpha)
		{
			a = alpha / 255.f;
		}
	};
}
