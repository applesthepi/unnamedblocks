#include "logger.hpp"

#include <iostream>

#define ERROR_DATA_NULL "logger not initialized; a stream is nullptr"
#define ERROR_OWNER_REQUIRED "this logger must be the owner of its stream in order to perform this operation"
#define ERROR_BUFFER_IDX_FAILURE "invalid buffer log _type idx"

#ifndef NDEBUG

#define CHECK_SYSTEM             \
if (m_stream_system == nullptr)  \
	internal_fatal(ERROR_DATA_NULL);

#define CHECK_EDITOR             \
if (m_stream_editor == nullptr)  \
	internal_fatal(ERROR_DATA_NULL);

#define CHECK_RUNTIME             \
if (m_stream_runtime == nullptr)  \
	internal_fatal(ERROR_DATA_NULL);

#define CHECK_ALL CHECK_SYSTEM CHECK_EDITOR CHECK_RUNTIME

#define CHECK_OWNER_SYSTEM   \
if (!m_stream_system_owner)  \
	internal_fatal(ERROR_OWNER_REQUIRED);

#define CHECK_OWNER_EDITOR  \
if (!m_stream_editor_owner) \
	internal_fatal(ERROR_OWNER_REQUIRED);

#define CHECK_OWNER_RUNTIME  \
if (!m_stream_runtime_owner) \
	internal_fatal(ERROR_OWNER_REQUIRED);

#else

#define CHECK_SYSTEM
#define CHECK_EDITOR
#define CHECK_RUNTIME

#define CHECK_OWNER_SYSTEM
#define CHECK_OWNER_EDITOR
#define CHECK_OWNER_RUNTIME

#endif

#define MUTEX_LOCK_SYSTEM m_stream_system->mutex.lock();
#define MUTEX_LOCK_EDITOR m_stream_editor->mutex.lock();
#define MUTEX_LOCK_RUNTIME m_stream_runtime->mutex.lock();

#define MUTEX_UNLOCK_SYSTEM m_stream_system->mutex.unlock();
#define MUTEX_UNLOCK_EDITOR m_stream_editor->mutex.unlock();
#define MUTEX_UNLOCK_RUNTIME m_stream_runtime->mutex.unlock();

#define MUTEX_LOCK_ALL MUTEX_LOCK_SYSTEM MUTEX_LOCK_EDITOR MUTEX_LOCK_RUNTIME
#define MUTEX_UNLOCK_ALL MUTEX_UNLOCK_SYSTEM MUTEX_UNLOCK_EDITOR MUTEX_UNLOCK_RUNTIME

#define PREP_LOG_RAW              \
switch (level)                    \
{                                 \
case cap::logger::level::SYSTEM:  \
	CHECK_SYSTEM                  \
	break;                        \
case cap::logger::level::EDITOR:  \
	CHECK_EDITOR                  \
	break;                        \
case cap::logger::level::RUNTIME: \
	CHECK_RUNTIME                 \
	break;                        \
}

#define PREP_LOG_MUT              \
switch (level)                    \
{                                 \
case cap::logger::level::SYSTEM:  \
	CHECK_SYSTEM                  \
    MUTEX_LOCK_SYSTEM             \
	break;                        \
case cap::logger::level::EDITOR:  \
	CHECK_EDITOR                  \
    MUTEX_LOCK_EDITOR             \
	break;                        \
case cap::logger::level::RUNTIME: \
	CHECK_RUNTIME                 \
    MUTEX_LOCK_RUNTIME            \
	break;                        \
}

#define FINISH_LOG_MUT            \
switch (level)                    \
{                                 \
case cap::logger::level::SYSTEM:  \
    MUTEX_UNLOCK_SYSTEM           \
	break;                        \
case cap::logger::level::EDITOR:  \
    MUTEX_UNLOCK_EDITOR           \
	break;                        \
case cap::logger::level::RUNTIME: \
    MUTEX_UNLOCK_RUNTIME          \
	break;                        \
}

