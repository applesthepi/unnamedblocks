#pragma once
#include "config.h"

#include <cappuccino/utils.hpp>
#include <cappuccino/color.hpp>

namespace esp::mod
{
///
class category
{
public:
	category(const std::string& mod_unlocalized_name, const std::string& mod_display_name);

	///
	std::string get_mod_unlocalized_name();

	///
	std::string get_mod_display_name();

	/// The color of the category, and the background color of all of the blocks registered to this category.
	virtual cap::color get_color() const;

	/// The name the engine uses to refer to this category as. Convention example: "mymod_some_category".
	virtual std::string get_unlocalized_name() const;

	/// What will be displayed on the category button as text. Convention example: "Some Category".
	virtual std::string get_display_name() const;
private:
	///
	const std::string m_mod_unlocalized_name;

	///
	const std::string m_mod_display_name;
};
}