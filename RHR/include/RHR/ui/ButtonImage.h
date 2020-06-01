#pragma once
#include "Button.h"

class ButtonImage : public Button
{
public:
	ButtonImage(std::function<void()>* callback, const std::string& path, const sf::Vector2f& size);

	void SetImage(const std::string& path);

	void frameUpdate(double deltaTime) override;
	bool mouseButton(bool down, const sf::Vector2i& position, const sf::Mouse::Button& button) override;
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates state) const override;
private:
	sf::Texture m_texture;
	sf::Sprite m_sprite;

	bool m_wasDown;
};
