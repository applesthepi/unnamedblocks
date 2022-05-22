#pragma once
#include "config.h"

#include "window.hpp"

#include <latte/utils.hpp>

namespace mac::renderer
{
///
struct state
{
	///
};

///
mac::renderer::state* create();

///
void destroy(mac::renderer::state* renderer_state);
}