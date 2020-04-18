#pragma once
#include <Espresso/Espresso.h>

#define CATEGORY_MAIN "test_category"

class SomeCategory : public ModCatagory
{
public:
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
		return "Test Mod";
	}
};