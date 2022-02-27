#pragma once
#include "config.h"

// utils.hpp requires logger.hpp

#include <glm/glm.hpp>
#include <mutex>
#include <string>
#include <vector>

#define LOG_DEBUG(message) latte::logger::info(latte::logger::level::SYSTEM, __FILE__, __LINE__, message)
#define LOG_DEBUG_VEC2(vec) \
	latte::logger::info(      \
		latte::logger::level::SYSTEM, __FILE__, __LINE__, std::to_string(vec.x) + ", " + std::to_string(vec.y))
#define LOG_DEBUG_VEC3(vec)         \
	latte::logger::info(              \
		latte::logger::level::SYSTEM, \
		__FILE__,                   \
		__LINE__,                   \
		std::to_string(vec.x) + ", " + std::to_string(vec.y) + ", " + std::to_string(vec.z))

#define LOG_A1 latte::logger::level level, const std::string &message
#define LOG_A2 latte::logger::level level, const std::vector<std::string>&messages
#define LOG_A3 latte::logger::level level, const std::string &file, int line, const std::string &message
#define LOG_A4 latte::logger::level level, const std::string &file, int line, const std::vector<std::string>&messages

#define LOG_A1G                \
	uint8_t _type;             \
	latte::logger::level _level; \
	std::string _message;
#define LOG_A2G                \
	uint8_t _type;             \
	latte::logger::level _level; \
	std::vector<std::string> _messages;
#define LOG_A3G                \
	uint8_t _type;             \
	latte::logger::level _level; \
	std::string _file;         \
	int _line;                 \
	std::string _message;
#define LOG_A4G                \
	uint8_t _type;             \
	latte::logger::level _level; \
	std::string _file;         \
	int _line;                 \
	std::vector<std::string> _messages;

#define LOG_A1N ._level = level, ._message = std::string(message)
#define LOG_A2N ._level = level, ._messages = std::vector<std::string>(messages)
#define LOG_A3N ._level = level, ._file = file, ._line = line, ._message = std::string(message)
#define LOG_A4N ._level = level, ._file = file, ._line = line, ._messages = std::vector<std::string>(messages)

#define LOG_A1P g._level, g._message
#define LOG_A2P g._level, g._messages
#define LOG_A3P g._level, g._file, g._line, g._message
#define LOG_A4P g._level, g._file, g._line, g._messages

namespace latte
{
///
class logger
{
public:
	///
	enum class level
	{
		SYSTEM,
		EDITOR,
		RUNTIME
	};

	/// Internal data structure used to override one static logger and feed another instead.
	struct stream
	{
		stream()
			: log_update(false)
		{}

		///
		bool log_update;

		///
		std::mutex mutex;

		///
		std::vector<std::string> cached_logs;
	};

	/// Buffer to record messages in one go, minimizing overhead in bursts.
	struct buffer
	{
	private:
		///
		bool m_destruction_flush;

	public:
		struct group_1
		{
			LOG_A1G
		};
		struct group_2
		{
			LOG_A2G
		};
		struct group_3
		{
			LOG_A3G
		};
		struct group_4
		{
			LOG_A4G
		};

		std::vector<group_1> m_recorded_1;
		std::vector<group_2> m_recorded_2;
		std::vector<group_3> m_recorded_3;
		std::vector<group_4> m_recorded_4;

		explicit buffer(bool destruction_flush);
		~buffer();

		///
		void flush();

		///
		void reserve(size_t size);

		///
		bool has_recorded_information() const;

		///
		void setup_push();

#define LOG_LC	info
#define LOG_LCI static_cast<uint8_t>(1)

		void LOG_LC(LOG_A1)
		{
			setup_push();
			m_recorded_1.push_back({._type = LOG_LCI, LOG_A1N});
		}

		void LOG_LC(LOG_A2)
		{
			setup_push();
			m_recorded_2.push_back({._type = LOG_LCI, LOG_A2N});
		}

		void LOG_LC(LOG_A3)
		{
			setup_push();
			m_recorded_3.push_back({._type = LOG_LCI, LOG_A3N});
		}

		void LOG_LC(LOG_A4)
		{
			setup_push();
			m_recorded_4.push_back({._type = LOG_LCI, LOG_A4N});
		}

#undef LOG_LC
#undef LOG_LCI
#define LOG_LC	warn
#define LOG_LCI static_cast<uint8_t>(2)

		void LOG_LC(LOG_A1)
		{
			setup_push();
			m_recorded_1.push_back({._type = LOG_LCI, LOG_A1N});
		}

