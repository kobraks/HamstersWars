#pragma once
#include <string>
#include <sstream>
#include <mutex>


#include "Types.h"

namespace game::log
{
	namespace level
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
	}

	class OutputStream
	{
	public:
		OutputStream() = default;
		OutputStream(const OutputStream&) = delete;
		OutputStream& operator=(const OutputStream&) = delete;

		virtual ~OutputStream();

		virtual uint64 write(const void* data, const uint64& size) = 0;
		virtual uint64 seek(const uint64& position) = 0;
		virtual uint64 tell() = 0;
		virtual void flush() = 0;

		virtual void write(const std::string& msg);
		virtual void write(const char* format, ...);
	protected:
		void set_stream(FILE*& stream);
		std::FILE*& get_stream();

	private:
		std::FILE* stream_ = nullptr;

	};

	class Output2File : public OutputStream
	{
	public:
		Output2File() = default;
		explicit Output2File(const std::string& file_name);
		~Output2File() override;

		void open(const std::string& file_name);
		uint64 write(const void* data, const uint64& size) override;
		uint64 seek(const uint64& position) override;
		uint64 tell() override;
		void flush() override;
	};

	class Output2Console : public OutputStream
	{
	public:
		explicit Output2Console(FILE* stream);
		Output2Console() = default;

		~Output2Console() override;

		void set_stream(FILE* stream);

		uint64 write(const void* data, const uint64& size) override;
		uint64 seek(const uint64& position) override;
		uint64 tell() override;
		void flush() override;
	};

	class Log
	{
	public:
		Log(const Log&) = delete;
		Log& operator=(const Log&) = delete;

		Log() = default;
		~Log();

		static void add_output(class OutputStream* out);
		static level::TLogLevel& reporting_level();

		void print(const int& log_level, const char* format, ...);
	protected:
		std::ostringstream out_;
		static inline std::mutex mutex_;

		std::ostringstream& get(const level::TLogLevel& level = level::info);
	private:
		static std::string get_time();

		class OutContainer
		{
		public:
			static inline std::vector<OutputStream*> outs;
			static void print_out(const std::string& msg);

			~OutContainer();
		};
	};
}

#pragma region level_defines

#define NO_LOG game::log::level::TLogLevel::no_log
#define LOG_ERROR game::log::level::TLogLevel::error
#define LOG_WARNING game::log::level::TLogLevel::warning
#define LOG_INFO game::log::level::TLogLevel::info
#define LOG_DEBUG game::log::level::TLogLevel::debug
#define LOG_DEBUG1 game::log::level::TLogLevel::debug1
#define LOG_DEBUG2 game::log::level::TLogLevel::debug2
#define LOG_DEBUG3 game::log::level::TLogLevel::debug3
#define LOG_DEBUG4 game::log::level::TLogLevel::debug4

#define LOG_MAX_LEVEL LOG_DEBUG4

#pragma endregion

#define LOG(log_level, ...) \
	if (log_level == NO_LOG || log_level > LOG_MAX_LEVEL); \
	else if (log_level > game::log::Log::reporting_level()); \
	else game::log::Log().print(log_level, ##__VA_ARGS__);

#define LOG_P(log_level, x) \
	if (log_level == NO_LOG || log_level > LOG_MAX_LEVEL); \
	else if (log_level > game::log::Log::reporting_level()); \
	else game::log::Log().print(log_level, #x);