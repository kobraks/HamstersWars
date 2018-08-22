#include "Keyboard.h"

#include <SFML/Window.hpp>

#include <Lua/lua.hpp>
#include <Lua/LuaIntf.h>

#include "Script.h"
#include "Log.h"

#define ASSERT(key) assert(key != sf::Keyboard::Unknown && key < sf::Keyboard::KeyCount)

bool game::Keyboard::is_pressed(const int& key)
{
	ASSERT(key);

	return get_instance()->pressed_keys_[key];
}

bool game::Keyboard::is_down(const int& key)
{
	ASSERT(key);

	return get_instance()->keys_[key];
}

bool game::Keyboard::is_up(const int& key)
{
	ASSERT(key);

	return get_instance()->up_keys_[key];
}

void game::Keyboard::add_action_on_key_released(const key_up_action& on_release)
{
	get_instance()->on_key_released_ += on_release;
}

void game::Keyboard::add_action_on_key_pressed(const key_pressed_action& on_pressed)
{
	get_instance()->on_key_pressed_ += on_pressed;
}

void game::Keyboard::clear_keys()
{
	auto& pressed_keys = get_instance()->pressed_keys_;
	auto& up_keys = get_instance()->up_keys_;

	for (size_t i = 0; i < sf::Keyboard::KeyCount; ++i)
	{
		pressed_keys[i] = false;
		up_keys[i] = false;
	}
}

void game::Keyboard::clear_pressed_keys()
{
	auto &keys = get_instance()->pressed_keys_;

	std::fill(keys, keys + sf::Keyboard::KeyCount, false);
}

void game::Keyboard::clear_up_keys()
{
	auto &keys = get_instance()->up_keys_;

	std::fill(keys, keys + sf::Keyboard::KeyCount, false);
}

game::Keyboard::Keyboard(): window_(nullptr)
{
	for (size_t i = 0; i < sf::Keyboard::KeyCount; ++i)
	{
		keys_[i] = false;
		up_keys_[i] = false;
		pressed_keys_[i] = false;
	}
}

game::Keyboard* game::Keyboard::get_instance()
{
	static Keyboard instance;
	return &instance;
}

void game::Keyboard::initialize(void* wwindow)
{
	auto window = reinterpret_cast<sf::Window*>(get_instance()->window_ = wwindow);

	lua::Script::register_class<Keyboard>(get_instance());

	add_action_on_key_pressed([](const int& button)
	{
		if (button != sf::Keyboard::Unknown)
		{
			get_instance()->keys_[button] = true;
			get_instance()->pressed_keys_[button] = true;
		}
	});

	add_action_on_key_released([](const int& button)
	{
		if (button != sf::Keyboard::Unknown)
		{
			get_instance()->keys_[button] = false;
			get_instance()->up_keys_[button] = true;
		}
	});
}

void game::Keyboard::parse_event(const sf::Event& event)
{
	switch(event.type)
	{
	case sf::Event::KeyPressed:
		get_instance()->on_key_pressed_(event.key.code);
		break;
	case sf::Event::KeyReleased:
		get_instance()->on_key_released_(event.key.code);
		break;
	default:
		assert(false);
	}
}

