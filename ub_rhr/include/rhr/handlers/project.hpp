#pragma once
#include "config.h"

#include "rhr/stacking/plane.hpp"

#include <cappuccino/utils.hpp>

namespace rhr::handler
{
///
class project
{
public:
	///
	//static void LoadProject(const std::string& path);

	///
	//static void SaveProject(const std::string& path);

	///
	static std::string current_path;

	///
	static std::vector<std::string> mods;
};
}