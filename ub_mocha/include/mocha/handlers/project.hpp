#pragma once
#include "config.h"

#include "mocha/stacking/plane.hpp"

#include <latte/utils.hpp>

namespace rhr::handler
{
///
class project
{
public:
	///
	static void load_project(const std::string& path);

	///
	static void save_project(const std::string& path);

	///
	static std::string current_path;

	///
	static std::vector<std::string> mods;
};
} // namespace rhr::handler