#define LOG_SWITCH_STREAM                  \
cap::logger::stream* use_stream = nullptr; \
switch (level)                             \
{                                          \
case cap::logger::level::SYSTEM:           \
	use_stream = m_stream_system;          \
	break;                                 \
case cap::logger::level::EDITOR:           \
	use_stream = m_stream_editor;          \
	break;                                 \
case cap::logger::level::RUNTIME:          \
	use_stream = m_stream_runtime;         \
	break;                                 \
}

#define LOG_EXE1(__UCS)                                    \
LOG_SWITCH_STREAM                                          \
std::string& out = use_stream->cached_logs.emplace_back(); \
out = " [" + std::string(__UCS) + "] " + message;          \
std::cout << out << "\n" << std::flush;                    \
use_stream->log_update = true;

#define LOG_EXE2(__UCS)                                         \
LOG_SWITCH_STREAM                                               \
for (auto& message : messages)                                  \
{                                                               \
	std::string& out = use_stream->cached_logs.emplace_back();  \
	out = " [" + std::string(__UCS) + "] " + message;           \
	std::cout << out << "\n";                                   \
}                                                               \
std::cout << std::flush;                                        \
use_stream->log_update = true;

#define LOG_EXE3(__UCS)                                                                                                                 \
LOG_SWITCH_STREAM                                                                                                                       \
std::string& out = use_stream->cached_logs.emplace_back();                                                                              \
out = "\n [" + std::string(__UCS) + "] [" + file + "] [" + std::to_string(line) + "]\n [" + std::string(__UCS) + "] " + message + "\n"; \
std::cout << out << "\n" << std::flush;                                                                                                 \
use_stream->log_update = true;

#define LOG_EXE4(__UCS)                                                                 \
LOG_SWITCH_STREAM                                                                       \
std::string& out1 = use_stream->cached_logs.emplace_back();                             \
out1 = "\n [" + std::string(__UCS) + "] [" + file + "] [" + std::to_string(line) + "]"; \
std::cout << out1 << "\n";                                                              \
for (auto& message : messages)                                                          \
{                                                                                       \
	std::string& out2 = use_stream->cached_logs.emplace_back();                         \
	out2 = " [" + std::string(__UCS) + "] " + message;                                  \
	std::cout << out2 << "\n";                                                          \
}                                                                                       \
std::cout << std::flush;                                                                \
use_stream->log_update = true;

#ifdef NDEBUG
#define ASSERT_ABORT
#else
#define ASSERT_ABORT abort();
#endif

cap::logger::buffer::buffer(bool destruction_flush)
	: m_destruction_flush(destruction_flush)
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

	m_recorded_1.clear();
	m_recorded_2.clear();
	m_recorded_3.clear();
	m_recorded_4.clear();
}

void cap::logger::buffer::reserve(size_t size)
{
	m_recorded_1.clear();
	m_recorded_2.clear();
	m_recorded_3.clear();
	m_recorded_4.clear();

	m_recorded_1.reserve(size);
	m_recorded_2.reserve(size);
	m_recorded_3.reserve(size);
	m_recorded_4.reserve(size);
}

bool cap::logger::buffer::has_recorded_information() const
{
	return
		!m_recorded_1.empty() ||
		!m_recorded_2.empty() ||
		!m_recorded_3.empty() ||
		!m_recorded_4.empty();
}

void cap::logger::buffer::setup_push()
{
	if (m_recorded_1.size() == m_recorded_1.capacity() ||
		m_recorded_2.size() == m_recorded_2.capacity() ||
		m_recorded_3.size() == m_recorded_3.capacity() ||
		m_recorded_4.size() == m_recorded_4.capacity())
		flush();
}

void cap::logger::initialize()
{
	m_stream_system = new cap::logger::stream();
	m_stream_editor = new cap::logger::stream();
	m_stream_runtime = new cap::logger::stream();
}

