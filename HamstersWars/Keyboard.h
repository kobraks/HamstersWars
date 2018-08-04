#pragma once
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>

#include <functional>

#include "Types.h"
#include "Register.h"
#include "Action.h"

#define KEYBOARD_KEY_PRESSED_PARAMETERS const int&
#define KEYBOARD_KEY_RELEASED_PARAMETERS const int&

namespace game
{
	class Game;

	class Keyboard : sf::NonCopyable, lua::Register
	{
	public:
		typedef std::function<void(KEYBOARD_KEY_PRESSED_PARAMETERS)> key_pressed_action;
		typedef std::function<void(KEYBOARD_KEY_RELEASED_PARAMETERS)> key_up_action;

		static bool is_pressed(const int& key);
		static bool is_down(const int& key);
		static bool is_up(const int& key);

		static void add_action_on_key_released(const key_up_action& on_release);
		static void add_action_on_key_pressed(const key_pressed_action& on_pressed);
	private:
		Keyboard();

		Action<KEYBOARD_KEY_PRESSED_PARAMETERS> on_key_pressed_;
		Action<KEYBOARD_KEY_RELEASED_PARAMETERS> on_key_released_;

		bool keys_[sf::Keyboard::KeyCount]{};
		bool up_keys_[sf::Keyboard::KeyCount]{};
		bool pressed_keys_[sf::Keyboard::KeyCount]{};

		void* window_;

		static Keyboard* get_instance();

		static void initialize(void* window);
		static void parse_event(const sf::Event& event);
		static void clear_keys();
		static void clear_pressed_keys();
		static void clear_up_keys();

		friend class Game;
	protected:
		void register_class(LuaIntf::LuaBinding& binding) const override;
	};
}
