#include "Logger.hpp"

#include <iostream>

CAP_C_EXPORT void Logger::Info(const std::string &message)
{
    m_logMutex.lock();
    std::cout << " [INFO] " << message << "\n";
    m_logMutex.unlock();
}

CAP_C_EXPORT void Logger::Warn(const std::string &message)
{
    m_logMutex.lock();
    std::cout << " [WARN] " << message << "\n";
    m_logMutex.unlock();
}

CAP_C_EXPORT void Logger::Error(const std::string &message)
{
    m_logMutex.lock();
    std::cout << " [ERROR] " << message << "\n";
    m_logMutex.unlock();
}

CAP_C_EXPORT void Logger::Debug(const std::string &message)
{
    m_logMutex.lock();
    std::cout << " [DEBUG] " << message << "\n";
    m_logMutex.unlock();
}

std::mutex CAP_C_EXPORT Logger::m_logMutex;