cap::logger::stream* cap::logger::get_stream_system()
{
	CHECK_SYSTEM
	CHECK_OWNER_SYSTEM

	return m_stream_system;
}

cap::logger::stream* cap::logger::get_stream_editor()
{
	CHECK_EDITOR
	CHECK_OWNER_EDITOR

	return m_stream_editor;
}

cap::logger::stream* cap::logger::get_stream_runtime()
{
	CHECK_RUNTIME
	CHECK_OWNER_RUNTIME

	return m_stream_runtime;
}

void cap::logger::set_stream_system(cap::logger::stream* stream_system)
{
	m_stream_system_owner = false;
	m_stream_system = stream_system;
}

void cap::logger::set_stream_editor(cap::logger::stream* stream_editor)
{
	m_stream_editor_owner = false;
	m_stream_editor = stream_editor;
}

void cap::logger::set_stream_runtime(cap::logger::stream* stream_runtime)
{
	m_stream_runtime_owner = false;
	m_stream_runtime = stream_runtime;
}

void cap::logger::flush_buffer(cap::logger::buffer* b)
{
	CHECK_ALL
	MUTEX_LOCK_ALL

	for (uint16_t i = 0; i < b->m_recorded_1.size(); i++)
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
			internal_fatal(ERROR_BUFFER_IDX_FAILURE);
			break;
		}
	}

	for (uint16_t i = 0; i < b->m_recorded_2.size(); i++)
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
			internal_fatal(ERROR_BUFFER_IDX_FAILURE);
			break;
		}
	}

	for (uint16_t i = 0; i < b->m_recorded_3.size(); i++)
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
			internal_fatal(ERROR_BUFFER_IDX_FAILURE);
			break;
		}
	}

	for (uint16_t i = 0; i < b->m_recorded_4.size(); i++)
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
			internal_fatal(ERROR_BUFFER_IDX_FAILURE);
			break;
		}
	}

	MUTEX_UNLOCK_ALL
}

#define LOG_LC  info
#define LOG_LCR info_raw
#define LOG_UCS "INFO"

void cap::logger::LOG_LC(LOG_A1) { PREP_LOG_MUT LOG_EXE1(LOG_UCS) FINISH_LOG_MUT }
void cap::logger::LOG_LC(LOG_A2) { PREP_LOG_MUT LOG_EXE2(LOG_UCS) FINISH_LOG_MUT }
void cap::logger::LOG_LC(LOG_A3) { PREP_LOG_MUT LOG_EXE3(LOG_UCS) FINISH_LOG_MUT }
void cap::logger::LOG_LC(LOG_A4) { PREP_LOG_MUT LOG_EXE4(LOG_UCS) FINISH_LOG_MUT }

void cap::logger::LOG_LCR(LOG_A1) { PREP_LOG_RAW LOG_EXE1(LOG_UCS) }
void cap::logger::LOG_LCR(LOG_A2) { PREP_LOG_RAW LOG_EXE2(LOG_UCS) }
void cap::logger::LOG_LCR(LOG_A3) { PREP_LOG_RAW LOG_EXE3(LOG_UCS) }
void cap::logger::LOG_LCR(LOG_A4) { PREP_LOG_RAW LOG_EXE4(LOG_UCS) }

#undef LOG_LC
#undef LOG_LCR
#undef LOG_UCS

#define LOG_LC  warn
#define LOG_LCR warn_raw

#define LOG_UCS "WARN"
void cap::logger::LOG_LC(LOG_A1) { PREP_LOG_MUT LOG_EXE1(LOG_UCS) FINISH_LOG_MUT }
void cap::logger::LOG_LC(LOG_A2) { PREP_LOG_MUT LOG_EXE2(LOG_UCS) FINISH_LOG_MUT }
void cap::logger::LOG_LC(LOG_A3) { PREP_LOG_MUT LOG_EXE3(LOG_UCS) FINISH_LOG_MUT }
void cap::logger::LOG_LC(LOG_A4) { PREP_LOG_MUT LOG_EXE4(LOG_UCS) FINISH_LOG_MUT }

