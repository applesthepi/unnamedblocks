#pragma once

#include "Argument.h"
#include "Global.h"

class ArgumentText : public Argument
{
public:
	ArgumentText(sf::Vector2u relitivePosition, const char* text)
		:Argument(relitivePosition)
	{
		m_Text = text;
		m_TextAgent = sf::Text(text, *Global::Font, Global::BlockHeight - 10);

		m_TextAgent.setFillColor(sf::Color::Black);

		m_background = sf::RectangleShape(sf::Vector2f(GetArgumentRawWidth(), Global::BlockHeight));
		m_background.setFillColor(sf::Color::White);
	}

	void Render(sf::RenderWindow* window) override
	{
		m_background.setSize(sf::Vector2f(GetArgumentRawWidth(), Global::BlockHeight));
		m_background.setPosition(GetAbsolutePosition().x, GetAbsolutePosition().y);

		m_TextAgent.setString(m_Text);
		m_TextAgent.setCharacterSize(Global::BlockHeight - 10);
		m_TextAgent.setPosition(GetAbsolutePosition().x, GetAbsolutePosition().y);

		window->draw(m_background);
		window->draw(m_TextAgent);
	}

	unsigned int GetArgumentRawWidth() override
	{
		return m_TextAgent.getLocalBounds().width;
	}

private:
	const char* m_Text;
	sf::Text m_TextAgent;
	sf::RectangleShape m_background;
};