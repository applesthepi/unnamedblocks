#pragma once
#include "config.h"

#include <Cappuccino/Utils.hpp>

/// Interface for enabling and disabling objects

class IEnableable
{
public:
	/// Sets default enable state during initializer.
	/// \param Enabled state.
	IEnableable(bool enabled);

	/// Sets enabled state.
	/// \param Enabled state.
	void SetEnabled(bool enabled);

	/// Get enabled state.
	/// \return Enabled state.
	bool GetEnabled();
protected:
	/// Enabled state.
	bool m_Enabled;
}