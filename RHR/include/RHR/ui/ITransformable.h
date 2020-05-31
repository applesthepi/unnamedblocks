#pragma once
#include <SFML/Graphics/Transformable.hpp>

class ITransformable : public sf::Transformable
{
public:
	void setSize(const sf::Vector2u& size);
	const sf::Vector2u& getSize();
protected:
	sf::Vector2u m_size;
};