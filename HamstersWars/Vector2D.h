#pragma once
#include <glm/glm.hpp>
#include <SFML/System/Vector2.hpp>

namespace gl
{
	class Vector2D
	{
	public:
		struct
		{
			float x, y;
		};

		Vector2D() : Vector2D(0.f, 0.f) {}
		Vector2D(const float& x, const float& y) : x(x), y(y){}
		explicit Vector2D(const sf::Vector2f& vector) : Vector2D(vector.x, vector.y) {}
		explicit Vector2D(const glm::vec2& vector) : Vector2D(vector.x, vector.y) {}

		static float cross(const Vector2D& left, const Vector2D& right)
		{
			return (left.x * right.y) - (left.y * right.x);
		}

		Vector2D cross(const Vector2D& vector) const
		{
			return Vector2D();
		}

		float dot(const Vector2D& vector) const
		{
			return glm::dot(glm::vec2(*this), glm::vec2(vector));
		}

		float lenght() const
		{
			return glm::length(glm::vec2(*this));
		}

		template<class T>
		void add(const T& number)
		{
			x += number;
			y += number;
		}

		void add(const Vector2D& vector)
		{
			x += vector.x;
			y += vector.y;
		}

		template<class T>
		void subtract(const T& number)
		{
			x -= number;
			y -= number;
		}

		void subtract(const Vector2D& vector)
		{
			x -= vector.x;
			y -= vector.y;
		}

		template<class T>
		void multiplay(const T& number)
		{
			x *= number;
			y *= number;
		}

		template<class T>
		void divide(const T& number)
		{
			x /= number;
			y /= number;
		}

		operator sf::Vector2f() const
		{
			return sf::Vector2f(x, y);
		}

		operator glm::vec2() const
		{
			return glm::vec2(x, y);
		}

		Vector2D& operator=(const sf::Vector2f& vector)
		{
			x = vector.x;
			y = vector.y;

			return *this;
		}

		Vector2D& operator=(const glm::vec2& vector)
		{
			x = vector.x;
			y = vector.y;

			return *this;
		}

		friend Vector2D operator+(const Vector2D& left, const Vector2D& right)
		{
			Vector2D result = left;
			result.subtract(right);

			return result;
		}

		friend Vector2D operator-(const Vector2D& left, const Vector2D& right)
		{
			Vector2D result = left;
			result.subtract(right);

			return result;
		}

		template<class T>
		friend Vector2D operator*(const Vector2D& left, const T& right)
		{
			Vector2D result = left;
			result.multiplay(right);

			return result;
		}

		template<class T>
		friend Vector2D operator/(const Vector2D& left, const T& right)
		{
			Vector2D result = left;
			result.divide(right);

			return result;
		}

		friend bool operator==(const Vector2D& left, const Vector2D& right)
		{
			return left.x == right.x && left.y == right.y;
		}

		friend bool operator!=(const Vector2D& left, const Vector2D& right)
		{
			return !(left.x == right.x && left.y == right.y);
		}
	};

	/*inline glm::vec3& operator=(glm::vec3& left, const Vector2D& right)
	{
		left.x = right.x;
		left.y = right.y;

		return left;
	}*/
}