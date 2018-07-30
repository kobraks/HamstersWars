#pragma once
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Vector2.hpp>
#include <functional>
#include "Register.h"
#include <GL/freeglut_std.h>

namespace game
{
	class Game;

	class Mouse : public sf::NonCopyable, public lua::Register
	{
	public:
		enum class Buttons : int { left = 0, right, middle, wheel_up, wheel_down };
		enum class CursorStyle : int
		{
			right_arrow = GLUT_CURSOR_RIGHT_ARROW,
			left_arrow = GLUT_CURSOR_LEFT_ARROW,
			info = GLUT_CURSOR_INFO,
			destroy = GLUT_CURSOR_DESTROY,
			help = GLUT_CURSOR_HELP,
			cycle = GLUT_CURSOR_CYCLE,
			spray = GLUT_CURSOR_SPRAY,
			wait = GLUT_CURSOR_WAIT,
			text = GLUT_CURSOR_TEXT,
			crosshair = GLUT_CURSOR_CROSSHAIR,
			up_down = GLUT_CURSOR_UP_DOWN,
			left_right = GLUT_CURSOR_LEFT_RIGHT,
			top_side = GLUT_CURSOR_TOP_SIDE,
			bottom_side = GLUT_CURSOR_BOTTOM_SIDE,
			left_side = GLUT_CURSOR_LEFT_SIDE,
			right_side = GLUT_CURSOR_RIGHT_SIDE,
			top_left_corner = GLUT_CURSOR_TOP_LEFT_CORNER,
			bottom_right_corner = GLUT_CURSOR_BOTTOM_RIGHT_CORNER,
			bottom_left_corner = GLUT_CURSOR_BOTTOM_LEFT_CORNER,
			full_crosshair = GLUT_CURSOR_FULL_CROSSHAIR,
			none = GLUT_CURSOR_NONE,
			inherit = GLUT_CURSOR_INHERIT
		};

		static void set_position(const int& x, const int& y);
		static void set_position(const sf::Vector2i& pos);

		static sf::Vector2i get_position();
		static int get_position_x();
		static int get_position_y();

		static void set_cursor(const int& type);
		static void set_cursor(const CursorStyle& style);

		static bool is_pressed(const Buttons& button);
		static bool is_down(const Buttons& button);
		static bool is_up(const Buttons& button);

		static bool is_left_key_pressed();
		static bool is_left_key_up();
		static bool is_left_key_down();

		static bool is_right_key_pressed();
		static bool is_right_key_up();
		static bool is_right_key_down();

		static bool is_middle_key_pressed();
		static bool is_middle_key_up();
		static bool is_middle_key_down();

		static bool is_wheel_up();
		static bool is_wheel_down();

		static void set_event_on_mouse_move(const std::function<void(const int&, const int&)>& function);
		static void set_action_on_mouse_click(const std::function<void(const Buttons&, const int&, const int&)>& function);
		static void set_action_on_mouse_release(const std::function<void(const Buttons&, const int&, const int&)>& function);

		static void clear_buttons();
		static void clear_pressed_buttons();
		static void clear_up_buttons();
	private:
		Mouse();

		std::function<void(const int&, const int&)> on_mouse_move_;
		std::function<void(const Buttons&, const int&, const int&)> on_mouse_click_;
		std::function<void(const Buttons&, const int&, const int&)> on_mouse_release_;

		sf::Vector2i position_;
		bool keys_[3]{};
		bool keys_up_[3]{};
		bool keys_pressed_[3]{};
		bool wheel_up_ = false;
		bool wheel_down_ = false;

		void on_passive_motion(const int& x, const int& y);
		void on_motion(const int& x, const int& y);
		void on_mouse(const int& button, const int& state, const int& x, const int& y);

		static void initialize();

		static Mouse* get_instance();

		friend class Game;
		friend void on_passive_motion(int, int);
		friend void on_motion(int, int);
		friend void on_mouse(int, int, int, int);

	protected:
		void register_class(LuaIntf::LuaBinding& binding) const override;
	};
}

