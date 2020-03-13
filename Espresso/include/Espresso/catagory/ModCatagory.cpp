#include "ModCatagory.h"

ModCatagory::ModCatagory()
{

}

const sf::Color ModCatagory::GetColor() const
{
	return sf::Color::Magenta;
}

const std::string ModCatagory::GetUnlocalizedName() const
{
	return "vin_null";
}

const std::string ModCatagory::GetDisplayName() const
{
	return "NULL";
}
