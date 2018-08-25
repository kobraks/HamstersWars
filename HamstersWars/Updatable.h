#pragma once

namespace game
{
	class Updatable
	{
	public:
		virtual void update(const float& time_step) = 0;

		virtual ~Updatable() = default;
	};
}

