#pragma once

#include <SFML/Graphics.hpp>

class Argument
{
public:
	Argument(sf::Vector2u relitivePosition);

	virtual void Render(sf::RenderWindow* window);
	virtual unsigned int GetArgumentRawWidth();
	void Update(const sf::Vector2u* blockPosition);

	sf::Vector2u GetAbsolutePosition();
	sf::Vector2u GetRelitivePosition();
private:
	sf::Vector2u m_relitivePosition;
	sf::Vector2u m_absolutePosition;
};