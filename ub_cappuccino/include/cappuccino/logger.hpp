#pragma once
#include "config.h"

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
	static void info(const std::string& message);

	///
	static void warn(const std::string& message);

	///
	static void error(const std::string& message);

	///
	static void fatal(const std::string& message);

	///
	static void debug(const std::string& message);

	///
	static void debug(const std::string& tag, glm::vec<2, int32_t> message);
private:
	///
	static std::mutex m_log_mutex;
};
}