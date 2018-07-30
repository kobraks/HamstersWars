#include "Mouse.h"
#include <GL/freeglut.h>

#include <Lua/lua.hpp>
#include <Lua/LuaIntf.h>

#include "Script.h"
#include "Log.h"

void set_position_i(const int &x, const int& y)
{
	game::Mouse::set_position(x, y);
}

void set_cursor_style_i(const int& style)
{
	game::Mouse::set_cursor(style);
}

namespace game
{
	void on_passive_motion(int x, int y)
	{
		Mouse::get_instance()->on_passive_motion(x, y);
	}

	void on_motion(int x, int y)
	{
		Mouse::get_instance()->on_motion(x, y);
	}

	void on_mouse(int button, int state, int x, int y)
	{
		Mouse::get_instance()->on_mouse(button, state, x, y);
	}
}

void game::Mouse::set_position(const int& x, const int& y)
{
	glutWarpPointer(x, y);

}

void game::Mouse::set_position(const sf::Vector2i& pos)
{
	set_position(pos.x, pos.y);
}

sf::Vector2i game::Mouse::get_position()
{
	return get_instance()->position_;
}

int game::Mouse::get_position_x()
{
	return get_instance()->position_.x;
}

int game::Mouse::get_position_y()
{
	return get_instance()->position_.y;
}

void game::Mouse::set_cursor(const int& type)
{
	glutSetCursor(type);
}

void game::Mouse::set_cursor(const CursorStyle& style)
{
	set_cursor(style);
}

bool game::Mouse::is_pressed(const Buttons& button)
{
	switch (button)
	{
	case Buttons::middle:
		return get_instance()->keys_pressed_[1];
	case Buttons::left:
		return get_instance()->keys_pressed_[0];
	case Buttons::right:
		return get_instance()->keys_pressed_[2];
	}

	return false;
}

bool game::Mouse::is_down(const Buttons& button)
{
	switch (button)
	{
	case Buttons::middle:
		return get_instance()->keys_[1];
	case Buttons::left:
		return get_instance()->keys_[0];
	case Buttons::right:
		return get_instance()->keys_[2];
	}

	return false;
}

bool game::Mouse::is_up(const Buttons& button)
{
	switch (button)
	{
	case Buttons::middle:
		return get_instance()->keys_up_[1];
	case Buttons::left:
		return get_instance()->keys_up_[0];
	case Buttons::right:
		return get_instance()->keys_up_[2];
	}

	return false;
}

bool game::Mouse::is_left_key_pressed()
{
	return is_pressed(Buttons::left);
}

bool game::Mouse::is_left_key_up()
{
	return is_up(Buttons::left);
}

bool game::Mouse::is_left_key_down()
{
	return is_down(Buttons::left);
}

bool game::Mouse::is_right_key_pressed()
{
	return is_pressed(Buttons::right);
}

bool game::Mouse::is_right_key_up()
{
	return is_up(Buttons::right);
}

bool game::Mouse::is_right_key_down()
{
	return is_down(Buttons::right);
}

bool game::Mouse::is_middle_key_pressed()
{
	return is_pressed(Buttons::middle);
}

bool game::Mouse::is_middle_key_up()
{
	return is_up(Buttons::middle);
}

bool game::Mouse::is_middle_key_down()
{
	return is_down(Buttons::middle);
}

bool game::Mouse::is_wheel_up()
{
	return get_instance()->wheel_up_;
}

bool game::Mouse::is_wheel_down()
{
	return get_instance()->wheel_down_;
}

void game::Mouse::set_event_on_mouse_move(const std::function<void(const int&, const int&)>& function)
{
	get_instance()->on_mouse_move_ = function;
}

void game::Mouse::set_action_on_mouse_click(const std::function<void(const game::Mouse::Buttons&, const int&, const int&)>& function)
{
	get_instance()->on_mouse_click_ = function;
}

void game::Mouse::set_action_on_mouse_release(const std::function<void(const game::Mouse::Buttons&, const int&, const int&)>& function)
{
	get_instance()->on_mouse_release_ = function;
}

void game::Mouse::clear_buttons()
{
	auto& keys_up = get_instance()->keys_up_;
	auto& keys_pressed = get_instance()->keys_pressed_;

	for (size_t i = 0; i < 3; ++i)
	{
		keys_up[i] = false;
		keys_pressed[i] = false;
	}
}

