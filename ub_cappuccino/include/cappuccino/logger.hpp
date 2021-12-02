#pragma once
#include "config.h"

// utils.hpp requires logger.hpp

#include <string>
#include <vector>
#include <mutex>
#include <glm/glm.hpp>

#define LOG_A1 const std::string& message
#define LOG_A2 const std::vector<std::string>& messages
#define LOG_A3 const std::string& file, int line, const std::string& message
#define LOG_A4 const std::string& file, int line, const std::vector<std::string>& messages

#define LOG_A1G uint8_t _type; std::string _message;
#define LOG_A2G uint8_t _type; std::vector<std::string> _messages;
#define LOG_A3G uint8_t _type; std::string _file; int _line; std::string _message;
#define LOG_A4G uint8_t _type; std::string _file; int _line; std::vector<std::string> _messages;

#define LOG_A1N ._message = message
#define LOG_A2N ._messages = messages
#define LOG_A3N ._file = file, ._line = line, ._message = message
#define LOG_A4N ._file = file, ._line = line, ._messages = messages

#define LOG_A1P g._message
#define LOG_A2P g._messages
#define LOG_A3P g._file, g._line, g._message
#define LOG_A4P g._file, g._line, g._messages

namespace cap
{
///
class logger
{
public:
	///
	struct buffer
	{
	private:
		///
		uint16_t m_recorded_max;

		///
		bool m_destruction_flush;

		///
		bool m_heap_buffer;
	public:
		struct group_1 { LOG_A1G };
		struct group_2 { LOG_A2G };
		struct group_3 { LOG_A3G };
		struct group_4 { LOG_A4G };

		group_1* m_recorded_1;
		group_2* m_recorded_2;
		group_3* m_recorded_3;
		group_4* m_recorded_4;

		uint16_t m_recorded_1_count;
		uint16_t m_recorded_2_count;
		uint16_t m_recorded_3_count;
		uint16_t m_recorded_4_count;

		explicit buffer(bool destruction_flush, bool heap_buffer = false);
		~buffer();

		///
		void flush();

		///
		void reserve(size_t size);

		///
		[[nodiscard]] bool has_recorded_information() const;

		///
		void setup_push();

#define LOG_LC  info
#define LOG_LCI static_cast<uint8_t>(1)

		void LOG_LC(LOG_A1)
		{ setup_push(); m_recorded_1[m_recorded_1_count++] = { ._type = LOG_LCI, LOG_A1N }; }

		void LOG_LC(LOG_A2)
		{ setup_push(); m_recorded_2[m_recorded_2_count++] = { ._type = LOG_LCI, LOG_A2N }; }

		void LOG_LC(LOG_A3)
		{ setup_push(); m_recorded_3[m_recorded_3_count++] = { ._type = LOG_LCI, LOG_A3N }; }

		void LOG_LC(LOG_A4)
		{ setup_push(); m_recorded_4[m_recorded_4_count++] = { ._type = LOG_LCI, LOG_A4N }; }

#undef LOG_LC
#undef LOG_LCI
#define LOG_LC  warn
#define LOG_LCI static_cast<uint8_t>(2)

		void LOG_LC(LOG_A1)
		{ setup_push(); m_recorded_1[m_recorded_1_count++] = { ._type = LOG_LCI, LOG_A1N }; }

		void LOG_LC(LOG_A2)
		{ setup_push(); m_recorded_2[m_recorded_2_count++] = { ._type = LOG_LCI, LOG_A2N }; }

		void LOG_LC(LOG_A3)
		{ setup_push(); m_recorded_3[m_recorded_3_count++] = { ._type = LOG_LCI, LOG_A3N }; }

		void LOG_LC(LOG_A4)
		{ setup_push(); m_recorded_4[m_recorded_4_count++] = { ._type = LOG_LCI, LOG_A4N }; }

#undef LOG_LC
#undef LOG_LCI
#define LOG_LC  error
#define LOG_LCI static_cast<uint8_t>(3)

		void LOG_LC(LOG_A1)
		{ setup_push(); m_recorded_1[m_recorded_1_count++] = { ._type = LOG_LCI, LOG_A1N }; }

		void LOG_LC(LOG_A2)
		{ setup_push(); m_recorded_2[m_recorded_2_count++] = { ._type = LOG_LCI, LOG_A2N }; }

		void LOG_LC(LOG_A3)
		{ setup_push(); m_recorded_3[m_recorded_3_count++] = { ._type = LOG_LCI, LOG_A3N }; }

		void LOG_LC(LOG_A4)
		{ setup_push(); m_recorded_4[m_recorded_4_count++] = { ._type = LOG_LCI, LOG_A4N }; }

#undef LOG_LC
#undef LOG_LCI
	};

	static void flush_buffer(buffer* b);

#define LOG_LC  info
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
#define LOG_LC  warn
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
#define LOG_LC  error
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
#define LOG_LC  fatal
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
	static std::mutex m_mutex;
};
}