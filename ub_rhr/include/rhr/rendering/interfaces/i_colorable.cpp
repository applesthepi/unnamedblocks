#include "IColorable.hpp"

IColorable::IColorable(const Color& color)
	:m_Color(color)
{

}

void IColorable::SetColor(const Color& color)
{
	m_Color = color;
	PostColorUpdate();
}

const Color& IColorable::GetColor()
{
	return m_Color;
}

void IColorable::PostColorUpdate()
{

}
