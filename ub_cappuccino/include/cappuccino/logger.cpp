#include "logger.hpp"

#include <iostream>

CAP_C_EXPORT void Logger::Info(const std::string& message)
{
    m_logMutex.lock();
    std::cout << " [INFO] " << message << "\n" << std::flush;
    m_logMutex.unlock();
}

CAP_C_EXPORT void Logger::Warn(const std::string& message)
{
    m_logMutex.lock();
    std::cout << " [WARN] " << message << "\n" << std::flush;
    m_logMutex.unlock();
}

CAP_C_EXPORT void Logger::Error(const std::string& message)
{
    m_logMutex.lock();
    std::cout << " [ERROR] " << message << "\n" << std::flush;
    m_logMutex.unlock();
}

CAP_C_EXPORT void Logger::Fatal(const std::string& message)
{
    m_logMutex.lock();
    std::cout << " [FATAL] " << message << "\n" << std::flush;
	abort();
}

CAP_C_EXPORT void Logger::Debug(const std::string& message)
{
    m_logMutex.lock();
    std::cout << " [DEBUG] " << message << "\n" << std::flush;
    m_logMutex.unlock();
}

std::mutex CAP_C_EXPORT Logger::m_logMutex;
