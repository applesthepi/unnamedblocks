#include "util.h"
#include <cstring>
#include <stdexcept>
#include <Cappuccino/Logger.h>
std::string executable_path;
// Returns the location of the unnamedblocks runtime folder. This is where res and mods are
// Not thread safe until after its ran once
const std::string& get_runtime_path() {
	static bool evaluated = false;
	static std::string path = RUNTIME_FOLDER_PATH;
	if(evaluated) {
		return path;
	}
	while(true) {
		size_t pos[] = {0, 0};
		pos[0] = path.find("${");

		if (pos[0] == std::string::npos) 
			break;

		pos[1] = path.find("}", pos[0]);

		if (pos[1] == std::string::npos) 
			throw std::runtime_error("Invalid mod search path. No closing } after a ${. Please report this to your maintainer. Value = " + std::string(RUNTIME_FOLDER_PATH));

		/// Blank environment variable
		if(pos[0] + 2 == pos[1]) {
			Logger::Warn("Environment variable definition in mods search path is blank. Treating it as though it doesn't exist. Please report this to your maintainer.");
			path.erase(pos[0]+2, pos[1]-1);
			continue;
		}

		std::string env = path.substr(pos[0] + 2, pos[1] - pos[0] - 2);
		const char* value = std::getenv(env.c_str());

		// Environment variable with no value
		// Could mean broken environment, unset XDG directory, or coffee deprived maintainer
		if(value == NULL) {
			if(env == "XDG_DATA_HOME") {
				const char* home = getenv("HOME");
				if(home == NULL) {
					Logger::Error("XDG_DATA_HOME is not set and HOME is not set. Your environment is probably busted. Please fix it.");
					throw std::runtime_error("XDG_DATA_HOME is not set and HOME is not set. Your environment is probably busted. Please fix it.");
				}
				else {
					// Replace in path
					path.erase(pos[0], pos[1] - pos[0] + 1);
					path.insert(pos[0], home);
					path.insert(pos[0] + strlen(home), "/.local/share");
				}
			}
			else if(env == "XDG_CONFIG_HOME") {
				const char* home = getenv("HOME");
				if(home == NULL) {
					Logger::Error("XDG_CONFIG_HOME is not set and HOME is not set. Your environment is probably busted. Please fix it.");
					throw std::runtime_error("XDG_CONFIG_HOME is not set and HOME is not set. Your environment is probably busted. Please fix it.");
				}
				else {
					// Replace in path
					path.erase(pos[0], pos[1] - pos[0] + 1);
					path.insert(pos[0], home);
					path.insert(pos[0] + strlen(home), "/.config");
				}
			}
			else if(env == "XDG_CACHE_HOME") {
				const char* home = getenv("HOME");
				if(home == NULL) {
					Logger::Error("XDG_CACHE_HOME is not set and HOME is not set. Your environment is probably busted. Please fix it.");
					throw std::runtime_error("XDG_CACHE_HOME is not set and HOME is not set. Your environment is probably busted. Please fix it.");
				}
				else {
					// Replace in path
					path.erase(pos[0], pos[1] - pos[0] + 1);
					path.insert(pos[0], home);
					path.insert(pos[0] + strlen(home), "/.cache");
				}
			}
			else {
				Logger::Warn("Environment variable value is blank. This could mean a broken environment or a coffee deprived maintainer. Environment variable name: " + env);
				path.erase(pos[0]+2, pos[1]-1);
			}
			continue;
		}

		// Replace in path
		path.erase(pos[0], pos[1] - pos[0] + 1);
		path.insert(pos[0], value);
	}
	evaluated = true;
	return path;
}
