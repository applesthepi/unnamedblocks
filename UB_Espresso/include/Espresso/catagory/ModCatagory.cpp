#include "ModCatagory.hpp"

ModCatagory::ModCatagory(const std::string& modUnlocalizedName, const std::string& modDisplayName)
	:m_modUnlocalizedName(modUnlocalizedName), m_modDisplayName(modDisplayName)
{

}

const std::string ModCatagory::GetModUnlocalizedName()
{
	return m_modUnlocalizedName;
}

const std::string ModCatagory::GetModDisplayName()
{
	return m_modDisplayName;
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
