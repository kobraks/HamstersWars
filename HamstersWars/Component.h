#pragma once
#include <memory>
#include <SFML/System.hpp>

namespace game {
	class Entity;
}

namespace game
{
	namespace component
	{
		class Component : public sf::NonCopyable
		{
		public:
			explicit Component(std::shared_ptr<Entity> owner);
			virtual ~Component() = default;

			virtual Component* copy() const = 0;

			std::shared_ptr<Entity> get_owner() const;
			void set_owner(std::shared_ptr<Entity> entity);
		private:
			std::shared_ptr<Entity> owner_;
		};
	}
}
