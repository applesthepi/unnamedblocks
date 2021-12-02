#include "logger.hpp"

#include <iostream>

#define LOG_BEGIN m_mutex.lock();
#define LOG_END   m_mutex.unlock();

#define LOG_BUFFER_IDX_FAILURE "invalid buffer log _type idx"

#define LOG_EXE1(__UCS) \
std::cout << " [" << (__UCS) << "] " << message << "\n" << std::flush;

#define LOG_EXE2(__UCS)                                      \
for (auto& message : messages)                               \
	std::cout << " [" << (__UCS) << "] " << message << "\n"; \
std::cout << std::flush;

#define LOG_EXE3(__UCS) \
std::cout << "\n [" << (__UCS) << "] [" << file << "] [" << line << "]\n [" << (__UCS) << "] " << message << "\n\n" << std::flush;

#define LOG_EXE4(__UCS)                                                    \
std::cout << "\n [" << (__UCS) << "] [" << file << "] [" << line << "]\n"; \
for (auto& message : messages)                                             \
	std::cout << " [" << (__UCS) << "] " << message << "\n";               \
std::cout << std::flush;

cap::logger::buffer::buffer(bool destruction_flush, bool heap_buffer)
	: m_destruction_flush(destruction_flush)
	, m_heap_buffer(heap_buffer)
	, m_recorded_max(0)
	, m_recorded_1_count(0)
	, m_recorded_2_count(0)
	, m_recorded_3_count(0)
	, m_recorded_4_count(0)
	, m_recorded_1(nullptr)
	, m_recorded_2(nullptr)
	, m_recorded_3(nullptr)
	, m_recorded_4(nullptr)
{
	reserve(10);
}

cap::logger::buffer::~buffer()
{
	if (m_destruction_flush)
		flush();
}

void cap::logger::buffer::flush()
{
	if (has_recorded_information())
		cap::logger::flush_buffer(this);

	m_recorded_1_count = 0;
	m_recorded_2_count = 0;
	m_recorded_3_count = 0;
	m_recorded_4_count = 0;
}

void cap::logger::buffer::reserve(size_t size)
{
	if (m_recorded_max > 0)
	{
		free(m_recorded_1);
		free(m_recorded_2);
		free(m_recorded_3);
		free(m_recorded_4);
	}

	m_recorded_max = static_cast<uint16_t>(size);

	if (m_heap_buffer)
	{
		if (size == 0)
		{
			cap::logger::warn("attempted to reserve heap buffer to 0; reserving to 1 instead");
			size = 1;
		}
		else if (size > 1000000)
		{
			cap::logger::warn("attempted to reserve heap buffer with more then 1 million groups; reserving to 1 million instead");
			size = 1000000;
		}

		m_recorded_1 = reinterpret_cast<group_1*>(malloc(size * sizeof(group_1)));
		m_recorded_2 = reinterpret_cast<group_2*>(malloc(size * sizeof(group_2)));
		m_recorded_3 = reinterpret_cast<group_3*>(malloc(size * sizeof(group_3)));
		m_recorded_4 = reinterpret_cast<group_4*>(malloc(size * sizeof(group_4)));
	}
	else
	{
		if (size == 0)
		{
			cap::logger::warn("attempted to reserve stack buffer to 0; reserving to 1 instead");
			size = 1;
		}
		else if (size > 100)
		{
			cap::logger::warn("attempted to reserve stack buffer with more then 100 groups; reserving to 100 instead");
			size = 100;
		}

		m_recorded_1 = reinterpret_cast<group_1*>(alloca(size * sizeof(group_1)));
		m_recorded_2 = reinterpret_cast<group_2*>(alloca(size * sizeof(group_2)));
		m_recorded_3 = reinterpret_cast<group_3*>(alloca(size * sizeof(group_3)));
		m_recorded_4 = reinterpret_cast<group_4*>(alloca(size * sizeof(group_4)));
	}

	m_recorded_1_count = 0;
	m_recorded_2_count = 0;
	m_recorded_3_count = 0;
	m_recorded_4_count = 0;
}

