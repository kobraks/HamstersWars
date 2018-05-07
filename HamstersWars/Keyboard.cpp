#include "Keyboard.h"
#include <GL/freeglut.h>

namespace game
{
	void on_key_press(key_t key, int x, int y)
	{
		Keyboard::get_instance()->on_key_press(key, x, y);
	}

	void on_key_up(key_t key, int x, int y)
	{
		Keyboard::get_instance()->on_key_up(key, x, y);
	}
}



bool game::Keyboard::is_pressed(const key_t& key)
{
	return get_instance()->keys_[key];
}

bool game::Keyboard::is_down(const key_t& key)
{
	return get_instance()->keys_[key];
}

bool game::Keyboard::is_up(const key_t& key)
{
	return !get_instance()->keys_[key];
}

void game::Keyboard::set_event_on_key_press(const std::function<void(const key_t&)>& function)
{
	get_instance()->on_key_press_ = function;
}

void game::Keyboard::set_event_on_key_up(const std::function<void(const key_t&)>& function)
{
	get_instance()->on_key_up_ = function;
}

game::Keyboard::Keyboard()
{
	for (auto& key : keys_)
		key = false;
}

void game::Keyboard::on_key_press(const key_t& key, const int& x, const int& y)
{
	if (!keys_[key])
	{
		if (on_key_press_)
			on_key_press_(key);
		keys_[key] = true;
	}
}

void game::Keyboard::on_key_up(const key_t& key, const int& x, const int& y)
{
	keys_[key] = false;
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
	glutKeyboardFunc(game::on_key_press);
	glutKeyboardUpFunc(game::on_key_up);
}
