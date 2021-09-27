#include "logger.hpp"

#include <iostream>

CAP_C_EXPORT void cap::logger::info(const std::string& message)
{
    m_log_mutex.lock();
    std::cout << " [INFO] " << message << "\n" << std::flush;
    m_log_mutex.unlock();
}

CAP_C_EXPORT void cap::logger::warn(const std::string& message)
{
    m_log_mutex.lock();
    std::cout << " [WARN] " << message << "\n" << std::flush;
    m_log_mutex.unlock();
}

CAP_C_EXPORT void cap::logger::error(const std::string& message)
{
    m_log_mutex.lock();
    std::cout << " [ERROR] " << message << "\n" << std::flush;
    m_log_mutex.unlock();
}

CAP_C_EXPORT void cap::logger::fatal(const std::string& message)
{
    m_log_mutex.lock();
    std::cout << " [FATAL] " << message << "\n" << std::flush;
	abort();
}

CAP_C_EXPORT void cap::logger::debug(const std::string& message)
{
    m_log_mutex.lock();
    std::cout << " [DEBUG] " << message << "\n" << std::flush;
    m_log_mutex.unlock();
}

void cap::logger::debug(glm::vec<2, int32_t> message)
{
	m_log_mutex.lock();
	std::cout << " [DEBUG] { " << std::to_string(message.x) << ", " << std::to_string(message.y) << " }\n" << std::flush;
	m_log_mutex.unlock();
}

std::mutex CAP_C_EXPORT cap::logger::m_log_mutex;
