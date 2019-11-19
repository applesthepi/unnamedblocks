#include "Argument.h"
#include "Global.h"

Argument::Argument(sf::Vector2u relitivePosition)
{
	m_relitivePosition = relitivePosition;
}

void Argument::Render(sf::RenderWindow* window)
{
	sf::RectangleShape basic = sf::RectangleShape(sf::Vector2f(GetArgumentRawWidth(), Global::BlockHeight));
	basic.setFillColor(sf::Color::Magenta);
	basic.setPosition(m_absolutePosition.x, m_absolutePosition.y);

	window->draw(basic);
}

unsigned int Argument::GetArgumentRawWidth()
{
	return 0;
}

void Argument::Update(const sf::Vector2u* blockPosition)
{
	m_absolutePosition = sf::Vector2u(blockPosition->x + m_relitivePosition.x, blockPosition->y + m_relitivePosition.y);
}

sf::Vector2u Argument::GetAbsolutePosition()
{
	return m_absolutePosition;
}

sf::Vector2u Argument::GetRelitivePosition()
{
	return m_relitivePosition;
}
