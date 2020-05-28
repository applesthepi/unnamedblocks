#pragma once

class MouseUpdatable
{
public:
	virtual const bool mouseButton(const bool& down, const sf::Vector2i& position, const sf::Mouse::Button& button);
};