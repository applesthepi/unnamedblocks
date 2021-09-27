#pragma once
#include "config.h"

#include "cappuccino/export.hpp"

// utils.hpp requires logger.hpp
#include <string>
#include <mutex>
#include <glm/glm.hpp>

namespace cap
{
///
class logger
{
public:
	///
	static CAP_C_EXPORT void info(const std::string& message);

	///
	static CAP_C_EXPORT void warn(const std::string& message);

	///
	static CAP_C_EXPORT void error(const std::string& message);

	///
	static CAP_C_EXPORT void fatal(const std::string& message);

	///
	static CAP_C_EXPORT void debug(const std::string& message);

	///
	static CAP_C_EXPORT void debug(glm::vec<2, int32_t> message);
private:
	///
	static CAP_C_EXPORT std::mutex m_log_mutex;
};
}