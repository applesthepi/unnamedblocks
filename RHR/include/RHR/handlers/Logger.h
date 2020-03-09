#pragma once

#include <string>
#include <mutex>
#include <string_view>
#include <iostream>

class Logger
{
public:
	static inline void Info(std::string_view message)
	{
		m_logMutex.lock();
		std::cout << " [INFO] " << message << "\n";
		m_logMutex.unlock();
	}

	static inline void Warn(std::string_view message)
	{
		m_logMutex.lock();
		std::cout << " [WARN] " << message << "\n";
		m_logMutex.unlock();
	}

	static inline void Error(std::string_view message)
	{
		m_logMutex.lock();
		std::cout << " [ERROR] " << message << "\n";
		m_logMutex.unlock();
	}

	static inline void Debug(std::string_view message)
	{
		m_logMutex.lock();
		std::cout << " [DEBUG] " << message << "\n";
		m_logMutex.unlock();
	}
private:
	static std::mutex m_logMutex;
};
