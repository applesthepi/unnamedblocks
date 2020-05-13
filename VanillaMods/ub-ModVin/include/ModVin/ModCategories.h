#pragma once
#include <Espresso/Espresso.h>

#define CATEGORY_SYSTEM "vin_system"
#define CATEGORY_OBJECTS "vin_objects"
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

class CategoryObjects : public ModCatagory
{
public:
	CategoryObjects(const std::string& modUnlocalizedName, const std::string& modDisplayName)
		:ModCatagory(modUnlocalizedName, modDisplayName) {}

	const sf::Color GetColor() const override
	{
		return sf::Color(150, 150, 150);
	}

	const std::string GetUnlocalizedName() const override
	{
		return CATEGORY_OBJECTS;
	}

	const std::string GetDisplayName() const override
	{
		return "Objects";
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