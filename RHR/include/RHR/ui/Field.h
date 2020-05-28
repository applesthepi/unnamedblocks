#pragma once
#include "UBRenderable.h"
#include "MouseUpdatable.h"

#include <SFML/Graphics.hpp>
#include <string>

class Field : public UBRenderable, public MouseUpdatable, public sf::Transformable
{
public:
	Field(const std::string& text);

	void frameUpdate(const double& deltaTime) override;
	const bool mouseButton(const bool& down, const sf::Vector2i& position, const sf::Mouse::Button& button) override;
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates state) const override;
private:
	std::string m_text;
	sf::Text m_input;
	sf::RectangleShape m_inputBackground;
	sf::RectangleShape m_inputLocHigh;
	sf::RectangleShape m_inputLoc;
	uint64_t m_textLoc;
	uint64_t m_textLocHigh;

	bool m_enabled;
	bool m_wasDown;
	bool m_selected;
};