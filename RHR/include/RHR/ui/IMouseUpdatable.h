#pragma once
#include <SFML/Graphics.hpp>

class IMouseUpdatable
{
public:
	virtual const bool mouseButton(bool down, const sf::Vector2i& position, const sf::Mouse::Button& button);
};