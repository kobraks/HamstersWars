#pragma once
#include <typeinfo>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <string>
#include <cstring>
#include <Windows.h>
#include <memory>
#include <string>
#include <cctype>
#include <locale>
#include <typeinfo>
#include <utility>
#include <deque>
#include <cstdarg>
#include <chrono>
#include <ctime>
#include <cstdio>
#include <algorithm>
#include <regex>
#include <deque>
#include <memory>
#include <stack>
#include <vector>
#include <functional>
#include <typeindex>

///gl classes
#include "Vector3D.h"
#include "Vector2D.h"
#include "Color.h"
#include "Program.h"

#include <SFML/Window.hpp>
#include <Lua/lua.hpp>
#include <Lua/LuaIntf.h>

namespace LuaIntf
{
	LUA_USING_LIST_TYPE(std::vector)
		LUA_USING_MAP_TYPE(std::map)
}

#include "Log.h"
#include "Defines.h"
#include "Types.h"
#include "Stack.h"
#include "Utils.h"