void cap::logger::LOG_LCR(LOG_A1) { PREP_LOG_RAW LOG_EXE1(LOG_UCS) }
void cap::logger::LOG_LCR(LOG_A2) { PREP_LOG_RAW LOG_EXE2(LOG_UCS) }
void cap::logger::LOG_LCR(LOG_A3) { PREP_LOG_RAW LOG_EXE3(LOG_UCS) }
void cap::logger::LOG_LCR(LOG_A4) { PREP_LOG_RAW LOG_EXE4(LOG_UCS) }

#undef LOG_LC
#undef LOG_LCR
#undef LOG_UCS

#define LOG_LC  error
#define LOG_LCR error_raw

#define LOG_UCS "ERROR"
void cap::logger::LOG_LC(LOG_A1) { PREP_LOG_MUT LOG_EXE1(LOG_UCS) ASSERT_ABORT FINISH_LOG_MUT }
void cap::logger::LOG_LC(LOG_A2) { PREP_LOG_MUT LOG_EXE2(LOG_UCS) ASSERT_ABORT FINISH_LOG_MUT }
void cap::logger::LOG_LC(LOG_A3) { PREP_LOG_MUT LOG_EXE3(LOG_UCS) ASSERT_ABORT FINISH_LOG_MUT }
void cap::logger::LOG_LC(LOG_A4) { PREP_LOG_MUT LOG_EXE4(LOG_UCS) ASSERT_ABORT FINISH_LOG_MUT }

void cap::logger::LOG_LCR(LOG_A1) { PREP_LOG_RAW LOG_EXE1(LOG_UCS) }
void cap::logger::LOG_LCR(LOG_A2) { PREP_LOG_RAW LOG_EXE2(LOG_UCS) }
void cap::logger::LOG_LCR(LOG_A3) { PREP_LOG_RAW LOG_EXE3(LOG_UCS) }
void cap::logger::LOG_LCR(LOG_A4) { PREP_LOG_RAW LOG_EXE4(LOG_UCS) }

#undef LOG_LC
#undef LOG_LCR
#undef LOG_UCS
#define LOG_LC  fatal
#define LOG_LCR fatal_raw

#define LOG_UCS "FATAL"
void cap::logger::LOG_LC(LOG_A1) { PREP_LOG_MUT LOG_EXE1(LOG_UCS) abort(); }
void cap::logger::LOG_LC(LOG_A2) { PREP_LOG_MUT LOG_EXE2(LOG_UCS) abort(); }
void cap::logger::LOG_LC(LOG_A3) { PREP_LOG_MUT LOG_EXE3(LOG_UCS) abort(); }
void cap::logger::LOG_LC(LOG_A4) { PREP_LOG_MUT LOG_EXE4(LOG_UCS) abort(); }

void cap::logger::LOG_LCR(LOG_A1) { PREP_LOG_RAW LOG_EXE1(LOG_UCS) abort(); }
void cap::logger::LOG_LCR(LOG_A2) { PREP_LOG_RAW LOG_EXE2(LOG_UCS) abort(); }
void cap::logger::LOG_LCR(LOG_A3) { PREP_LOG_RAW LOG_EXE3(LOG_UCS) abort(); }
void cap::logger::LOG_LCR(LOG_A4) { PREP_LOG_RAW LOG_EXE4(LOG_UCS) abort(); }

#undef LOG_LC
#undef LOG_LCR
#undef LOG_UCS

void cap::logger::internal_fatal(const std::string& error)
{
	std::cout << " [FATAL] " + error << std::endl;
	abort();
}

cap::logger::stream* cap::logger::m_stream_system;
cap::logger::stream* cap::logger::m_stream_editor;
cap::logger::stream* cap::logger::m_stream_runtime;

bool cap::logger::m_stream_system_owner = true;
bool cap::logger::m_stream_editor_owner = true;
bool cap::logger::m_stream_runtime_owner = true;
