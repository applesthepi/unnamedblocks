#pragma once
#include <Espresso/Espresso.h>

#define CATEGORY_MAIN "vin_category"
#define CATEGORY_OTHER "vin_category_other"

class SomeCategory : public ModCatagory
{
public:
	SomeCategory(const std::string& modUnlocalizedName, const std::string& modDisplayName)
		:ModCatagory(modUnlocalizedName, modDisplayName)
	{

	}

	const sf::Color GetColor() const override
	{
		return sf::Color(255, 140, 0);
	}

	const std::string GetUnlocalizedName() const override
	{
		return CATEGORY_MAIN;
	}

	const std::string GetDisplayName() const override
	{
		return "Main Test Category";
	}
};

class SomeOtherCategory : public ModCatagory
{
public:
	SomeOtherCategory(const std::string& modUnlocalizedName, const std::string& modDisplayName)
		:ModCatagory(modUnlocalizedName, modDisplayName)
	{

	}

	const sf::Color GetColor() const override
	{
		return sf::Color(0, 182, 255);
	}

	const std::string GetUnlocalizedName() const override
	{
		return CATEGORY_OTHER;
	}

	const std::string GetDisplayName() const override
	{
		return "Second Test Category";
	}
};