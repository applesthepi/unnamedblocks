#pragma once
#include <SFML/Graphics/Drawable.hpp>

class IRenderable : public sf::Drawable
{
public:
	virtual void frameUpdate(double deltaTime);
};