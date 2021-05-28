#include "ModCategories.h"

const sf::Color CategorySystem::GetColor() const
{
	return sf::Color(88, 95, 179);
}

const std::string CategorySystem::GetUnlocalizedName() const
{
	return CATEGORY_SYSTEM;
}

const std::string CategorySystem::GetDisplayName() const
{
	return "System";
}

const sf::Color CategoryUtility::GetColor() const
{
	return sf::Color(205, 57, 51);
}

const std::string CategoryUtility::GetUnlocalizedName() const
{
	return CATEGORY_UTILITY;
}

const std::string CategoryUtility::GetDisplayName() const
{
	return "Utility";
}

const sf::Color CategoryGraphics::GetColor() const
{
	return sf::Color(230, 143, 46);
}

const std::string CategoryGraphics::GetUnlocalizedName() const
{
	return CATEGORY_GRAPHICS;
}

const std::string CategoryGraphics::GetDisplayName() const
{
	return "Graphics";
}

const sf::Color CategoryBooleans::GetColor() const
{
	return sf::Color(230, 103, 67);
}

const std::string CategoryBooleans::GetUnlocalizedName() const
{
	return CATEGORY_BOOLEANS;
}

const std::string CategoryBooleans::GetDisplayName() const
{
	return "Booleans";
}

const sf::Color CategoryReals::GetColor() const
{
	return sf::Color(79, 154, 62);
}

const std::string CategoryReals::GetUnlocalizedName() const
{
	return CATEGORY_REALS;
}

const std::string CategoryReals::GetDisplayName() const
{
	return "Reals";
}

const sf::Color CategoryStrings::GetColor() const
{
	return sf::Color(16, 167, 167);
}

const std::string CategoryStrings::GetUnlocalizedName() const
{
	return CATEGORY_STRINGS;
}

const std::string CategoryStrings::GetDisplayName() const
{
	return "Strings";
}
