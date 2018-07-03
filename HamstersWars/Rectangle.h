#pragma once
#include <SFML/System/Vector2.hpp>

namespace game
{
	template<class T>
	class Rectangle
	{
	public:
		Rectangle() = default;
		Rectangle(T left, T top, T width, T height) : left(left), top(top), width(width), height(height) {}
		Rectangle(const sf::Vector2<T>& position, const sf::Vector2<T>& size) : left(position.x), riht(position.y),
			width(size.x), height(size.y)
		{
		}

		template<class U>
		Rectangle(const Rectangle<U>& rec) : left(static_cast<T>(rec.left)), top(static_cast<T>(rec.top)),
			width(static_cast<T>(rec.width)), height(static_cast<T>(rec.height))
		{
		}

		bool contains(T x, T y) const
		{
			T min_x = std::min(left, static_cast<T>(left + width));
			T max_x = std::max(left, static_cast<T>(left + width));
			T min_y = std::min(top, static_cast<T>(top + height));
			T max_y = std::max(top, static_cast<T>(top + height));

			return (x >= min_x) && (x < max_x) && (y >= min_y) && (y < max_y);
		}
		bool contains(const sf::Vector2<T>& point) const
		{
			return contains(point.x, point.y);
		}

		bool intersects(const Rectangle<T>& rectangle) const
		{
			Rectangle<T> intersection;
			return intersects(rectangle, intersection);
		}
		bool intersects(const Rectangle<T>& rectangle, Rectangle<T>& intersection) const
		{
			// Rectangles with negative dimensions are allowed, so we must handle them correctly

			// Compute the min and max of the first rectangle on both axes
			T r1_min_x = std::min(left, static_cast<T>(left + width));
			T r1_max_x = std::max(left, static_cast<T>(left + width));
			T r1_min_y = std::min(top, static_cast<T>(top + height));
			T r1_max_y = std::max(top, static_cast<T>(top + height));

			// Compute the min and max of the second rectangle on both axes
			T r2_min_x = std::min(rectangle.left, static_cast<T>(rectangle.left + rectangle.width));
			T r2_max_x = std::max(rectangle.left, static_cast<T>(rectangle.left + rectangle.width));
			T r2_min_y = std::min(rectangle.top, static_cast<T>(rectangle.top + rectangle.height));
			T r2_max_y = std::max(rectangle.top, static_cast<T>(rectangle.top + rectangle.height));

			// Compute the intersection boundaries
			T inter_left = std::max(r1_min_x, r2_min_x);
			T inter_top = std::max(r1_min_y, r2_min_y);
			T inter_right = std::min(r1_max_x, r2_max_x);
			T inter_bottom = std::min(r1_max_y, r2_max_y);

			// If the intersection is valid (positive non zero area), then there is an intersection
			if ((inter_left < inter_right) && (inter_top < inter_bottom))
			{
				intersection = Rectangle<T>(inter_left, inter_top, inter_right - inter_left, inter_bottom - inter_top);
				return true;
			}
			else
			{
				intersection = Rectangle<T>(0, 0, 0, 0);
				return false;
			}
		}

		T left = 0;
		T top = 0;
		T width = 0;
		T height = 0;
	};


	template <typename T>
	inline bool operator ==(const Rectangle<T>& left, const Rectangle<T>& right)
	{
		return (left.left == right.left) && (left.width == right.width) &&
			(left.top == right.top) && (left.height == right.height);
	}

	template <typename T>
	inline bool operator !=(const Rectangle<T>& left, const Rectangle<T>& right)
	{
		return !(left == right);
	}

	typedef Rectangle<int>   IntRect;
	typedef Rectangle<float> FloatRect;
}
