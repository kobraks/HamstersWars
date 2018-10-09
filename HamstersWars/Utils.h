#pragma once
#include <Lua/lua.hpp>
#include <Lua/LuaIntf.h>
#include "Vector3D.h"
#include "Vector2D.h"
#include "Color.h"
#include "UnknownTableElementException.h"
#define GET_TYPE_NAME(type) typeid(type).name()

namespace game::utils
{
	typedef LuaIntf::LuaRef LuaTable;

	/**
	 * \brief transformating string to upper case form
	 * \param str string to be transformed
	 * \return a upper case string
	 */
	std::string to_upper_copy(std::string str);
	/**
	 * \brief transformating string to upper case form
	 * \param str string to be transformed
	 * \return str parametr is returned
	 */
	std::string& to_upper(std::string& str);

	/**
	 * \brief transformating string to lower case form
	 * \param str string to be transformed
	 * \return a lower case string
	 */
	std::string to_lower_copy(std::string str);
	/**
	 * \brief transformating string to lower case form
	 * \param str string to be transformed
	 * \return str parameter is returned
	 */
	std::string& to_lower(std::string& str);

	/**
	 * \brief returning gl::Vector3D from lua table if lua table is no vector it will throw exception::UnknownTableElementException()
	 * \param vector a lua table
	 * \return returns a gl::Vector3D of lua table if error exception will be thrown
	 */
	gl::Vector3D get_vector3d(const LuaTable& vector);
	/**
	* \brief returning gl::Vector2D from lua table if lua table is no vector it will throw exception::UnknownTableElementException()
	* \param vector a lua table
	* \return returns a gl::Vector2D of lua table if error exception will be thrown
	*/
	gl::Vector2D get_vector2d(const LuaTable& vector);
	/**
	* \brief returning gl::Color from lua table if lua table is no vector it will throw exception::UnknownTableElementException()
	* \param vector a lua table
	* \return returns a gl::Color of lua table if error exception will be thrown
	*/
	gl::Color get_color(const LuaTable& vector);

	/**
	 * \brief i check if lua table contains an element
	 * \param table lua table
	 * \param key witch key it will look up
	 * \return returns true if it could find an element
	 */
	bool table_contains(const LuaTable& table, const std::string& key);
	/**
	 * \brief i check if lua table contains an element an return it as reference
	 * \param element reference to object if table contains an specifed key
	 * \param table lua table
	 * \param key key witch it will look up for
	 * \return return true if it contains that key
	 */
	bool get_table_if_contains(LuaIntf::LuaRef& element, const LuaTable& table, std::string& key);

	/**
	 * \brief compare a pair of string if they are equals this ignore case size.
	 * \param str1 first string
	 * \param str2 secound string
	 * \return returns true if string are equals
	 */
	bool equals(const std::string& str1, const std::string& str2);

	/**
	 * \brief returing a file path from lua_table
	 * \param path an lua table
	 * \return a path if it is no path it will return an empty string
	 */
	std::string get_path(const LuaIntf::LuaRef& path);
	/**
	 * \brief checking if gived lua table is an path
	 * \param lua_ref lua table
	 * \return return true if lua table is an path
	 */
	bool is_path(const LuaIntf::LuaRef& lua_ref);
	/**
	 * \brief checking if a gived path is an local path
	 * \param path path
	 * \return returns true if its local path
	 */
	bool is_local_path(const std::string& path);

	/**
	 * \brief returns greater values of to vectors
	 * \param left first vector 
	 * \param right secound vector
	 * \return returns greater values of to vectors
	 */
	gl::Vector3D get_greater_values(const gl::Vector3D& left, const gl::Vector3D& right);
	/**
	* \brief returns greater values of to vectors
	* \param left first vector
	* \param right secound vector
	* \return returns greater values of to vectors
	*/
	gl::Vector3D get_lesser_values(const gl::Vector3D& left, const gl::Vector3D& right);

	/**
	* \brief returns greater values of to vectors
	* \param left first vector
	* \param right secound vector
	* \return returns greater values of to vectors
	*/
	gl::Vector2D get_greater_values(const gl::Vector2D& left, const gl::Vector2D& right);
	/**
	* \brief returns greater values of to vectors
	* \param left first vector
	* \param right secound vector
	* \return returns greater values of to vectors
	*/
	gl::Vector2D get_lesser_values(const gl::Vector2D& left, const gl::Vector2D& right);


	std::string& l_trim(std::string &string);
	std::string& r_trim(std::string &string);
	std::string& trim(std::string &string);

	std::string l_trim_copy(std::string string);
	std::string r_trim_copy(std::string string);
	std::string trim_copy(std::string string);

	template<class Type>
	static inline Type convert(const std::string& value)
	{
		Type result;
		std::istringstream stream(value);
		if (stream >> result)
			return result;

		assert(false && "Unable to convert string to value");
		throw std::runtime_error("Unable to convert string to value");
	}

	template<>
	static inline std::string convert<std::string>(const std::string& value)
	{
		return value;
	}

	template<>
	static inline const char* convert<const char*>(const std::string& value)
	{
		return value.c_str();
	}

	template<>
	static inline char* convert<char*>(const std::string& value)
	{
		auto result = new char[value.size() + 1];
		std::copy(value.begin(), value.end(), result);
		result[value.size() + 1] = 0;

		return result;
	}

	template<class Type>
	static inline std::string to_string(const Type& value)
	{
		std::ostringstream stream;

		if (stream << value)
			return stream.str();

		assert(false && "Unable to convert value to string");
		throw std::runtime_error("Unable to convert string to value");
	}

	template<>
	static inline std::string to_string<std::string>(const std::string& value)
	{
		return value;
	}
}

