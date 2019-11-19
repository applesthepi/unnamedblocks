#pragma once

#include "registries/BlockRegistry.h"
#include "args/Argument.h"

#include <SFML/Graphics.hpp>

class Block
{
public:
	Block(std::string type);

	void Render(sf::RenderWindow* window);
private:
	std::vector<Argument*> m_args;
	sf::Vector2u m_position;
};