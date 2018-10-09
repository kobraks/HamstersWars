#pragma once
#include <memory>
#include "Model.h"
#include "Program.h"

namespace game
{
	class Entity;

	typedef unsigned char key_t;

	typedef unsigned int uint;
	typedef unsigned char uchar;
	typedef unsigned short ushort;

	// 8 bits integer types
	typedef signed   char int8;     ///< Standard 8 bit signed integer
	typedef unsigned char uint8;    ///< Standard 8 bit unsigned integer

	// 16 bits integer types
	typedef signed   short int16;   ///< Standard 16 bit signed integer
	typedef unsigned short uint16;  ///< Standard 16 bit unsigned integer

	// 32 bits integer types
	typedef signed   int int32;     ///< Standard 32 bit signed integer
	typedef unsigned int uint32;    ///< Standard 32 bit unsigned integer

	// 64 bits integer types
	typedef signed   __int64 int64;    ///< Standard 64 bit signed integer
	typedef unsigned __int64 uint64;    ///< Standard 64 bit unsigned integer

	typedef std::string property_id_type;
	typedef std::string system_id_type;
	typedef uint32 entity_id_type;
	typedef uint32 layer_id_type;

	typedef Entity* entity_pointer_type;
}
