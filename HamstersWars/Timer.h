#pragma once

#include <functional>

#include "Action.h"
#include "LuaAction.h"
#include "Register.h"

#define TIMER_PARAMETERS float

namespace game
{
	class Timer : public lua::Register
	{
	public:
		typedef std::function<void(TIMER_PARAMETERS)> action;
		Timer() = default;
		explicit Timer(const float& time);

		float get_delay() const;
		void set_delay(const float& time);

		bool is_running() const;
		bool is_paused() const;

		void start(const bool& start = true);
		void pause(const bool& pause = true);
		void stop(const bool& stop = true);

		void update(const float& time_step);
		void on_timer();

		void add_action_on_update(const action& action);
		void add_lua_action_on_update(const LuaIntf::LuaRef& function);
	protected:
		void register_class(LuaIntf::LuaBinding& binding) const override;
	private:
		bool run_ = false;
		bool pause_ = false;
		float delta_time_ = 0;
		float delay_ = 0;

		Action<TIMER_PARAMETERS> actions_;
		LuaAction<TIMER_PARAMETERS> lua_actions_;
	};

}
