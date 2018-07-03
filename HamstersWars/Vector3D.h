#pragma once
#include <glm/glm.hpp>
#include <SFML/System/Vector3.hpp>

namespace gl
{
	class Vector3D
	{
	public:
		struct
		{
			float x, y, z;
		};

		Vector3D() : Vector3D(0.f, 0.f, 0.f) {}
		Vector3D(const float& x, const float& y, const float& z) : x(x), y(y), z(z) {}
		Vector3D(const sf::Vector3f& vector) : Vector3D(vector.x, vector.y, vector.z) {}
		Vector3D(const glm::vec3& vector) : Vector3D(vector.x, vector.y, vector.z) {}


		static float cross(const Vector3D& left, const Vector3D& right)
		{
			return (left.x * right.y) - (left.y * right.x);
		}

		Vector3D cross(const Vector3D& vector) const
		{
			return glm::cross(glm::vec3(*this), glm::vec3(vector));
		}

		float dot(const Vector3D& vector) const
		{
			return glm::dot(glm::vec3(*this), glm::vec3(vector));
		}

		float lenght() const
		{
			return glm::length(glm::vec3(*this));
		}

		template<class T>
		void add(const T& number)
		{
			x += number;
			y += number;
			z += number;
		}

		void add(const Vector3D& vector)
		{
			x += vector.x;
			y += vector.y;
			z += vector.z;
		}

		template<class T>
		void subtract(const T& number)
		{
			x -= number;
			y -= number;
			z -= number;
		}

		void subtract(const Vector3D& vector)
		{
			x -= vector.x;
			y -= vector.y;
			z -= vector.z;
		}

		template<class T>
		void multiplay(const T& number)
		{
			x *= number;
			y *= number;
			z *= number;
		}

		template<class T>
		void divide(const T& number)
		{
			x /= number;
			y /= number;
			z /= number;
		}

		operator sf::Vector3f() const
		{
			return sf::Vector3f(x, y, z);
		}

		operator glm::vec3() const
		{
			return glm::vec3(x, y, z);
		}

		Vector3D& operator+(const Vector3D& vector)
		{
			add(vector);
			return *this;
		}

		Vector3D& operator-(const Vector3D& vector)
		{
			add(vector);
			return *this;
		}

		Vector3D& operator=(const glm::vec3& vector)
		{
			x = vector.x;
			y = vector.y;
			z = vector.z;

			return *this;
		}

		Vector3D& operator=(const sf::Vector3f& vector)
		{
			x = vector.x;
			y = vector.y;
			z = vector.z;

			return *this;
		}

		template<class T>
		friend Vector3D operator+(const Vector3D& left, const T& number)
		{
			auto result = left;
			result.add(number);

			return result;
		}

		friend Vector3D operator+(const Vector3D& left, const Vector3D& right)
		{
			Vector3D result = left;
			result.add(right);

			return result;
		}

		template<class T>
		friend Vector3D operator-(const Vector3D& left, const T& number)
		{
			auto result = left;
			result.subtract(number);

			return result;
		}

		friend Vector3D operator-(const Vector3D& left, const Vector3D& right)
		{
			Vector3D result = left;
			result.subtract(right);

			return result;
		}

		template<class T>
		friend Vector3D operator*(const Vector3D& left, const T& right)
		{
			Vector3D result = left;
			result.multiplay(right);

			return result;
		}

		template<class T>
		friend Vector3D operator/(const Vector3D& left, const T& right)
		{
			Vector3D result = left;
			result.divide(right);

			return result;
		}

		friend bool operator==(const Vector3D& left, const Vector3D& right)
		{
			return left.x == right.x && left.y == right.y && left.z == right.z;
		}

		friend bool operator!=(const Vector3D& left, const Vector3D& right)
		{
			return !(left.x == right.x && left.y == right.y && left.z == right.z);
		}
	};
}
