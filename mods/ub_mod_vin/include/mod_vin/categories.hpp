#pragma once
#include "config.h"

#include <espresso/mod/category.hpp>

#define CATEGORY_SYSTEM "vin_system"
#define CATEGORY_UTILITY "vin_utility"
#define CATEGORY_GRAPHICS "vin_graphics"
#define CATEGORY_BOOLEANS "vin_booleans"
#define CATEGORY_REALS "vin_reals"
#define CATEGORY_STRINGS "vin_strings"

class CategorySystem : public esp::mod::category
{
public:
	CategorySystem(const std::string& modUnlocalizedName, const std::string& modDisplayName)
		:esp::mod::category(modUnlocalizedName, modDisplayName) {}

	cap::color get_color() const override;
	std::string get_unlocalized_name() const override;
	std::string get_display_name() const override;
};

class CategoryUtility : public esp::mod::category
{
public:
	CategoryUtility(const std::string& modUnlocalizedName, const std::string& modDisplayName)
		:esp::mod::category(modUnlocalizedName, modDisplayName) {}

	cap::color get_color() const override;
	std::string get_unlocalized_name() const override;
	std::string get_display_name() const override;
};

class CategoryGraphics : public esp::mod::category
{
public:
	CategoryGraphics(const std::string& modUnlocalizedName, const std::string& modDisplayName)
		:esp::mod::category(modUnlocalizedName, modDisplayName) {}

	cap::color get_color() const override;
	std::string get_unlocalized_name() const override;
	std::string get_display_name() const override;
};

class CategoryBooleans : public esp::mod::category
{
public:
	CategoryBooleans(const std::string& modUnlocalizedName, const std::string& modDisplayName)
		:esp::mod::category(modUnlocalizedName, modDisplayName) {}

	cap::color get_color() const override;
	std::string get_unlocalized_name() const override;
	std::string get_display_name() const override;
};

class CategoryReals : public esp::mod::category
{
public:
	CategoryReals(const std::string& modUnlocalizedName, const std::string& modDisplayName)
		:esp::mod::category(modUnlocalizedName, modDisplayName) {}

	cap::color get_color() const override;
	std::string get_unlocalized_name() const override;
	std::string get_display_name() const override;
};

class CategoryStrings : public esp::mod::category
{
public:
	CategoryStrings(const std::string& modUnlocalizedName, const std::string& modDisplayName)
		:esp::mod::category(modUnlocalizedName, modDisplayName) {}

	cap::color get_color() const override;
	std::string get_unlocalized_name() const override;
	std::string get_display_name() const override;
};