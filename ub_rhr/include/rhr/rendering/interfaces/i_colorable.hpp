#pragma once
#include "config.h"

#include <cappuccino/color.hpp>
#include <cappuccino/utils.hpp>

namespace rhr::render::interfaces
{
/// Interface for setting a color.
class i_colorable
{
public:
	///
	i_colorable(const cap::color& color);

	///
	void set_color(const cap::color& color);

	///
	const cap::color& get_color();

protected:
	///
	cap::color m_color;

	/// Gets called after the color is changed.
	virtual void post_color_update();
};
} // namespace rhr::render::interfaces