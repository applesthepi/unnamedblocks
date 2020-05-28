#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

class Button : public sf::Drawable, public sf::Transformable
{
public:
	Button(std::function<void()>* callback);

	virtual void FrameUpdate();
protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const override;

	std::function<void()>* m_callback;
};