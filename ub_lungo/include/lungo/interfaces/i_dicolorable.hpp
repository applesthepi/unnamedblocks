#pragma once
#include "config.h"

#include <espresso/color.hpp>
#include <latte/utils.hpp>

namespace rhr::render::interfaces
{
/// Interface for setting two colors.
class i_dicolorable
{
public:
	/// Sets default colors.
	i_dicolorable(const espresso::color& primary_color, const espresso::color& secondary_color);

	///
	void set_color_primary(const espresso::color& primary);

	///
	void set_color_secondary(const espresso::color& secondary);

	///
	const espresso::color& get_color_primary();

	///
	const espresso::color& get_color_secondary();

protected:
	/// Gets called after the color is changed.
	virtual void post_color_update();

	///
	espresso::color m_color_primary;

	///
	espresso::color m_color_secondary;
};
} // namespace rhr::render::interfaces