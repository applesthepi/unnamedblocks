#include "IColorable.hpp"

IColorable::IColorable(const Color&& color)
	:m_Color(color)
{

}

void IColorable::SetColor(const Color&& color)
{
	m_Color = std::move(color);
}

const Color& IColorable::GetColor()
{
	return m_Color;
}