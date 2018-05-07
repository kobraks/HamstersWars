#include "Mouse.h"
#include <GL/freeglut.h>

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

bool game::Mouse::is_pressed(const Buttons& button)
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


game::Mouse::Mouse()
{
	for (auto& key : keys_)
		key = false;
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
			btn = Buttons::middle;
			break;
		case GLUT_LEFT_BUTTON:
			keys_[0] = true;
			btn = Buttons::left;
			break;
		case GLUT_RIGHT_BUTTON:
			keys_[2] = true;
			btn = Buttons::right;
			break;
		case 3:
			btn = Buttons::wheel_up;
			break;
		case 4:
			btn = Buttons::wheel_down;
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
			btn = Buttons::middle;
			break;
		case GLUT_LEFT_BUTTON:
			keys_[0] = false;
			btn = Buttons::left;
			break;
		case GLUT_RIGHT_BUTTON:
			keys_[2] = false;
			btn = Buttons::right;
			break;
		case 3:
			btn = Buttons::wheel_up;
			break;
		case 4:
			btn = Buttons::wheel_down;
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
}

game::Mouse* game::Mouse::get_instance()
{
	static Mouse mouse;
	return &mouse;
}
