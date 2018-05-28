#pragma once
#include <string>
#include <ostream>

class Log
{
public:
	enum TLogLevel : int
	{
		no_log = 0,
		log_error,
		log_warning,
		log_info,
		log_debug,
		log_debug1,
		log_debug2,
		log_debug3,
		log_debug4
	};

	Log(Log&) = delete;

	static TLogLevel& max_level();

	static void write_error(const std::string& message, const std::string& error_message = "");

	static TLogLevel& level();
	static void print(const char* format, ...);

	static std::ostream& stream();

	static TLogLevel log_level_from_string(const std::string& string);
	static std::string log_level_to_string(const TLogLevel& log_level);

	//static void clear();
private:
	Log() = default;

	//static void print(const std::string& message, const int& log_level);
	static unsigned long int log_line();
	static void add_log(const char* format, ...);
	static std::string get_string(const char* format, va_list args);
	static std::string get_time();
};
