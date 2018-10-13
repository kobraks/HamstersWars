#pragma once
#include "Types.h"
namespace game
{
	namespace interfaces
	{
		class Component;
	}

	class ComponentManager
	{
	public:
		typedef interfaces::Component component;
		typedef std::unordered_map<std::type_index, component*> component_list_type;

		ComponentManager() = default;
		ComponentManager(const ComponentManager& manager);
		ComponentManager& operator=(const ComponentManager& manager);

		~ComponentManager();


		void add(const std::type_index& type_index, component* component);

		template<class Type>
		Type* get()
		{
			auto component = components_.find(typeid(Type));
			if (component != components_.end())
				return dynamic_cast<Type*>(component->second);

			return nullptr;
		}

		void clear();
	private:
		component_list_type components_;

	};

}

