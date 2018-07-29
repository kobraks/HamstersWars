#pragma once

#pragma region exceptions

#define BAD_LOG_STREAM_EXCEPTION ""
#define UNKNOWN_TABLE_ELEMENT_EXCEPTION "Unknown table element"
#define UNABLE_TO_FIND_SPECIFED_TABLE_KEY_EXCEPTION "Unable to find %s key"
#define NOT_FUNCTION_EXCEPTION "Value of %s is not valid function"

#pragma endregion 

#pragma region paths

#define RESOURCES_PATH "../resources/"
#define LUA_SCRIPTS_PATH RESOURCES_PATH"lua_scripts/"
#define MODELS_PATH RESOURCES_PATH"models/"
#define SHADERS_PATH RESOURCES_PATH"shaders/"
#define TEXTURES_PATH RESOURCES_PATH"textures/"

#pragma endregion

#define LOG_FILE "../log.txt"

#pragma region shaders

#define VERTEX_INDEX 0
#define COLOR_INDEX 1
#define TEXCOORD_INDEX 2
#define NORMAL_INDEX 3

#pragma endregion

#define DEFAULT_TEXTURE TEXTURES_PATH"default.png"

#pragma region components

#define COLLIDER_COMPONENT "COLLIDER"
#define GRAPHIC_COMPONENT "GRAPHIC"
#define SCRIPT_COMPONENT "SCRIPT"
#define TRANSFORM_COMPONENT "TRANSFORM"

#pragma endregion

#pragma region script_component_elements

#define UPDATE_FUNCTION "ON_UPDATE"
#define COPY_FUNCTION "ON_COPY"
#define CREATE_FUNCTION "ON_CREATE"
#define DESTROY_FUNCTION "ON_DESTROY"

#pragma endregion