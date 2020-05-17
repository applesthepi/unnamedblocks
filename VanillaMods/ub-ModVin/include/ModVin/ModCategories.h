#pragma once
#include <Espresso/Espresso.h>

#define CATEGORY_SYSTEM "vin_system"
#define CATEGORY_UTILITY "vin_utility"
#define CATEGORY_GRAPHICS "vin_graphics"
#define CATEGORY_BOOLEANS "vin_booleans"
#define CATEGORY_REALS "vin_reals"
#define CATEGORY_STRINGS "vin_strings"

class CategorySystem : public ModCatagory
{
public:
	CategorySystem(const std::string& modUnlocalizedName, const std::string& modDisplayName)
		:ModCatagory(modUnlocalizedName, modDisplayName) {}

	const sf::Color GetColor() const override
	{
		return sf::Color(150, 150, 150);
	}

	const std::string GetUnlocalizedName() const override
	{
		return CATEGORY_SYSTEM;
	}

	const std::string GetDisplayName() const override
	{
		return "System";
	}
};

class CategoryUtility : public ModCatagory
{
public:
	CategoryUtility(const std::string& modUnlocalizedName, const std::string& modDisplayName)
		:ModCatagory(modUnlocalizedName, modDisplayName) {}

	const sf::Color GetColor() const override
	{
		return sf::Color(150, 150, 150);
	}

	const std::string GetUnlocalizedName() const override
	{
		return CATEGORY_UTILITY;
	}

	const std::string GetDisplayName() const override
	{
		return "Utility";
	}
};

class CategoryGraphics : public ModCatagory
{
public:
	CategoryGraphics(const std::string& modUnlocalizedName, const std::string& modDisplayName)
		:ModCatagory(modUnlocalizedName, modDisplayName) {}

	const sf::Color GetColor() const override
	{
		return sf::Color(150, 150, 150);
	}

	const std::string GetUnlocalizedName() const override
	{
		return CATEGORY_GRAPHICS;
	}

	const std::string GetDisplayName() const override
	{
		return "Graphics";
	}
};

class CategoryBooleans : public ModCatagory
{
public:
	CategoryBooleans(const std::string& modUnlocalizedName, const std::string& modDisplayName)
		:ModCatagory(modUnlocalizedName, modDisplayName) {}

	const sf::Color GetColor() const override
	{
		return sf::Color(150, 150, 150);
	}

	const std::string GetUnlocalizedName() const override
	{
		return CATEGORY_BOOLEANS;
	}

	const std::string GetDisplayName() const override
	{
		return "Booleans";
	}
};

class CategoryReals : public ModCatagory
{
public:
	CategoryReals(const std::string& modUnlocalizedName, const std::string& modDisplayName)
		:ModCatagory(modUnlocalizedName, modDisplayName) {}

	const sf::Color GetColor() const override
	{
		return sf::Color(150, 150, 150);
	}

	const std::string GetUnlocalizedName() const override
	{
		return CATEGORY_REALS;
	}

	const std::string GetDisplayName() const override
	{
		return "Reals";
	}
};

class CategoryStrings : public ModCatagory
{
public:
	CategoryStrings(const std::string& modUnlocalizedName, const std::string& modDisplayName)
		:ModCatagory(modUnlocalizedName, modDisplayName) {}

	const sf::Color GetColor() const override
	{
		return sf::Color(150, 150, 150);
	}

	const std::string GetUnlocalizedName() const override
	{
		return CATEGORY_STRINGS;
	}

	const std::string GetDisplayName() const override
	{
		return "Strings";
	}
};