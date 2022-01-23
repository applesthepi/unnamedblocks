#pragma once
#include "config.h"

#include <cappuccino/color.hpp>
#include <cappuccino/utils.hpp>

namespace rhr::render::interfaces
{
/// Interface for setting two colors.
class i_dicolorable
{
public:
	/// Sets default colors.
	i_dicolorable(const cap::color& primary_color, const cap::color& secondary_color);

	///
	void set_color_primary(const cap::color& primary);

	///
	void set_color_secondary(const cap::color& secondary);

	///
	const cap::color& get_color_primary();

	///
	const cap::color& get_color_secondary();

protected:
	/// Gets called after the color is changed.
	virtual void post_color_update();

	///
	cap::color m_color_primary;

	///
	cap::color m_color_secondary;
};
} // namespace rhr::render::interfaces