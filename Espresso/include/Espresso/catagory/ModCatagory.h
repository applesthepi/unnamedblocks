#pragma once

#include <string>
#include <SFML/Graphics.hpp>

class ModCatagory
{
public:
	ModCatagory();

	// the color of the category, and the background color of all of the blocks registered to this category
	virtual const sf::Color GetColor() const;

	// the name the engine uses to refer to this category as. Convention example: "mymod_some_category"
	virtual const std::string GetUnlocalizedName() const;

	// what will be displayed on the category button as text. Convention example: "Some Category"
	virtual const std::string GetDisplayName() const;
};
