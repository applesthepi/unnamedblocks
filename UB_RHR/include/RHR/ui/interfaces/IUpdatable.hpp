#pragma once
#include "config.h"

#include <Cappuccino/Utils.hpp>

/// Interface for objects that update.

/// Can be abstracted into a joint interface.

class IUpdatable
{
public:
	/// Runs initializer list.
	IUpdatable();

	/// Setup upstream callbacks.
	//void SetupVirtualFunctions(void(*frameUpdate)(double));

	/// Runs every frame before rendering.
	/// \param Seconds since last frame.
	virtual void FrameUpdate(double deltaTime);
private:
	/// Check for virtual
	//bool IsVirtual();

	/// Garantees upstream callbacks exist
	//bool m_Virtual;

	/// Render upstream callback
	//void(*m_FrameUpdate)(double);
};