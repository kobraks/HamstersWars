#pragma once
#include <SFML/System/NonCopyable.hpp>
#include <functional>
#include "Types.h"

namespace game
{
	class Game;

	class Keyboard : sf::NonCopyable
	{
		static bool is_pressed(const key_t& key);
		static bool is_down(const key_t& key);
		static bool is_up(const key_t& key);

		static void set_event_on_key_press(const std::function<void(const key_t&)>& function);
		static void set_event_on_key_up(const std::function<void(const key_t&)>& function);

	private:
		Keyboard();

		std::function<void(const key_t&)> on_key_press_;
		std::function<void(const key_t&)> on_key_up_;

		void on_key_press(const key_t& key, const int& x, const int& y);
		void on_key_up(const key_t& key, const int& x, const int& y);

		key_t keys_[256]{};
		static Keyboard* get_instance();

		static void initialize();



		friend class Game;
		friend void on_key_press(key_t, int, int);
		friend void on_key_up(key_t key, int x, int y);
	};
}
