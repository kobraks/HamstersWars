#pragma once
#include <string>
#include <ostream>
#include <sstream>
#include <mutex>
#include <cstdarg>
#include <chrono>
#include <ctime>
#include <cstdio>

namespace LogLevel
{
	enum TLogLevel : int
	{
		no_log = 0,
		error,
		warning,
		info,
		debug,
		debug1,
		debug2,
		debug3,
		debug4
	};

	TLogLevel from_string(const std::string& string);
	std::string to_string(const TLogLevel& level);

	std::ostream& operator<<(std::ostream& stream, const TLogLevel& level);
};

#pragma region log_level

#define NO_LOG LogLevel::no_log
#define LOG_ERROR LogLevel::error
#define LOG_WARNING LogLevel::warning
#define LOG_INFO LogLevel::info
#define LOG_DEBUG LogLevel::debug
#define LOG_DEBUG1 LogLevel::debug1
#define LOG_DEBUG2 LogLevel::debug2
#define LOG_DEBUG3 LogLevel::debug3
#define LOG_DEBUG4 LogLevel::debug4

#define LOG_MAX_LEVEL LOG_DEBUG4

#pragma endregion

#define __FORCE_LOG_TO_CONSOLE

class Output2File
{
public:
	static FILE*& stream();
	static void output(const std::string& msg);
};

template <class Output>
class Log
{
public:
	Log() = default;
	Log(const Log&) = delete;
	Log(Log&&) = delete;
	Log& operator=(const Log&) = delete;
	Log& operator=(Log&&) = delete;

	~Log();

	static LogLevel::TLogLevel& reporting_level();

	void print(const int& log_level, const char* format, ...);
protected:
	std::ostringstream os_;
	static inline std::mutex mutex_ = std::mutex();

	std::ostringstream& get(const LogLevel::TLogLevel& level = LOG_INFO);
private:

	static std::string get_string(const char* format, va_list args)
	{
		va_list arg_copy;
		va_copy(arg_copy, args);

		const auto lenght = vsnprintf(nullptr, 0, format, args);

		if (lenght <= 0)
			return "";

		const auto string = new char[lenght + 1];
		vsnprintf(string, lenght + 1, format, arg_copy);
		std::string result = string;

		delete[] string;
		return result;
	}

	static std::string get_time()
	{
		auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

		char buf[26];

		ctime_s(buf, 26, &time);
		auto i = std::find(buf, buf + 26, '\n');
		*i = ' ';
		return buf;
	}
};

template <class Output>
Log<Output>::~Log()
{
	os_ << std::endl;
	Output::output(os_.str());

#ifdef __FORCE_LOG_TO_CONSOLE
	fprintf(stderr, "%s", os_.str().c_str());
#endif
}

template <class Output>
std::ostringstream& Log<Output>::get(const LogLevel::TLogLevel& level)
{
	os_ << "- " << get_time();
	os_ << " " << level << ": ";
	os_ << std::string(level > LogLevel::debug ? level - LogLevel::debug : 0, '\t');
	return os_;
}

template <class Output>
LogLevel::TLogLevel& Log<Output>::reporting_level()
{
	static LogLevel::TLogLevel reporting_level = LogLevel::debug4;
	return reporting_level;
}

template <class Output>
void Log<Output>::print(const int& log_level, const char* format, ...)
{
	std::lock_guard<std::mutex> guard(mutex_);
	va_list args;
	va_start(args, format);
	auto text = get_string(format, args);
	va_end(args);

	get(static_cast<LogLevel::TLogLevel>(log_level)) << text;
}

class FileLog : public Log<Output2File>
{
};

#define LOG(log_level, ...) \
	if (log_level == NO_LOG || log_level > LOG_MAX_LEVEL); \
	else if (log_level > FileLog::reporting_level() || !Output2File::stream()); \
	else FileLog().print(log_level, ##__VA_ARGS__);