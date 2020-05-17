#pragma once
#include "config.h"

#include <string>
#include <mutex>
//#include <string_view>
#include <iostream>

#ifndef LINUX
#ifdef __CAP
#define CAP_EXPORT __declspec(dllexport)
#define TYPDEF_EXPORT
#else
#define CAP_EXPORT __declspec(dllimport)
#define TYPDEF_EXPORT
#endif
#else
#ifdef __CAP
#define CAP_EXPORT
#define TYPDEF_EXPORT
#else
#define CAP_EXPORT 
#define TYPDEF_EXPORT 
#endif
#endif

class Logger
{
public:
	static CAP_EXPORT inline void Info(const std::string& message)
	{
		m_logMutex.lock();
		std::cout << " [INFO] " << message << "\n";
		m_logMutex.unlock();
	}

	static CAP_EXPORT inline void Warn(const std::string& message)
	{
		m_logMutex.lock();
		std::cout << " [WARN] " << message << "\n";
		m_logMutex.unlock();
	}

	static CAP_EXPORT inline void Error(const std::string& message)
	{
		m_logMutex.lock();
		std::cout << " [ERROR] " << message << "\n";
		m_logMutex.unlock();
	}

	static CAP_EXPORT inline void Debug(const std::string& message)
	{
		m_logMutex.lock();
		std::cout << " [DEBUG] " << message << "\n";
		m_logMutex.unlock();
	}
private:
	static CAP_EXPORT std::mutex m_logMutex;
};
