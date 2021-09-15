#include "categories.hpp"

cap::color CategorySystem::get_color() const
{
	return cap::color().from_u8({ 88, 95, 179, 255 });
}

std::string CategorySystem::get_unlocalized_name() const
{
	return CATEGORY_SYSTEM;
}

std::string CategorySystem::get_display_name() const
{
	return "System";
}

cap::color CategoryUtility::get_color() const
{
	return cap::color().from_u8({ 205, 57, 51, 255 });
}

std::string CategoryUtility::get_unlocalized_name() const
{
	return CATEGORY_UTILITY;
}

std::string CategoryUtility::get_display_name() const
{
	return "Utility";
}

cap::color CategoryGraphics::get_color() const
{
	return cap::color().from_u8({ 230, 143, 46, 255 });
}

std::string CategoryGraphics::get_unlocalized_name() const
{
	return CATEGORY_GRAPHICS;
}

std::string CategoryGraphics::get_display_name() const
{
	return "Graphics";
}

cap::color CategoryBooleans::get_color() const
{
	return cap::color().from_u8({ 230, 103, 67, 255 });
}

std::string CategoryBooleans::get_unlocalized_name() const
{
	return CATEGORY_BOOLEANS;
}

std::string CategoryBooleans::get_display_name() const
{
	return "Booleans";
}

cap::color CategoryReals::get_color() const
{
	return cap::color().from_u8({ 79, 154, 62, 255 });
}

std::string CategoryReals::get_unlocalized_name() const
{
	return CATEGORY_REALS;
}

std::string CategoryReals::get_display_name() const
{
	return "Reals";
}

cap::color CategoryStrings::get_color() const
{
	return cap::color().from_u8({ 16, 167, 167, 255 });
}

std::string CategoryStrings::get_unlocalized_name() const
{
	return CATEGORY_STRINGS;
}

std::string CategoryStrings::get_display_name() const
{
	return "Strings";
}
