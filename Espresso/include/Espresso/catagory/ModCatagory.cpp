#include "ModCatagory.h"

ModCatagory::ModCatagory()
{

}

void ModCatagory::SetColor(const sf::Color& color)
{
	m_color = color;
}

void ModCatagory::SetUnlocalizedName(const std::string& unlocalizedName)
{
	m_unlocalizedName = unlocalizedName;
}

void ModCatagory::SetDisplayName(const std::string& displayName)
{
	m_displayName = displayName;
}

const sf::Color& ModCatagory::GetColor() const
{
	return m_color;
}

const std::string& ModCatagory::GetUnlocalizedName() const
{
	return m_unlocalizedName;
}

const std::string& ModCatagory::GetDisplayName() const
{
	return m_displayName;
}