bool cap::logger::buffer::has_recorded_information() const
{
	return
		m_recorded_1_count > 0 ||
		m_recorded_2_count > 0 ||
		m_recorded_3_count > 0 ||
		m_recorded_4_count > 0;
}

void cap::logger::buffer::setup_push()
{
	if (m_recorded_1_count >= m_recorded_max ||
		m_recorded_2_count >= m_recorded_max ||
		m_recorded_3_count >= m_recorded_max ||
		m_recorded_4_count >= m_recorded_max)
		flush();
}

void cap::logger::flush_buffer(cap::logger::buffer* b)
{
	LOG_BEGIN

	for (uint16_t i = 0; i < b->m_recorded_1_count; i++)
	{
		buffer::group_1& g = b->m_recorded_1[static_cast<size_t>(i)];
		switch (g._type)
		{
		case 1:
			cap::logger::info_raw(LOG_A1P);
			break;
		case 2:
			cap::logger::warn_raw(LOG_A1P);
			break;
		case 3:
			cap::logger::error_raw(LOG_A1P);
			break;
		default:
			cap::logger::fatal_raw(LOG_BUFFER_IDX_FAILURE);
			break;
		}
	}

	for (uint16_t i = 0; i < b->m_recorded_2_count; i++)
	{
		buffer::group_2& g = b->m_recorded_2[static_cast<size_t>(i)];
		switch (g._type)
		{
		case 1:
			cap::logger::info_raw(LOG_A2P);
			break;
		case 2:
			cap::logger::warn_raw(LOG_A2P);
			break;
		case 3:
			cap::logger::error_raw(LOG_A2P);
			break;
		default:
			cap::logger::fatal_raw(LOG_BUFFER_IDX_FAILURE);
			break;
		}
	}

	for (uint16_t i = 0; i < b->m_recorded_1_count; i++)
	{
		buffer::group_3& g = b->m_recorded_3[static_cast<size_t>(i)];
		switch (g._type)
		{
		case 1:
			cap::logger::info_raw(LOG_A3P);
			break;
		case 2:
			cap::logger::warn_raw(LOG_A3P);
			break;
		case 3:
			cap::logger::error_raw(LOG_A3P);
			break;
		default:
			cap::logger::fatal_raw(LOG_BUFFER_IDX_FAILURE);
			break;
		}
	}

	for (uint16_t i = 0; i < b->m_recorded_1_count; i++)
	{
		buffer::group_4& g = b->m_recorded_4[static_cast<size_t>(i)];
		switch (g._type)
		{
		case 1:
			cap::logger::info_raw(LOG_A4P);
			break;
		case 2:
			cap::logger::warn_raw(LOG_A4P);
			break;
		case 3:
			cap::logger::error_raw(LOG_A4P);
			break;
		default:
			cap::logger::fatal_raw(LOG_BUFFER_IDX_FAILURE);
			break;
		}
	}

	LOG_END
}

#define LOG_LC  info
#define LOG_LCR info_raw
#define LOG_UCS "INFO"

void cap::logger::LOG_LC(LOG_A1) { LOG_BEGIN LOG_EXE1(LOG_UCS) LOG_END }
void cap::logger::LOG_LC(LOG_A2) { LOG_BEGIN LOG_EXE2(LOG_UCS) LOG_END }
void cap::logger::LOG_LC(LOG_A3) { LOG_BEGIN LOG_EXE3(LOG_UCS) LOG_END }
void cap::logger::LOG_LC(LOG_A4) { LOG_BEGIN LOG_EXE4(LOG_UCS) LOG_END }

