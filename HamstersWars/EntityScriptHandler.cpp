#include "EntityScriptHandler.h"



game::script::EntityScriptHandler::EntityScriptHandler(Entity* entity)
{
}


game::script::EntityScriptHandler::~EntityScriptHandler()
{
}

float game::script::EntityScriptHandler::get_elapsed_time()
{
	return 1.f;
}

//void game::script::EntityScriptHandler::scale(float x, float y, float z)
//{
//}

void game::script::EntityScriptHandler::rotate(float angle, float x, float y, float z)
{
}

void game::script::EntityScriptHandler::translate(float x, float y, float z)
{
}

void game::script::EntityScriptHandler::scale(glm::vec3)
{
}

void game::script::EntityScriptHandler::destroy()
{
}
