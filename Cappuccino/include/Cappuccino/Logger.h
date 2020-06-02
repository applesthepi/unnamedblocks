#pragma once
#include "config.h"

#include <string>
#include <mutex>
//#include <string_view>
#include "Cappuccino.h"
#include <iostream>

class Logger
{
public:
	static CAP_C_EXPORT inline void Info(const std::string& message)
	{
		m_logMutex.lock();
		std::cout << " [INFO] " << message << "\n";
		m_logMutex.unlock();
	}

	static CAP_C_EXPORT inline void Warn(const std::string& message)
	{
		m_logMutex.lock();
		std::cout << " [WARN] " << message << "\n";
		m_logMutex.unlock();
	}

	static CAP_C_EXPORT inline void Error(const std::string& message)
	{
		m_logMutex.lock();
		std::cout << " [ERROR] " << message << "\n";
		m_logMutex.unlock();
	}

	static CAP_C_EXPORT inline void Debug(const std::string& message)
	{
		m_logMutex.lock();
		std::cout << " [DEBUG] " << message << "\n";
		m_logMutex.unlock();
	}
private:
	static CAP_C_EXPORT std::mutex m_logMutex;
};
