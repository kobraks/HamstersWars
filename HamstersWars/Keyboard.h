#pragma once
#include <SFML/System/NonCopyable.hpp>
#include <functional>
#include "Types.h"
#include "Register.h"
#include <GL/freeglut_std.h>

namespace game
{
	class Game;

	class Keyboard : sf::NonCopyable, lua::Register
	{
	public:
		/*enum SpecialKeys
		{
			no_scepcial = 0,
			shift = GLUT_ACTIVE_SHIFT,
			ctrl = GLUT_ACTIVE_CTRL,
			alt = GLUT_ACTIVE_ALT
		};*/

		enum Keys
		{
			A = 0,
			B,
			C,
			D,
			E,
			F,
			G,
			H,
			I,
			J,
			K,
			L,
			M,
			N,
			O,
			P,
			Q,
			R,
			S,
			T,
			U,
			V,
			W,
			X,
			Y,
			Z,
			NUM1,
			NUM2,
			NUM3,
			NUM4,
			NUM5,
			NUM6,
			NUM7,
			NUM8,
			NUM9,
			NUM0,
			F1,
			F2,
			F3,
			F4,
			F5,
			F6,
			F7,
			F8,
			F9,
			F10,
			F11,
			F12,
			PAGE_UP,
			PAGE_DOWN,
			HOME,
			END,
			INSERT,
			RETURN,
			TAB,
			TILDE,
			COMMA

		};

		typedef std::function<void(const key_t&)> key_pressed_function;
		typedef std::function<void(const key_t&)> key_up_function;

		static int get_special_keys();

		static bool is_shift_pressed();
		static bool is_alt_pressed();
		static bool is_ctrl_pressed();
		static bool is_pressed(const key_t& key);
		static bool is_down(const key_t& key);
		static bool is_up(const key_t& key);

		static void set_event_on_key_press(const key_pressed_function& function);
		static void set_event_on_key_up(const key_up_function& function);

		static void clear_keys();
		static void clear_pressed_keys();
		static void clear_up_keys();

	private:
		Keyboard();

		std::function<void(const key_t&)> on_key_press_;
		std::function<void(const key_t&)> on_key_up_;

		void on_key_press(const key_t& key, const int& x, const int& y);
		void on_key_up(const key_t& key, const int& x, const int& y);

		key_t keys_[256]{};
		key_t up_keys_[256]{};
		key_t pressed_keys_[256]{};
		static Keyboard* get_instance();

		static void initialize();

		friend class Game;
		friend void on_key_press(key_t, int, int);
		friend void on_key_up(key_t key, int x, int y);
	protected:
		void register_class(LuaIntf::LuaBinding& binding) const override;
	};
}