void game::Keyboard::register_class(LuaIntf::LuaBinding& binding) const
{
	using namespace sf;

	LOG(LOG_INFO, "Registering keyboard interactions");

	binding.beginModule("keyboard").
#pragma region keys
		beginModule("keys").
#pragma region A-Z
				addConstant("a", sf::Keyboard::Key::A).
				addConstant("b", sf::Keyboard::Key::B).
				addConstant("c", sf::Keyboard::Key::C).
				addConstant("d", sf::Keyboard::Key::D).
				addConstant("e", sf::Keyboard::Key::E).
				addConstant("f", sf::Keyboard::Key::F).
				addConstant("g", sf::Keyboard::Key::G).
				addConstant("h", sf::Keyboard::Key::H).
				addConstant("i", sf::Keyboard::Key::I).
				addConstant("j", sf::Keyboard::Key::J).
				addConstant("k", sf::Keyboard::Key::K).
				addConstant("l", sf::Keyboard::Key::L).
				addConstant("m", sf::Keyboard::Key::M).
				addConstant("n", sf::Keyboard::Key::N).
				addConstant("o", sf::Keyboard::Key::O).
				addConstant("p", sf::Keyboard::Key::P).
				addConstant("r", sf::Keyboard::Key::R).
				addConstant("s", sf::Keyboard::Key::S).
				addConstant("t", sf::Keyboard::Key::T).
				addConstant("u", sf::Keyboard::Key::U).
				addConstant("w", sf::Keyboard::Key::W).
				addConstant("z", sf::Keyboard::Key::Z).
				addConstant("x", sf::Keyboard::Key::X).
				addConstant("y", sf::Keyboard::Key::Y).
				addConstant("q", sf::Keyboard::Key::Q).
				addConstant("V", sf::Keyboard::Key::V).
#pragma endregion

#pragma region num
				addConstant("num0", sf::Keyboard::Key::Num0).
				addConstant("num1", sf::Keyboard::Key::Num1).
				addConstant("num2", sf::Keyboard::Key::Num2).
				addConstant("num3", sf::Keyboard::Key::Num3).
				addConstant("num4", sf::Keyboard::Key::Num4).
				addConstant("num5", sf::Keyboard::Key::Num5).
				addConstant("num6", sf::Keyboard::Key::Num6).
				addConstant("num7", sf::Keyboard::Key::Num7).
				addConstant("num8", sf::Keyboard::Key::Num8).
				addConstant("num9", sf::Keyboard::Key::Num9).
#pragma endregion

#pragma region numpad
				addConstant("numpad0", sf::Keyboard::Key::Numpad0).
				addConstant("numpad1", sf::Keyboard::Key::Numpad1).
				addConstant("numpad2", sf::Keyboard::Key::Numpad2).
				addConstant("numpad3", sf::Keyboard::Key::Numpad3).
				addConstant("numpad4", sf::Keyboard::Key::Numpad4).
				addConstant("numpad5", sf::Keyboard::Key::Numpad5).
				addConstant("numpad6", sf::Keyboard::Key::Numpad6).
				addConstant("numpad7", sf::Keyboard::Key::Numpad7).
				addConstant("numpad8", sf::Keyboard::Key::Numpad8).
				addConstant("numpad9", sf::Keyboard::Key::Numpad9).
#pragma endregion

#pragma region f
				addConstant("F1", sf::Keyboard::Key::F1).
				addConstant("F2", sf::Keyboard::Key::F2).
				addConstant("F3", sf::Keyboard::Key::F3).
				addConstant("F4", sf::Keyboard::Key::F4).
				addConstant("F5", sf::Keyboard::Key::F5).
				addConstant("F6", sf::Keyboard::Key::F6).
				addConstant("F7", sf::Keyboard::Key::F7).
				addConstant("F8", sf::Keyboard::Key::F8).
				addConstant("F9", sf::Keyboard::Key::F9).
				addConstant("F10", sf::Keyboard::Key::F10).
				addConstant("F11", sf::Keyboard::Key::F11).
				addConstant("F12", sf::Keyboard::Key::F12).
				addConstant("F13", sf::Keyboard::Key::F13).
				addConstant("F14", sf::Keyboard::Key::F14).
				addConstant("F15", sf::Keyboard::Key::F15).
#pragma endregion

#pragma region other
				addConstant("rcontrol", sf::Keyboard::Key::RControl).
				addConstant("ralt", sf::Keyboard::Key::RAlt).
				addConstant("rshift", sf::Keyboard::Key::RShift).
				addConstant("rsystem", sf::Keyboard::Key::RSystem).

				addConstant("tab", sf::Keyboard::Key::Tab).

				addConstant("lcontrol", sf::Keyboard::Key::LControl).
				addConstant("lalt", sf::Keyboard::Key::LAlt).
				addConstant("lshift", sf::Keyboard::Key::LShift).
				addConstant("lsystem", sf::Keyboard::Key::LSystem).

				addConstant("enter", sf::Keyboard::Key::Return).
				addConstant("return", sf::Keyboard::Key::Return).

				addConstant("menu", sf::Keyboard::Key::Menu).
				addConstant("lbracket", sf::Keyboard::Key::LBracket).
				addConstant("rbracket", sf::Keyboard::Key::RBracket).
				addConstant("semicolon", sf::Keyboard::Key::SemiColon).
				addConstant("comma", sf::Keyboard::Key::Comma).
				addConstant("period", sf::Keyboard::Key::Period).
				addConstant("quote", sf::Keyboard::Key::Quote).
				addConstant("slash", sf::Keyboard::Key::Slash).
				addConstant("backslash", sf::Keyboard::Key::BackSlash).
				addConstant("tilde", sf::Keyboard::Key::Tilde).
				addConstant("equal", sf::Keyboard::Key::Equal).
				addConstant("dash", sf::Keyboard::Key::Dash).
				addConstant("space", sf::Keyboard::Key::Space).
				addConstant("backspace", sf::Keyboard::Key::BackSpace).
				addConstant("pageup", sf::Keyboard::Key::PageUp).
				addConstant("pagedown", sf::Keyboard::Key::PageDown).
				addConstant("end", sf::Keyboard::Key::End).
				addConstant("home", sf::Keyboard::Key::Home).
				addConstant("insert", sf::Keyboard::Key::Insert).
				addConstant("delete", sf::Keyboard::Key::Delete).
				addConstant("add", sf::Keyboard::Key::Add).
				addConstant("subtract", sf::Keyboard::Key::Subtract).
				addConstant("multiply", sf::Keyboard::Key::Multiply).
				addConstant("divide", sf::Keyboard::Key::Divide).
				addConstant("left", sf::Keyboard::Key::Left).
				addConstant("up", sf::Keyboard::Key::Up).
				addConstant("down", sf::Keyboard::Key::Down).
				addConstant("right", sf::Keyboard::Key::Right).
				addConstant("pause", sf::Keyboard::Key::Pause).
				addConstant("escpe", sf::Keyboard::Key::Escape).
#pragma endregion
				
			endModule().
#pragma endregion


			addFunction("is_key_pressed", &is_pressed).
			addFunction("is_key_up", &is_up).
			addFunction("is_key_down", &is_down).
		endModule();
}
