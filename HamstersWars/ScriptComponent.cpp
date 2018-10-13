#include "stdafx.h"
#include "ScriptComponent.h"
#include "Entity.h"
#include "EntityScriptHandler.h"

namespace game::component
{
	ScriptComponent::ScriptComponent() : Component(SCRIPT_COMPONENT)
	{
		properties.add<LuaIntf::LuaRef>("on_update", LuaIntf::LuaRef());
		properties.add<LuaIntf::LuaRef>("on_copy", LuaIntf::LuaRef());
		properties.add<LuaIntf::LuaRef>("on_create", LuaIntf::LuaRef());
		properties.add<LuaIntf::LuaRef>("on_destroy", LuaIntf::LuaRef());
	}

	ScriptComponent::~ScriptComponent()
	{
		entity_ = nullptr;
		delete handler_;
	}

	void ScriptComponent::parse_table(LuaIntf::LuaRef table)
	{
		for (auto row : table)
		{
			auto key = utils::to_upper_copy(utils::trim_copy(row.key<std::string>()));

			if (key == UPDATE_FUNCTION)
			{
				try
				{
					if (row.value().isFunction())
					{
						LOG(LOG_INFO, "Setting update function");
						properties.set<LuaIntf::LuaRef>("on_update", row.value());
					}
				}
				catch(std::exception& ex)
				{
					LOG(LOG_ERROR, "Unable to set update function %s", ex.what());
				}
			}
			else if (key == CREATE_FUNCTION)
			{
				try
				{
					if (row.value().isFunction())
					{
						LOG(LOG_INFO, "Setting create function");
						properties.set<LuaIntf::LuaRef>("on_create", row.value());
					}
				}
				catch (std::exception& ex)
				{
					LOG(LOG_ERROR, "Unable to set create function %s", ex.what());
				}
			}
			else if (key == COPY_FUNCTION)
			{
				try
				{
					if (row.value().isFunction())
					{
						LOG(LOG_INFO, "Setting copy function");
						properties.set<LuaIntf::LuaRef>("on_copy", row.value());
					}
				}
				catch (std::exception& ex)
				{
					LOG(LOG_ERROR, "Unable to set copy function %s", ex.what());
				}
			}
			else if (key == DESTROY_FUNCTION)
			{
				try
				{
					if (row.value().isFunction())
					{
						LOG(LOG_INFO, "Setting destroy function");
						properties.set<LuaIntf::LuaRef>("on_destroy", row.value());
					}
				}
				catch (std::exception& ex)
				{
					LOG(LOG_ERROR, "Unable to set destroy function %s", ex.what());
				}
			}
			else
			{
				LOG(LOG_WARNING, "Unable to recognize gived table_type key: %s", row.key<std::string>().c_str());
			}
		}
	}

	interfaces::Component* ScriptComponent::clone() const
	{
		auto component = new ScriptComponent();
		component->properties = properties;

		return component;
	}

	void ScriptComponent::update()
	{
		auto fun = properties.get<LuaIntf::LuaRef>("on_update");
		if (fun.isValid() && fun.isFunction())
			if (handler_)
			{
				try
				{
					fun.call(*handler_);
				}
				catch (std::exception& ex)
				{
					LOG(LOG_ERROR, "Lua throws exception on_update %s", ex.what());
				}
			}
	}

	void ScriptComponent::create()
	{
		auto fun = properties.get<LuaIntf::LuaRef>("on_create");
		if (fun.isValid() && fun.isFunction())
			if (handler_)
			{
				try
				{
					fun.call(*handler_);
				}
				catch (std::exception& ex)
				{
					LOG(LOG_ERROR, "Lua throws exception on_create %s", ex.what());
				}
			}
	}

	void ScriptComponent::copy()
	{
		auto fun = properties.get<LuaIntf::LuaRef>("on_copy");
		if (fun.isValid() && fun.isFunction())
			if (handler_)
			{
				try
				{
					fun.call(*handler_);
				}
				catch (std::exception& ex)
				{
					LOG(LOG_ERROR, "Lua throws exception on_copy %s", ex.what());
				}
			}
	}

	void ScriptComponent::destroy()
	{
		auto fun = properties.get<LuaIntf::LuaRef>("on_destroy");
		if (fun.isValid() && fun.isFunction())
			if (handler_)
			{
				try
				{
					fun.call(*handler_);
				}
				catch (std::exception& ex)
				{
					LOG(LOG_ERROR, "Lua throws exception on_destroy %s", ex.what());
				}
			}
	}

	void ScriptComponent::set_entity(entity::Entity* entity)
	{
		entity_ = entity;
		if (handler_)
		{
			delete handler_;
			handler_ = nullptr;
		}
		handler_ = new entity::EntityScriptHandler(entity);
	}
}
