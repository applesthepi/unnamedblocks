#pragma once
#include "RHR/stacking/Plane.h"

#include <string>

class ProjectHandler
{
public:
	static void Initialize();
	static void LoadProject(std::string path, Plane* plane, BlockRegistry* registry);
	static void SaveProject(std::string path, Plane* plane);

	static std::string CurrentPath;
	static std::vector<std::string> Mods;
};