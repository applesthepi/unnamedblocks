#include "ModCategories.hpp"

Color CategorySystem::GetColor() const
{
	return Color().FromU8({ 88, 95, 179, 255 });
}

std::string CategorySystem::GetUnlocalizedName() const
{
	return CATEGORY_SYSTEM;
}

std::string CategorySystem::GetDisplayName() const
{
	return "System";
}

Color CategoryUtility::GetColor() const
{
	return Color().FromU8({ 205, 57, 51, 255 });
}

std::string CategoryUtility::GetUnlocalizedName() const
{
	return CATEGORY_UTILITY;
}

std::string CategoryUtility::GetDisplayName() const
{
	return "Utility";
}

Color CategoryGraphics::GetColor() const
{
	return Color().FromU8({ 230, 143, 46, 255 });
}

std::string CategoryGraphics::GetUnlocalizedName() const
{
	return CATEGORY_GRAPHICS;
}

std::string CategoryGraphics::GetDisplayName() const
{
	return "Graphics";
}

Color CategoryBooleans::GetColor() const
{
	return Color().FromU8({ 230, 103, 67, 255 });
}

std::string CategoryBooleans::GetUnlocalizedName() const
{
	return CATEGORY_BOOLEANS;
}

std::string CategoryBooleans::GetDisplayName() const
{
	return "Booleans";
}

Color CategoryReals::GetColor() const
{
	return Color().FromU8({ 79, 154, 62, 255 });
}

std::string CategoryReals::GetUnlocalizedName() const
{
	return CATEGORY_REALS;
}

std::string CategoryReals::GetDisplayName() const
{
	return "Reals";
}

Color CategoryStrings::GetColor() const
{
	return Color().FromU8({ 16, 167, 167, 255 });
}

std::string CategoryStrings::GetUnlocalizedName() const
{
	return CATEGORY_STRINGS;
}

std::string CategoryStrings::GetDisplayName() const
{
	return "Strings";
}
