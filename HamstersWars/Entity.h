#pragma once

#include "Types.h"
#include "Component.h"

#include <map>
#include <typeindex>
#include <string>

namespace game
{
	class Entity
	{
	public:
		Entity();
		Entity(const Entity& entity);
		Entity(Entity&& entity) noexcept;

		~Entity();

		Entity* copy() const;

		std::string get_type() const;
		void set_type(const std::string& type);

		void add_component(std::type_index type_index, component::Component* component);

		template<class T>
		void add_component(component::Component* component)
		{
			add_component(typeid(T), component);
		}

		template<class T>
		T* get_component()
		{
			auto component = components_.find(typeid(T));
			if (component != components_.end())
				return dynamic_cast<T*>(component->second);

			return nullptr;
		}

		template<class T>
		void remove_component()
		{
			auto& component = components_.find(typeid(T));
			if (component != components_.end())
			{
				delete component->second;
			}

		}

	private:
		std::string type_;

		std::map<std::type_index, component::Component*> components_;

	};
}

