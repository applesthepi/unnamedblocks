#include "ITransformable.h"

void ITransformable::setSize(const sf::Vector2u& size)
{
	m_size = size;
}

const sf::Vector2u& ITransformable::getSize()
{
	return m_size;
}
