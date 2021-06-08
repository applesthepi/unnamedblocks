#pragma once
#include "config.h"

#include <Cappuccino/Utils.hpp>

class ModCatagory
{
public:
	ModCatagory(const std::string& modUnlocalizedName, const std::string& modDisplayName);
	const std::string GetModUnlocalizedName();
	const std::string GetModDisplayName();

	// the color of the category, and the background color of all of the blocks registered to this category
	virtual const glm::vec<3, uint8_t>&& GetColor() const;

	// the name the engine uses to refer to this category as. Convention example: "mymod_some_category"
	virtual const std::string GetUnlocalizedName() const;

	// what will be displayed on the category button as text. Convention example: "Some Category"
	virtual const std::string GetDisplayName() const;
private:
	const std::string m_modUnlocalizedName;
	const std::string m_modDisplayName;
};
