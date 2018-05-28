#include "Log.h"
#include <cstdarg>
#include <iostream>
#include <chrono>
#include <sstream>
#include <ctime>
#include <cstdio>
#include <algorithm>

#include "BadLogStreamException.h"

void Log::write_error(const std::string& message, const std::string& error_message)
{
	if (max_level() >= log_error)
	{
		add_log("%u: %s [%s]: %s\n", log_line(), get_time().c_str(), log_level_to_string(log_error).c_str(), message.c_str());
		if (!error_message.empty())
			add_log("error message: %s \n", error_message.c_str());
	}
}

//void Log::print(const std::string& message, const int& log_level)
//{
//	if (max_level() >= log_level)
//		add_log("%u: [%s]: %s\n", log_line(), log_level_to_string(static_cast<TLogLevel>(log_level)).c_str(), message.c_str());
//}

Log::TLogLevel& Log::level()
{
	static TLogLevel level = log_info;
	return level;
}

void Log::print(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	auto text = get_string(format, args);
	va_end(args);

	if (max_level() >= level())
	{
		add_log("%u: %s [%s]: %s\n", log_line(), get_time().c_str(), log_level_to_string(static_cast<TLogLevel>(level())).c_str(), text.c_str());
		stream().flush();
	}
}

std::ostream& Log::stream()
{
	static std::ostream ostream(std::cout.rdbuf());
	return ostream;
}

Log::TLogLevel Log::log_level_from_string(const std::string& string)
{
	std::string str = string;

	std::transform(str.begin(), str.end(), str.begin(), ::toupper);

	if (str == "LOG_ERROR")
		return TLogLevel::log_error;
	if (str == "LOG_WARNING")
		return log_warning;
	if (str == "LOG_INFO")
		return log_info;
	if (str == "LOG_DEBUG")
		return log_debug;
	if (str == "LOG_DEBUG1")
		return log_debug1;
	if (str == "LOG_DEBUG2")
		return log_debug2;
	if (str == "LOG_DEBUG3")
		return log_debug3;
	if (str == "LOG_DEBUG4")
		return log_debug4;
	return no_log;
}

unsigned long Log::log_line()
{
	static unsigned long int line = 0;
	line++;
	return line;
}

std::string Log::log_level_to_string(const TLogLevel& log_level)
{
	static const char* const buffer[] = { "NOLOG", "ERROR", "WARNING", "INFO", "DEBUG", "DEBUG1", "DEBUG2", "DEBUG3", "DEBUG4" };
	return buffer[static_cast<int>(log_level)];
}

void Log::add_log(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	auto text = get_string(format, args);
	va_end(args);

	printf("%s", text.c_str());

	if (stream().good())
		stream() << text;
	else
		throw game::exception::BadLogStreamException();
}

std::string Log::get_string(const char* format, va_list args)
{
	va_list arg_copy;
	va_copy(arg_copy, args);

	auto lenght = vsnprintf(nullptr, 0, format, args);

	if (lenght <= 0)
		return "";

	auto string = new char[lenght + 1];
	vsnprintf(string, lenght + 1, format, arg_copy);
	std::string result = string;

	delete[] string;
	return result;
}

std::string Log::get_time()
{
	auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	char buf[26];

	ctime_s(buf, 26, &time);
	auto i = std::find(buf, buf + 26, '\n');
	*i = ' ';
	return buf;
}

Log::TLogLevel& Log::max_level()
{
	static TLogLevel level = log_debug4;
	return level;
}
