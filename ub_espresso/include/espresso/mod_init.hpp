#pragma once
#include "config.h"

#include "mod_data.hpp"

/// Include once in the mod main header. This function initializes the mod from it's perspective.
void initialize_mod(const esp::mod_push* push, esp::mod_pull* pull)
{
	latte::logger::initialize();

	latte::logger::set_stream_system(push->stream_system);
	latte::logger::set_stream_editor(push->stream_editor);
	latte::logger::set_stream_runtime(push->stream_runtime);

	esp::registry::set(push->esp_registry);
}