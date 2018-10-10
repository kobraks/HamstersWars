#include "Log.h"
#include "stdafx.h"

namespace game::log
{
	namespace level
	{
		TLogLevel from_string(const std::string& string)
		{
			std::string str = string;

			std::transform(str.begin(), str.end(), str.begin(), ::toupper);
			if (str.find("ERROR") != std::string::npos)
				return error;
			if (str.find("WARNING") != std::string::npos)
				return warning;
			if (str.find("INFO") != std::string::npos)
				return info;
			if (str.find("DEBUG") != std::string::npos)
				return debug;
			if (str.find("DEBUG1") != std::string::npos)
				return debug1;
			if (str.find("DEBUG2") != std::string::npos)
				return debug2;
			if (str.find("DEBUG3") != std::string::npos)
				return debug3;
			if (str.find("DEBUG4") != std::string::npos)
				return debug4;
			return no_log;
		}

		std::string to_string(const TLogLevel& level)
		{
			static const char* const buffer[] = {
				"NOLOG", "ERROR", "WARNING", "INFO", "DEBUG", "DEBUG1", "DEBUG2", "DEBUG3", "DEBUG4"
			};
			return buffer[static_cast<int>(level)];
		}

		std::ostream& operator<<(std::ostream& stream, const TLogLevel& level)
		{
			stream << to_string(level);
			return stream;
		}
	}

	OutputStream::~OutputStream() = default;

	void OutputStream::write(const std::string& msg)
	{
		write(reinterpret_cast<const void*>(msg.data()), msg.size());
	}

	void OutputStream::write(const char* format, ...)
	{
		//std::lock_guard<std::mutex> guard(mutex);
		va_list args;
		va_start(args, format);
		auto msg = utils::get_string(format, args);
		va_end(args);

		write(msg);
	}

	void OutputStream::set_stream(FILE*& stream)
	{
		stream_ = stream;
	}

	std::FILE*& OutputStream::get_stream()
	{
		return stream_;
	}

	Output2File::Output2File(const std::string& file_name)
	{
		open(file_name);
	}

	Output2File::~Output2File()
	{
		auto stream = get_stream();
		if (stream)
		{
			std::fflush(stream);
			std::fclose(stream);
			delete stream;
		}
	}

	void Output2File::open(const std::string& file_name)
	{
		auto& stream = get_stream();
		if (stream)
		{
			std::fflush(stream);
			std::fclose(stream);
		}

		errno_t error;
		if (error = fopen_s(&stream, file_name.c_str(), "w"))
			throw std::exception(std::string(std::string("Unable to open file") + file_name).c_str());

		set_stream(stream);
	}

	uint64 Output2File::write(const void* data, const uint64& size)
	{
		auto stream = get_stream();
		if (stream)
			return std::fwrite(data, 1, static_cast<std::size_t>(size), stream);

		return -1;
	}

	uint64 Output2File::seek(const uint64& position)
	{
		auto stream = get_stream();

		if (stream)
		{
			if (std::fseek(stream, static_cast<long>(position), SEEK_SET))
				return -1;

			return tell();
		}

		return -1;
	}

	uint64 Output2File::tell()
	{
		auto stream = get_stream();
		if (stream)
			return std::ftell(stream);

		return -1;
	}

	void Output2File::flush()
	{
		auto stream = get_stream();
		std::fflush(stream);

	}

	Output2Console::Output2Console(FILE* stream)
	{
		set_stream(stream);
	}

	Output2Console::~Output2Console()
	{
		auto stream = get_stream();
		if (stream)
		{
			std::fflush(stream);
			std::fclose(stream);
			delete stream;
		}
	}

	void Output2Console::set_stream(FILE* stream)
	{
		OutputStream::set_stream(stream);
	}

	uint64 Output2Console::write(const void* data, const uint64& size)
	{
		auto stream = get_stream();
		if (stream)
			return std::fwrite(data, 1, static_cast<std::size_t>(size), stream);

		return -1;
	}

	uint64 Output2Console::seek(const uint64& position)
	{
		auto stream = get_stream();

		if (stream)
		{
			if (std::fseek(stream, static_cast<long>(position), SEEK_SET))
				return -1;

			return tell();
		}

		return -1;
	}

	uint64 Output2Console::tell()
	{
		auto stream = get_stream();
		if (stream)
			return std::ftell(stream);

		return -1;
	}

	void Output2Console::flush()
	{
		auto stream = get_stream();
		std::fflush(stream);
	}

	Log::~Log()
	{
		out_ << std::endl;
		OutContainer::print_out(out_.str());
	}

	void Log::add_output(OutputStream* out)
	{
		OutContainer::outs.push_back(out);
	}

	level::TLogLevel& Log::reporting_level()
	{
		static auto reporting_level = level::debug4;
		return reporting_level;
	}

	void Log::print(const int& log_level, const char* format, ...)
	{
		std::lock_guard<std::mutex> guard(mutex_);
		va_list args;
		va_start(args, format);
		auto string = utils::get_string(format, args);
		va_end(args);

		get(static_cast<level::TLogLevel>(log_level)) << string;
	}

	std::ostringstream& Log::get(const level::TLogLevel& level)
	{
		out_ << "- " << get_time();
		out_ << " " << level << ": ";
		out_ << std::string(level > level::debug ? level - level::debug : 0, '\t');
		return out_;
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

	void Log::OutContainer::print_out(const std::string& msg)
	{
		for (auto out : outs)
		{
			out->write(msg);
			out->flush();
		}
	}

	Log::OutContainer::~OutContainer()
	{
		for (auto out : outs)
			delete out;
	}
}
