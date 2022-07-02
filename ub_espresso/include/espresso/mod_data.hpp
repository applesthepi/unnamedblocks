#pragma once
#include "config.h"

#include "espresso/registry.hpp"

#include <latte/utils.hpp>

namespace esp
{
///
struct mod_push
{
	///
	latte::logger::stream* stream_system;
	latte::logger::stream* stream_editor;
	latte::logger::stream* stream_runtime;

	///
	esp::registry* esp_registry;
};

///
struct mod_pull
{

};
}