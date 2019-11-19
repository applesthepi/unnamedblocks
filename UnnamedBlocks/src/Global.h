#pragma once

#include <SFML/Graphics/Font.hpp>

class Global
{
public:
	static void LoadDefaults()
	{
		BlockHeight = 30;
		Font = new sf::Font();
		Font->loadFromFile("res/AnonymousPro-Regular.ttf");
	}

	static unsigned int BlockHeight;
	static sf::Font* Font;
};