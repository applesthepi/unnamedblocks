#pragma once
#include "config.h"

#include "Export.hpp"

// Utils.hpp requires Logger.hpp
#include <string>
#include <mutex>

class Logger
{
public:
	static CAP_C_EXPORT void Info(const std::string& message);

	static CAP_C_EXPORT void Warn(const std::string& message);

	static CAP_C_EXPORT void Error(const std::string& message);

	static CAP_C_EXPORT void Debug(const std::string& message);
private:
	static CAP_C_EXPORT std::mutex m_logMutex;
};
