#pragma once
#include "config.h"

#include <Cappuccino/Utils.hpp>

/// Interface for objects that update.

/// Can be abstracted into a joint interface.

class IUpdatable
{
public:
	/// Runs every frame before rendering.
	/// \param Seconds since last frame.
	virtual void FrameUpdate(double deltaTime);
};