#pragma once
#include "config.h"

#include "espresso/color.hpp"

#include <latte/utils.hpp>

namespace esp
{
///
class category
{
public:
	///
	virtual std::string get_mod_unlocalized_name() = 0;

	///
	virtual std::string get_mod_display_name() = 0;

	/// The color of the category, and the background color of all of the blocks registered to this category.
	virtual espresso::color get_color() = 0;

	/// The name the engine uses to refer to this category as. Convention example: "mymod_some_category".
	virtual std::string get_unlocalized_name() = 0;

	/// What will be displayed on the category button as text. Convention example: "Some Category".
	virtual std::string get_display_name() = 0;
};
}