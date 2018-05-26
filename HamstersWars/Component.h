#pragma once

namespace game {
	class Entity;
}

namespace game
{
	namespace component
	{
		class Component
		{
		public:
			explicit Component(Entity* owner);
			virtual ~Component() = default;

			virtual Component* copy() const = 0;

			Entity* get_owner() const;
			void set_owner(Entity* entity);
		private:
			Entity* owner_;
		};
	}
}
