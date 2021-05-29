#pragma once
#include "Button.h"

class ButtonText : public Button
{
public:
	ButtonText(std::function<void()>* callback, const std::string& text, uint16_t charSize, const sf::Vector2f& size, const sf::Color& background, const sf::Color& forground);

	void SetText(const std::string& text);

	void frameUpdate(double deltaTime) override;
	bool mouseButton(bool down, const sf::Vector2i& position, const sf::Mouse::Button& button) override;
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates state) const override;
private:
	sf::RectangleShape m_background;
	sf::Text m_text;
	
	bool m_wasDown;

	sf::Color m_colorBackground;
	sf::Color m_colorForground;
};
