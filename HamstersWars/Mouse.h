#pragma once
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Vector2.hpp>
#include <functional>

namespace game
{
	class Game;

	class Mouse : public sf::NonCopyable
	{
	public:
		enum class Buttons : int { left = 0, right, middle, wheel_up, wheel_down };

		static void set_position(const int& x, const int& y);
		static void set_position(const sf::Vector2i& pos);

		static sf::Vector2i get_position();
		static int get_position_x();
		static int get_position_y();

		static void set_cursor(const int& type);

		static bool is_pressed(const Buttons& button);

		static void set_event_on_mouse_move(const std::function<void(const int&, const int&)>& function);
		static void set_action_on_mouse_click(const std::function<void(const Buttons&, const int&, const int&)>& function);
		static void set_action_on_mouse_release(const std::function<void(const Buttons&, const int&, const int&)>& function);
	private:
		Mouse();

		std::function<void(const int&, const int&)> on_mouse_move_;
		std::function<void(const Buttons&, const int&, const int&)> on_mouse_click_;
		std::function<void(const Buttons&, const int&, const int&)> on_mouse_release_;

		sf::Vector2i position_;
		bool keys_[3]{};

		void on_passive_motion(const int& x, const int& y);
		void on_motion(const int& x, const int& y);
		void on_mouse(const int& button, const int& state, const int& x, const int& y);

		static void initialize();

		static Mouse* get_instance();

		friend class Game;
		friend void on_passive_motion(int, int);
		friend void on_motion(int, int);
		friend void on_mouse(int, int, int, int);

	};
}

