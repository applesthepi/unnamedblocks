#pragma once
#include "config.h"

#include "stacking/Plane.hpp"

#include <Cappuccino/Utils.hpp>

class ProjectHandler
{
public:
	static void LoadProject(const std::string& path);
	static void SaveProject(const std::string& path);

	static std::string CurrentPath;
	static std::vector<std::string> Mods;
};
