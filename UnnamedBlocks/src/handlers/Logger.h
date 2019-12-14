#pragma once

#include <string>
#include <mutex>

class Logger
{
public:
	static void Info(std::string message)
	{
		m_logMutex.lock();
		printf(" [INFO] %s\n", message.c_str());
		m_logMutex.unlock();
	}

	static void Warn(std::string message)
	{
		m_logMutex.lock();
		printf(" [WARN] %s\n", message.c_str());
		m_logMutex.unlock();
	}

	static void Error(std::string message)
	{
		m_logMutex.lock();
		printf(" [ERROR] %s\n", message.c_str());
		m_logMutex.unlock();
	}
private:
	static std::mutex m_logMutex;
};