#include "puyo/common/log.hpp"

#include <cinttypes>
#include <cstring>
#include <cstdarg>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <array>
#include <sstream>

using namespace puyo;

namespace
{
	using write_func_ptr = void(*)(const log::content &, log::loc);
	using stream_getter_ptr = std::ostream &(*)();

	constexpr const char error_message[] = "<log error>";

	constexpr const std::array<const char *, 5> levels =
	{
		"debug",
		"info",
		"success",
		"warn",
		"error"
	};

	constexpr const char *to_string(log::level lvl)
	{
		return levels[lvl];
	}

	constexpr const std::size_t initial_size = 256UL;
	std::mutex _logmtx;
	std::ofstream _stream;

	class timestamp
	{
		char buff[128];

	public:
		timestamp()
		{
			using namespace std::chrono;
			system_clock::time_point stp(system_clock::now());
			microseconds us = duration_cast<microseconds>(stp.time_since_epoch());
			seconds sec = duration_cast<seconds>(us);
			std::time_t time = sec.count();
			std::tm *tm = std::localtime(&time);
			size_t tm_sz = std::strftime(buff, sizeof(buff), "%T", tm);
			if (!tm_sz)
			{
				*buff = 0;
				return;
			}
		}

		explicit operator bool() const
		{
			return *buff;
		}

		friend std::ostream &operator<<(std::ostream &os, const timestamp &ts);
	};

	std::ostream &operator<<(std::ostream &os, const timestamp &ts)
	{
		return os << ts.buff;
	}

	std::ostream &operator<<(std::ostream &os, puyo::log::loc at)
	{
		std::ios::fmtflags flags(os.flags());
		os << at.file << ":" << std::left << std::setw(3) << at.line;
		os.flags(flags);
		return os;
	}

	std::ostream &operator<<(std::ostream &os, const puyo::log::content &cnt)
	{
		os << cnt.msg;
		return os;
	}

	std::ostream &operator<<(std::ostream &os, puyo::log::level lvl)
	{
		os << to_string(lvl);
		return os;
	}

	void write_single(log::level lvl, const log::content &cnt, log::loc at, std::ostream &os)
	{
		auto ts = timestamp{};

		if (ts && cnt)
			os << timestamp{} << ": " << at << " " << lvl << ": " << cnt << "\n";
		else
			os << error_message << "\n";
	}

	template <typename... Args>
	void write_multiple(log::level lvl, const log::content &cnt, log::loc at, Args &...streams)
	{
		std::ostringstream oss;
		write_single(lvl, cnt, at, oss);
		std::string str = oss.str();
		((streams << str), ...);
	}

	template <log::level lvl, auto &os, auto &...other>
	void write_impl(const log::content &cnt, log::loc at)
	{
		if constexpr (!sizeof...(other))
			write_single(lvl, cnt, at, os);
		else
			write_multiple(lvl, cnt, at, os, other...);
	}

	template <auto &os>
	std::ostream &getter_impl()
	{
		return os;
	}

	void do_nothing(const log::content &, log::loc) {}

	std::array<std::pair<write_func_ptr, stream_getter_ptr>, 5> funcs =
	{
		std::pair{ do_nothing, getter_impl<_stream> },
		std::pair{ do_nothing, getter_impl<_stream> },
		std::pair{ do_nothing, getter_impl<_stream> },
		std::pair{ do_nothing, getter_impl<_stream> },
		std::pair{ do_nothing, getter_impl<_stream> }
	};

	template <auto &os, log::level... lvl>
	void set_funcs()
	{
		((funcs[lvl] = { write_impl<lvl, os>, getter_impl<os> }), ...);
	}

	std::string error_opening_file(const std::string &file)
	{
		std::string msg("Error opening file ");
		msg.append(file).append(": ").append(std::strerror(errno));
		return msg;
	}

	static_assert(funcs.size() == log::error + 1);
	static_assert(levels.size() == log::error + 1);
}

namespace puyo
{
	log::loc::operator bool() const
	{
		return file && line;
	}

	void log::content::_init(const char *fmt, ...)
	{
		msg.resize(initial_size);

		va_list args, args_copy;
		va_start(args, fmt);
		va_copy(args_copy, args);
		int retval = std::vsnprintf(msg.data(), msg.size() + 1, fmt, args);
		va_end(args);

		std::size_t size = static_cast<std::size_t>(retval);
		if (retval < 0)
			msg.resize(0);
		else if (size > msg.size())
		{
			msg.resize(size);
			if (std::vsnprintf(msg.data(), msg.size() + 1, fmt, args_copy) < 0)
				msg.resize(0);
		}
		else if (size < msg.size())
			msg.resize(size);
		va_end(args_copy);
	}

	log::content::operator bool() const
	{
		return !msg.empty();
	}

	void log::init(bool quiet, const std::string &path)
	{
		static std::once_flag oflag;
		try
		{
			std::call_once(oflag, [](bool quiet, const std::string &path)
				{
					if (quiet)
						set_funcs<std::cerr, error>();
					else if (path.empty())
					{
						set_funcs<std::cout, debug, info, success>();
						set_funcs<std::cerr, warning, error>();
					}
					else
					{
						_stream.open(path);
						if (!_stream)
							throw std::runtime_error(error_opening_file(path));
						set_funcs<_stream, debug, info, success, warning, error>();
					}
				}, quiet, path);
		}
		catch (...)
		{
			throw;
		}
	}

	std::mutex &log::mutex()
	{
		return _logmtx;
	}

	std::ostream &log::stream(level lvl)
	{
		return (*funcs[lvl].second)();
	}

	void log::write(level lvl, const content &cnt, loc at)
	{
		std::scoped_lock lock(_logmtx);
		(*funcs[lvl].first)(cnt, at);
	}
}
