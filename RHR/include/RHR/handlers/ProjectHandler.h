#pragma once
#include "RHR/stacking/Plane.h"

#include <string>

class ProjectHandler
{
public:
	static void LoadProject(const std::string& path);
	static void SaveProject(const std::string& path);

	static std::string CurrentPath;
	static std::vector<std::string> Mods;
};
