#include "IDiColorable.hpp"

IDiColorable::IDiColorable(const Color& primary, const Color& secondary)
	:m_ColorPrimary(primary), m_ColorSecondary(secondary)
{

}

void IDiColorable::SetColorPrimary(const Color& primary)
{
	m_ColorPrimary = primary;
}

void IDiColorable::SetColorSecondary(const Color& secondary)
{
	m_ColorSecondary = secondary;
}

const Color& IDiColorable::GetColorPrimary()
{
	return m_ColorPrimary;
}

const Color& IDiColorable::GetColorSecondary()
{
	return m_ColorSecondary;
}