void cap::logger::LOG_LCR(LOG_A1) { LOG_EXE1(LOG_UCS) }
void cap::logger::LOG_LCR(LOG_A2) { LOG_EXE2(LOG_UCS) }
void cap::logger::LOG_LCR(LOG_A3) { LOG_EXE3(LOG_UCS) }
void cap::logger::LOG_LCR(LOG_A4) { LOG_EXE4(LOG_UCS) }

#undef LOG_LC
#undef LOG_LCR
#undef LOG_UCS

#define LOG_LC  warn
#define LOG_LCR warn_raw
#define LOG_UCS "WARN"

void cap::logger::LOG_LC(LOG_A1) { LOG_BEGIN LOG_EXE1(LOG_UCS) LOG_END }
void cap::logger::LOG_LC(LOG_A2) { LOG_BEGIN LOG_EXE2(LOG_UCS) LOG_END }
void cap::logger::LOG_LC(LOG_A3) { LOG_BEGIN LOG_EXE3(LOG_UCS) LOG_END }
void cap::logger::LOG_LC(LOG_A4) { LOG_BEGIN LOG_EXE4(LOG_UCS) LOG_END }

void cap::logger::LOG_LCR(LOG_A1) { LOG_EXE1(LOG_UCS) }
void cap::logger::LOG_LCR(LOG_A2) { LOG_EXE2(LOG_UCS) }
void cap::logger::LOG_LCR(LOG_A3) { LOG_EXE3(LOG_UCS) }
void cap::logger::LOG_LCR(LOG_A4) { LOG_EXE4(LOG_UCS) }

#undef LOG_LC
#undef LOG_LCR
#undef LOG_UCS

#define LOG_LC  error
#define LOG_LCR error_raw
#define LOG_UCS "ERROR"

void cap::logger::LOG_LC(LOG_A1) { LOG_BEGIN LOG_EXE1(LOG_UCS) LOG_END }
void cap::logger::LOG_LC(LOG_A2) { LOG_BEGIN LOG_EXE2(LOG_UCS) LOG_END }
void cap::logger::LOG_LC(LOG_A3) { LOG_BEGIN LOG_EXE3(LOG_UCS) LOG_END }
void cap::logger::LOG_LC(LOG_A4) { LOG_BEGIN LOG_EXE4(LOG_UCS) LOG_END }

void cap::logger::LOG_LCR(LOG_A1) { LOG_EXE1(LOG_UCS) }
void cap::logger::LOG_LCR(LOG_A2) { LOG_EXE2(LOG_UCS) }
void cap::logger::LOG_LCR(LOG_A3) { LOG_EXE3(LOG_UCS) }
void cap::logger::LOG_LCR(LOG_A4) { LOG_EXE4(LOG_UCS) }

#undef LOG_LC
#undef LOG_LCR
#undef LOG_UCS

#define LOG_LC  fatal
#define LOG_LCR fatal_raw
#define LOG_UCS "FATAL"

void cap::logger::LOG_LC(LOG_A1) { LOG_BEGIN LOG_EXE1(LOG_UCS) abort(); }
void cap::logger::LOG_LC(LOG_A2) { LOG_BEGIN LOG_EXE2(LOG_UCS) abort(); }
void cap::logger::LOG_LC(LOG_A3) { LOG_BEGIN LOG_EXE3(LOG_UCS) abort(); }
void cap::logger::LOG_LC(LOG_A4) { LOG_BEGIN LOG_EXE4(LOG_UCS) abort(); }

void cap::logger::LOG_LCR(LOG_A1) { LOG_EXE1(LOG_UCS) abort(); }
void cap::logger::LOG_LCR(LOG_A2) { LOG_EXE2(LOG_UCS) abort(); }
void cap::logger::LOG_LCR(LOG_A3) { LOG_EXE3(LOG_UCS) abort(); }
void cap::logger::LOG_LCR(LOG_A4) { LOG_EXE4(LOG_UCS) abort(); }

#undef LOG_LC
#undef LOG_LCR
#undef LOG_UCS

std::mutex cap::logger::m_mutex;