void game::Mouse::clear_pressed_buttons()
{
	auto& buttons = get_instance()->keys_pressed_;

	for (auto& button : buttons)
	{
		button = false;
	}
}

void game::Mouse::clear_up_buttons()
{
	auto& buttons = get_instance()->keys_pressed_;

	for (auto& button : buttons)
	{
		button = false;
	}
}


game::Mouse::Mouse()
{
	for (size_t i = 0; i < 3; ++i)
	{
		keys_[i] = false;
		keys_up_[i] = false;
		keys_pressed_[i] = false;
	}
}

void game::Mouse::on_passive_motion(const int& x, const int& y)
{
	position_ = sf::Vector2i(x, y);

	if (on_mouse_move_)
		on_mouse_move_(x, y);
}

void game::Mouse::on_motion(const int& x, const int& y)
{
	position_ = sf::Vector2i(x, y);

	if (on_mouse_move_)
		on_mouse_move_(x, y);
}

void game::Mouse::on_mouse(const int& button, const int& state, const int& x, const int& y)
{
	position_ = sf::Vector2i(x, y);

	Buttons btn;

	if (state == GLUT_DOWN)
	{
		switch (button)
		{
		case GLUT_MIDDLE_BUTTON:
			keys_[1] = true;
			keys_pressed_[1] = true;
			btn = Buttons::middle;
			break;
		case GLUT_LEFT_BUTTON:
			keys_[0] = true;
			keys_pressed_[0] = true;
			btn = Buttons::left;
			break;
		case GLUT_RIGHT_BUTTON:
			keys_[2] = true;
			keys_pressed_[2] = true;
			btn = Buttons::right;
			break;
		case 3:
			btn = Buttons::wheel_up;
			wheel_up_ = true;
			break;
		case 4:
			btn = Buttons::wheel_down;
			wheel_down_ = true;
			break;
		}

		if (on_mouse_click_)
			on_mouse_click_(btn, x, y);
	}
	else if (state == GLUT_UP)
	{
		switch (button)
		{
		case GLUT_MIDDLE_BUTTON:
			keys_[1] = false;
			keys_up_[1] = true;
			btn = Buttons::middle;
			break;
		case GLUT_LEFT_BUTTON:
			keys_[0] = false;
			keys_up_[0] = true;
			btn = Buttons::left;
			break;
		case GLUT_RIGHT_BUTTON:
			keys_[2] = false;
			keys_up_[2] = true;
			btn = Buttons::right;
			break;
		case 3:
			btn = Buttons::wheel_up;
			wheel_up_ = false;
			break;
		case 4:
			btn = Buttons::wheel_down;
			wheel_down_ = false;
			break;
		}

		if (on_mouse_release_)
			on_mouse_release_(btn, x, y);
	}
}

void game::Mouse::initialize()
{
	glutMouseFunc(game::on_mouse);
	glutPassiveMotionFunc(game::on_passive_motion);
	glutMotionFunc(game::on_motion);

	lua::Script::register_class<Mouse>(get_instance());
}

game::Mouse* game::Mouse::get_instance()
{
	static Mouse mouse;
	return &mouse;
}

void game::Mouse::register_class(LuaIntf::LuaBinding& binding) const
{
	binding.
		addFunction("set_mouse_position", &set_position_i, LUA_ARGS(const int&, const int&)).

		addFunction("get_mouse_position_y", &get_position_y).
		addFunction("get_mouse_position_x", &get_position_x).

		addFunction("is_left_mouse_button_pressed", &is_left_key_pressed).
		addFunction("is_left_mouse_button_up", &is_left_key_up).
		addFunction("is_left_mouse_button_down", &is_left_key_down).

		addFunction("is_right_mouse_button_pressed", &is_right_key_pressed).
		addFunction("is_right_mouse_button_up", &is_right_key_up).
		addFunction("is_right_mouse_button_down", &is_right_key_down).

		addFunction("is_middle_mouse_button_pressed", &is_middle_key_pressed).
		addFunction("is_middle_mouse_button_up", &is_middle_key_up).
		addFunction("is_middle_mouse_button_down", &is_middle_key_down).

		addFunction("set_cursor_style", &set_cursor_style_i);
}
