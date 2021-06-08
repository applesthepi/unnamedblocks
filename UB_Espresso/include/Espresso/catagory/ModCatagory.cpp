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

const glm::vec<3, uint8_t>&& ModCatagory::GetColor() const
{
	return std::move(glm::vec<3, uint8_t>(255, 0, 255));
}

const std::string ModCatagory::GetUnlocalizedName() const
{
	return "NULL";
}

const std::string ModCatagory::GetDisplayName() const
{
	return "NULL";
}
