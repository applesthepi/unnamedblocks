#pragma once
#include "config.h"

#include <espresso/color.hpp>
#include <latte/utils.hpp>

namespace rhr::render::interfaces
{
/// Interface for setting a color.
class i_colorable
{
public:
	///
	i_colorable(const espresso::color& color);

	///
	void set_color(const espresso::color& color);

	///
	const espresso::color& get_color();

protected:
	///
	espresso::color m_color;

	/// Gets called after the color is changed.
	virtual void post_color_update();
};
} // namespace rhr::render::interfaces