#pragma once
#include "config.h"
#include <string>
// Gets the path to the users runtime directory containing res and mods. Shouldn't have a trailing slash
const std::string& get_runtime_path();
// Path to the unnamedblocks executable. Should have a trailing \\ or / unless something has gone horribly wrong
extern std::string executable_path;
