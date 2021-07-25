#pragma once
#include "config.h"

#include <cappuccino/utils.hpp>

namespace rhr::render::interfaces
{
/// Interface for enabling and disabling objects.
class i_enableable
{
public:
	/// Sets default enabled state.
	i_enableable(bool enabled);

	///
	void set_enabled(bool enabled);

	///
	bool get_enabled();
protected:
	///
	bool m_enabled;
};
}