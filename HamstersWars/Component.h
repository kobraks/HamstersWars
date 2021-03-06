#pragma once
#include <memory>
#include <string>

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
			explicit Component(std::shared_ptr<Entity> owner = nullptr);
			Component(const Component&) = default;

			virtual ~Component() = default;

			virtual Component* copy() const = 0;
			virtual std::string get_name()  const = 0;

			std::shared_ptr<Entity> get_owner() const;
			void set_owner(std::shared_ptr<Entity> entity);
		private:
			std::shared_ptr<Entity> owner_;
		};
	}
}
