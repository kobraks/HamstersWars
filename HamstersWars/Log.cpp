#include "Log.h"
#include <cstdarg>
#include <cstdio>
#include <algorithm>
#include <string>

LogLevel::TLogLevel LogLevel::from_string(const std::string& string)
{
	std::string str = string;

	std::transform(str.begin(), str.end(), str.begin(), ::toupper);
	if (str.find("ERROR") != std::string::npos)
		return log_error;
	if (str.find("WARNING") != std::string::npos)
		return log_warning;
	if (str.find("INFO") != std::string::npos)
		return log_info;
	if (str.find("DEBUG") != std::string::npos)
		return log_debug;
	if (str.find("DEBUG1") != std::string::npos)
		return log_debug1;
	if (str.find("DEBUG2") != std::string::npos)
		return log_debug2;
	if (str.find("DEBUG3") != std::string::npos)
		return log_debug3;
	if (str.find("DEBUG4") != std::string::npos)
		return log_debug4;
	return no_log;
}

std::string LogLevel::to_string(const TLogLevel& level)
{
	static const char* const buffer[] = {
		"no_log", "error", "warning", "info", "debug", "debug1", "debug2", "debug3", "debug4"
	};
	return buffer[static_cast<int>(level)];
}

std::ostream& LogLevel::operator<<(std::ostream& stream, const TLogLevel& level)
{
	stream << to_string(level);
	return stream;
}

FILE*& Output2File::stream()
{
	static FILE* stream = stderr;
	return stream;
}

void Output2File::output(const std::string& msg)
{
	auto stream = Output2File::stream();
	if (!stream)
		return;
	fprintf(stream, "%s", msg.c_str());
	fflush(stream);
}