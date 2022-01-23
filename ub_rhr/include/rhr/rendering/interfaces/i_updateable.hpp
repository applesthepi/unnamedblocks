#pragma once
#include "config.h"

#include <cappuccino/utils.hpp>

namespace rhr::render::interfaces
{
/// Interface for objects that update.
class i_updateable
{
public:
	/// Runs every frame before rendering.
	/// \param Seconds since last frame.
	virtual void frame_update(f64 delta_time);
};
} // namespace rhr::render::interfaces