#pragma once
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>

#include <functional>
#include "Register.h"
#include "Action.h"

#define MOUSE_BUTTON_EVENTS_PARAMETERS const int&, const int&, const int&
#define MOUSE_MOVE_EVENT_PARAMETERS const int&, const int&
#define MOUSE_WHEEL_EVENT_PARAMETERS const float&, const int&, const int&, const int&

namespace game
{
	class Game;

	class Mouse : public sf::NonCopyable, public lua::Register
	{
	public:
		const static int BUTTONS_COUNT = 4;

		typedef std::function<void(MOUSE_BUTTON_EVENTS_PARAMETERS)> button_events;
		typedef std::function<void(MOUSE_MOVE_EVENT_PARAMETERS)> move_event;
		typedef std::function<void(MOUSE_WHEEL_EVENT_PARAMETERS)> wheel_event;

		enum Buttons { left = 0, right = 2, middle = 1, undefined = 3 };
		enum Wheel { wheel_up, wheel_down };

		static void set_position(const int& x, const int& y);
		static void set_position(const sf::Vector2i& pos);

		static sf::Vector2i get_position();
		static int get_position_x();
		static int get_position_y();

		static bool is_pressed(const int& button);
		static bool is_down(const int& button);
		static bool is_up(const int& button);

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

		static void add_action_on_press(const button_events& action);
		static void add_action_on_release(const button_events& action);
		static void add_action_on_move(const move_event& action);
		static void add_action_on_scroll(const wheel_event& action);

		static void set_visable(bool visable = true);
		static void set_grabbed(bool grabbed = true);
	private:
		Mouse();

		Action<MOUSE_MOVE_EVENT_PARAMETERS> on_move_;
		Action<MOUSE_BUTTON_EVENTS_PARAMETERS> on_key_pressed_;
		Action<MOUSE_BUTTON_EVENTS_PARAMETERS> on_key_released_;
		Action<MOUSE_WHEEL_EVENT_PARAMETERS> on_scroll_;

		sf::Vector2i position_;
		bool keys_[BUTTONS_COUNT]{};
		bool keys_up_[BUTTONS_COUNT]{};
		bool keys_pressed_[BUTTONS_COUNT]{};
		bool wheel_up_ = false;
		bool wheel_down_ = false;

		bool in_window_ = true;

		void* window_;

		void on_passive_motion(const int& x, const int& y);
		void on_motion(const int& x, const int& y);
		void on_mouse(const int& button, const int& state, const int& x, const int& y);

		static void set_window(void* window);
		static void initialize();

		static Mouse* get_instance();
		static void parse_event(sf::Event& event);


		friend class Game;

		static Buttons translate(const sf::Mouse::Button& button);
		static Wheel translate(const float& wheel);

		static void clear_buttons();
		static void clear_pressed_buttons();
		static void clear_up_buttons();
	protected:
		void register_class(LuaIntf::LuaBinding& binding) const override;
	};
}

