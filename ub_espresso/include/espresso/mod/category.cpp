#include "category.hpp"

ModCatagory::ModCatagory(const std::string& modUnlocalizedName, const std::string& modDisplayName)
	:m_modUnlocalizedName(modUnlocalizedName), m_modDisplayName(modDisplayName)
{

}

std::string ModCatagory::GetModUnlocalizedName()
{
	return m_modUnlocalizedName;
}

std::string ModCatagory::GetModDisplayName()
{
	return m_modDisplayName;
}

cap::color ModCatagory::GetColor() const
{
	return cap::color().from_u8({ 255, 0, 255, 255 });
}

std::string ModCatagory::GetUnlocalizedName() const
{
	return "NULL";
}

std::string ModCatagory::GetDisplayName() const
{
	return "NULL";
}
