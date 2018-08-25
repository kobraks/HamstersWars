#include "Mouse.h"

#include <Lua/lua.hpp>
#include <Lua/LuaIntf.h>
#include <SFML/Window.hpp>

#include "Script.h"
#include "Log.h"

#define POSITION(pos) pos.x, pos.y

void set_position_x(const int &x)
{
	auto y = game::Mouse::get_position_y();
	game::Mouse::set_position(x, y);
}

void set_position_y(const int& y)
{
	auto x = game::Mouse::get_position_x();
	game::Mouse::set_position(x, y);
}

void set_position_i(const int &x, const int& y)
{
	game::Mouse::set_position(x, y);
}

void game::Mouse::set_position(const int& x, const int& y)
{
	set_position(sf::Vector2i(x, y));
}

void game::Mouse::set_position(const sf::Vector2i& pos)
{
	auto window = reinterpret_cast<sf::Window*>(get_instance()->window_);

	sf::Mouse::setPosition(pos, *window);
	get_instance()->position_ = pos;
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

bool game::Mouse::is_pressed(const int& button)
{
	auto keys = get_instance()->keys_pressed_;

	switch(button)
	{
	case Buttons::left:
		return keys[0];
	case Buttons::right:
		return keys[2];
	case Buttons::middle:
		return keys[1];
	default:
		assert(false);
	}
}

bool game::Mouse::is_down(const int& button)
{
	auto keys = get_instance()->keys_;

	switch (button)
	{
	case Buttons::left:
		return keys[0];
	case Buttons::right:
		return keys[2];
	case Buttons::middle:
		return keys[1];
	default:
		assert(false);
	}
}

bool game::Mouse::is_up(const int& button)
{
	auto keys = get_instance()->keys_up_;

	switch (button)
	{
	case Buttons::left:
		return keys[0];
	case Buttons::right:
		return keys[2];
	case Buttons::middle:
		return keys[1];
	default:
		assert(false);
	}
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

void game::Mouse::add_action_on_press(const button_events& action)
{
	get_instance()->on_key_pressed_ += action;
}

void game::Mouse::add_action_on_release(const button_events& action)
{
	get_instance()->on_key_released_ += action;
}

void game::Mouse::add_action_on_move(const move_event& action)
{
	get_instance()->on_move_ += action;
}

void game::Mouse::add_action_on_scroll(const wheel_event& action)
{
	get_instance()->on_scroll_ += action;
}

void game::Mouse::set_visable(bool visable)
{
	auto window = reinterpret_cast<sf::Window*>(get_instance()->window_);

	window->setMouseCursorVisible(visable);
}

void game::Mouse::set_grabbed(bool grabbed)
{
	auto window = reinterpret_cast<sf::Window*>(get_instance()->window_);

	window->setMouseCursorGrabbed(grabbed);
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

void game::Mouse::initialize(void* wwindow, const bool& visable, const bool& grab)
{
	lua::Script::register_class<Mouse>(get_instance());

	auto window = reinterpret_cast<sf::Window*>(get_instance()->window_ = wwindow);

	window->setMouseCursorGrabbed(grab);
	window->setMouseCursorVisible(visable);

	add_action_on_press([](const int& button, const int& x, const int& y)
	{
		get_instance()->keys_pressed_[button] = true;
		get_instance()->keys_[button] = true;
	});

	add_action_on_release([](const int& button, const int& x, const int& y)
	{
		get_instance()->keys_up_[button] = true;
		get_instance()->keys_[button] = false;
	});

	add_action_on_scroll([](const float& delta, const int& direction, const int& x, const int& y)
	{
		if (direction == wheel_up)
		{
			get_instance()->wheel_up_ = true;
			get_instance()->wheel_down_ = false;
		}
		else
		{
			get_instance()->wheel_up_ = false;
			get_instance()->wheel_down_ = true;
		}
	});

	add_action_on_move([](const int& x, const int& y)
	{
		get_instance()->position_ = sf::Vector2i(x, y);
	});
}

game::Mouse* game::Mouse::get_instance()
{
	static Mouse mouse;
	return &mouse;
}

void game::Mouse::parse_event(sf::Event& event)
{
	auto instance = get_instance();
	auto& in_window = instance->in_window_;
	auto& on_move = instance->on_move_;
	auto& on_key_pressed = instance->on_key_pressed_;
	auto& on_key_released = instance->on_key_released_;
	auto& on_scroll = instance->on_scroll_;

	auto position = sf::Mouse::getPosition(*(reinterpret_cast<sf::Window*>(get_instance()->window_)));

	switch(event.type)
	{
	case sf::Event::MouseLeft:
		if (in_window)
			in_window = false;
	break;

	case sf::Event::MouseEntered:
		in_window = true;
		break;

	case sf::Event::MouseMoved:
		if(in_window)
			on_move(POSITION(position));
		break;

	case sf::Event::MouseButtonPressed:
		if (in_window)
			on_key_pressed(translate(event.mouseButton.button), POSITION(position));
		break;

	case sf::Event::MouseButtonReleased:
		if (in_window)
			on_key_released(translate(event.mouseButton.button), POSITION(position));
		break;

	case sf::Event::MouseWheelMoved:
	case sf::Event::MouseWheelScrolled:
		if (in_window)
			on_scroll(event.mouseWheelScroll.delta, translate(event.mouseWheelScroll.delta), POSITION(position));
		break;

	default:
		assert(false);
	}
}

game::Mouse::Buttons game::Mouse::translate(const sf::Mouse::Button& button)
{
	switch(button)
	{
	case sf::Mouse::Left:
		return left;
	case sf::Mouse::Right:
		return right;
	case sf::Mouse::Middle:
		return middle;
	default:
		return undefined;
	}
}

game::Mouse::Wheel game::Mouse::translate(const float& wheel)
{
	if (wheel > 0)
		return wheel_up;
	
	return wheel_down;
}

void game::Mouse::register_class(LuaIntf::LuaBinding& binding) const
{
	LOG(LOG_INFO, "Registering mouse functions");

	binding.
		beginModule("mouse").

			beginModule("buttons").
				addConstant("left", left).
				addConstant("right", right).
				addConstant("middle", middle).
				addConstant("undefined", undefined).
			endModule().

			beginModule("position").
				addProperty("x", &get_position_x, &set_position_x).
				addProperty("y", &get_position_y, &set_position_y).
			endModule().

			addFunction("set_position", &set_position_i, LUA_ARGS(const int&, const int&)).
			addFunction("get_position_x", &get_position_x).
			addFunction("get_position_y", &get_position_y).

			addFunction("set_visable", &set_visable, LUA_ARGS(LuaIntf::_def<bool, true>)).
			addFunction("set_grabbed", &set_grabbed, LUA_ARGS(LuaIntf::_def<bool, true>)).

			addFunction("is_left_button_pressed", &is_left_key_pressed).
			addFunction("is_left_button_up", &is_left_key_up).
			addFunction("is_left_button_down", &is_left_key_down).

			addFunction("is_right_button_pressed", &is_right_key_pressed).
			addFunction("is_right_button_up", &is_right_key_up).
			addFunction("is_right_button_down", &is_right_key_down).

			addFunction("is_middle_button_pressed", &is_middle_key_pressed).
			addFunction("is_middle_button_up", &is_middle_key_up).
			addFunction("is_middle_button_down", &is_middle_key_down).

			addFunction("is_button_pressed", &is_pressed, LUA_ARGS(const int&)).
			addFunction("is_button_up", &is_up, LUA_ARGS(const int&)).
			addFunction("is_button_down", &is_down, LUA_ARGS(const int&)).

			addFunction("is_wheel_down", &is_wheel_down).
			addFunction("is_wheel_up", &is_wheel_up).
		endModule();
}
