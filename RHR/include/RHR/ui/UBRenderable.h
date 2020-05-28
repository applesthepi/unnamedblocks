#pragma once
#include <SFML/Graphics/Drawable.hpp>

class UBRenderable : public sf::Drawable
{
public:
	virtual void frameUpdate(const double& deltaTime);
protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};