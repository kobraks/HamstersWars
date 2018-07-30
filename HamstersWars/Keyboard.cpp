#include "Keyboard.h"
#include <GL/freeglut.h>

#include <Lua/lua.hpp>
#include <Lua/LuaIntf.h>

#include "Script.h"
#include "Log.h"

namespace game
{
	 translate(key_t key)


	void on_key_press(key_t key, int x, int y)
	{
		Keyboard::get_instance()->on_key_press(key, x, y);
	}

	void on_key_up(key_t key, int x, int y)
	{
		Keyboard::get_instance()->on_key_up(key, x, y);
	}
}


int game::Keyboard::get_special_keys()
{
	int keys = glutGetModifiers();

	int result = 0;

	if (keys & shift)
		result |= shift;
	if (keys & alt)
		result |= alt;
	if (keys & ctrl)
		result |= ctrl;

	return result;
}

bool game::Keyboard::is_shift_pressed()
{
	return get_special_keys() & shift;
}

bool game::Keyboard::is_alt_pressed()
{
	return get_special_keys() & alt;
}

bool game::Keyboard::is_ctrl_pressed()
{
	return get_special_keys() & ctrl;
}

bool game::Keyboard::is_pressed(const key_t& key)
{
	return get_instance()->pressed_keys_[key];
}

bool game::Keyboard::is_down(const key_t& key)
{
	return get_instance()->keys_[key];
}

bool game::Keyboard::is_up(const key_t& key)
{
	return get_instance()->up_keys_[key];
}

void game::Keyboard::set_event_on_key_press(const key_pressed_function& function)
{
	get_instance()->on_key_press_ = function;
}

void game::Keyboard::set_event_on_key_up(const key_up_function& function)
{
	get_instance()->on_key_up_ = function;
}

void game::Keyboard::clear_keys()
{
	auto& pressed_keys = get_instance()->pressed_keys_;
	auto& up_keys = get_instance()->up_keys_;

	for (size_t i = 0; i < 256; ++i)
	{
		pressed_keys[i] = false;
		up_keys[i] = false;
	}
}

void game::Keyboard::clear_pressed_keys()
{
	auto &keys = get_instance()->pressed_keys_;

	std::fill(keys, keys + 256, false);
}

void game::Keyboard::clear_up_keys()
{
	auto &keys = get_instance()->up_keys_;

	std::fill(keys, keys + 256, false);
}

game::Keyboard::Keyboard()
{
	for (size_t i = 0; i < 256; ++i)
	{
		keys_[i] = false;
		up_keys_[i] = false;
		pressed_keys_[i] = false;
	}
}

void game::Keyboard::on_key_press(const key_t& key, const int& x, const int& y)
{
	if (!keys_[key])
	{
		keys_[key] = true;
		pressed_keys_[key] = true;

		if (on_key_press_)
			on_key_press_(key);
	}
}

void game::Keyboard::on_key_up(const key_t& key, const int& x, const int& y)
{
	keys_[key] = false;
	up_keys_[key] = true;
	if (on_key_up_)
		on_key_up_(key);
}


game::Keyboard* game::Keyboard::get_instance()
{
	static Keyboard instance;
	return &instance;
}

void game::Keyboard::initialize()
{
	auto handler = glutGetWindow();

	glutKeyboardFunc(game::on_key_press);
	glutKeyboardUpFunc(game::on_key_up);

	lua::Script::register_class<Keyboard>(get_instance());
}

void game::Keyboard::register_class(LuaIntf::LuaBinding& binding) const
{
	Log::level() = Log::log_info;
	Log::print("Register keyboard functions");

	binding.beginModule("keyboard").
		addFunction("is_key_pressed", &is_pressed).
		addFunction("is_key_up", &is_up).
		addFunction("is_key_down", &is_down).
		addFunction("is_shift_pressed", &is_shift_pressed).
		addFunction("is_alt_pressed", &is_alt_pressed).
		addFunction("is_ctrl_pressed", &is_ctrl_pressed);
}