		void LOG_LC(LOG_A2)
		{
			setup_push();
			m_recorded_2.push_back({._type = LOG_LCI, LOG_A2N});
		}

		void LOG_LC(LOG_A3)
		{
			setup_push();
			m_recorded_3.push_back({._type = LOG_LCI, LOG_A3N});
		}

		void LOG_LC(LOG_A4)
		{
			setup_push();
			m_recorded_4.push_back({._type = LOG_LCI, LOG_A4N});
		}

#undef LOG_LC
#undef LOG_LCI
#define LOG_LC	error
#define LOG_LCI static_cast<uint8_t>(3)

		void LOG_LC(LOG_A1)
		{
			setup_push();
			m_recorded_1.push_back({._type = LOG_LCI, LOG_A1N});
		}

		void LOG_LC(LOG_A2)
		{
			setup_push();
			m_recorded_2.push_back({._type = LOG_LCI, LOG_A2N});
		}

		void LOG_LC(LOG_A3)
		{
			setup_push();
			m_recorded_3.push_back({._type = LOG_LCI, LOG_A3N});
		}

		void LOG_LC(LOG_A4)
		{
			setup_push();
			m_recorded_4.push_back({._type = LOG_LCI, LOG_A4N});
		}

#undef LOG_LC
#undef LOG_LCI
	};

	///
	static void initialize();

	/// Retrieves the stream from the logger to use in other loggers.
	static stream* get_stream_system();

	/// Retrieves the stream from the logger to use in other loggers.
	static stream* get_stream_editor();

	/// Retrieves the stream from the logger to use in other loggers.
	static stream* get_stream_runtime();

	/// Overriding basic functions of logger and use the provided stream instead.
	static void set_stream_system(stream* stream_system);

	/// Overriding basic functions of logger and use the provided stream instead.
	static void set_stream_editor(stream* stream_editor);

	/// Overriding basic functions of logger and use the provided stream instead.
	static void set_stream_runtime(stream* stream_runtime);

	///
	static void flush_buffer(buffer* b);

#define LOG_LC	info
#define LOG_LCR info_raw

	static void LOG_LC(LOG_A1);
	static void LOG_LC(LOG_A2);
	static void LOG_LC(LOG_A3);
	static void LOG_LC(LOG_A4);

	static void LOG_LCR(LOG_A1);
	static void LOG_LCR(LOG_A2);
	static void LOG_LCR(LOG_A3);
	static void LOG_LCR(LOG_A4);

#undef LOG_LC
#undef LOG_LCR
#define LOG_LC	warn
#define LOG_LCR warn_raw

	static void LOG_LC(LOG_A1);
	static void LOG_LC(LOG_A2);
	static void LOG_LC(LOG_A3);
	static void LOG_LC(LOG_A4);

	static void LOG_LCR(LOG_A1);
	static void LOG_LCR(LOG_A2);
	static void LOG_LCR(LOG_A3);
	static void LOG_LCR(LOG_A4);

#undef LOG_LC
#undef LOG_LCR
#define LOG_LC	error
#define LOG_LCR error_raw

	static void LOG_LC(LOG_A1);
	static void LOG_LC(LOG_A2);
	static void LOG_LC(LOG_A3);
	static void LOG_LC(LOG_A4);

	static void LOG_LCR(LOG_A1);
	static void LOG_LCR(LOG_A2);
	static void LOG_LCR(LOG_A3);
	static void LOG_LCR(LOG_A4);

#undef LOG_LC
#undef LOG_LCR
#define LOG_LC	fatal
#define LOG_LCR fatal_raw

	static void LOG_LC(LOG_A1);
	static void LOG_LC(LOG_A2);
	static void LOG_LC(LOG_A3);
	static void LOG_LC(LOG_A4);

	static void LOG_LCR(LOG_A1);
	static void LOG_LCR(LOG_A2);
	static void LOG_LCR(LOG_A3);
	static void LOG_LCR(LOG_A4);

#undef LOG_LC
#undef LOG_LCR
private:
	///
	static void internal_fatal(const std::string& error);

	/// Stream for system based logs (vulkan or imgui for example).
	static stream* m_stream_system;

	/// Stream for editor based logs (ub program compiling for example).
	static stream* m_stream_editor;

	/// Stream for runtime based logs (block logging for example).
	static stream* m_stream_runtime;

	///
	static bool m_stream_system_owner;

	///
	static bool m_stream_editor_owner;

	///
	static bool m_stream_runtime_owner;
};
} // namespace cap