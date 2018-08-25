#include "Timer.h"

#include "Log.h"

#define ADD_FUNCTION(X) addFunction(#X, &Timer::X)
#define ADD_FUNCTION_PARAMS(X, ...) addFunction(#X, &Timer::X, LUA_ARGS(##__VA_ARGS__))
#define ADD_PROPERTY(X, GETTER, SETTER) addProperty(#X, GETTER, SETTER)

game::Timer::Timer(const float& time) : delay_(time)
{
	start();
}

float game::Timer::get_delay() const
{
	return delay_;	
}

void game::Timer::set_delay(const float& time)
{
	delay_ = time;
}

bool game::Timer::is_running() const
{
	return run_;
}

bool game::Timer::is_paused() const
{
	return pause_;
}

void game::Timer::start(const bool& start)
{
	run_ = start;
}

void game::Timer::pause(const bool& pause)
{
	pause_ = pause;
}

void game::Timer::stop(const bool& stop)
{
	run_ = !stop;
}

void game::Timer::update(const float& time_step)
{
	if (run_ && !pause_)
	{
		delta_time_ += time_step;

		if (delta_time_ >= delay_)
		{
			run_ = false;
			LOG(LOG_DEBUG, "on_timer");
			on_timer();
		}
	}
}

void game::Timer::on_timer()
{
	actions_(delay_);
	lua_actions_(delay_);
}

void game::Timer::add_action_on_update(const action& action)
{
	actions_ += action;
}

void game::Timer::add_lua_action_on_update(const LuaIntf::LuaRef& function)
{
	lua_actions_ += function;
}

void game::Timer::register_class(LuaIntf::LuaBinding& binding) const
{
	LOG(LOG_INFO, "Registering Timer");

	using namespace LuaIntf;

	binding.
		beginClass<Timer>("Timer").
			addConstructor(LUA_ARGS(float)).
			ADD_FUNCTION_PARAMS(start, _def<bool, true>).
			ADD_FUNCTION_PARAMS(pause, _def<bool, true>).
			ADD_FUNCTION_PARAMS(stop, _def<bool, true>).
			ADD_PROPERTY(delay, &Timer::get_delay, &Timer::set_delay).
			ADD_FUNCTION(is_paused).
			ADD_FUNCTION(get_delay).
		endClass();
}
