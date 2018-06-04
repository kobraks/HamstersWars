#pragma once
#include <functional>
#include "Model.h"
#include "Program.h"

namespace game
{
	class Drawable;

	typedef unsigned char key_t;
	typedef unsigned int id_t;

	typedef unsigned int uint;
	typedef unsigned char uchar;
	typedef unsigned short ushort;

	using shader_behavior = std::function<void(gl::Program&, const Drawable*)>;
}
