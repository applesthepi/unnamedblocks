#pragma once

#include "Argument.h"
#include "Global.h"

class ArgumentText : public Argument
{
public:
	ArgumentText(sf::Vector2u relitivePosition, std::string text)
		:Argument(relitivePosition)
	{
		m_Text = text;
		m_TextAgent = sf::Text(text, *Global::Font, Global::BlockHeight - 10);

		m_TextAgent.setFillColor(sf::Color::Black);
		m_TextAgent.setString(m_Text);
		m_TextAgent.setCharacterSize(Global::BlockHeight - Global::BlockBorder);
		m_TextAgent.setPosition(GetAbsolutePosition().x, GetAbsolutePosition().y);
	}

	void FrameUpdate(sf::RenderWindow* window) override
	{
		m_TextAgent.setString(m_Text);
		m_TextAgent.setCharacterSize(Global::BlockHeight - Global::BlockBorder);
		m_TextAgent.setPosition(GetAbsolutePosition().x, GetAbsolutePosition().y);
	}

	void Render(sf::RenderTexture* render, sf::RenderWindow* window) override
	{
		if (render == nullptr)
		{
			window->draw(m_TextAgent);
		}
		else
		{
			render->draw(m_TextAgent);
		}
	}

	unsigned int GetArgumentRawWidth() override
	{
		return m_TextAgent.getLocalBounds().width;
	}

private:
	std::string m_Text;
	sf::Text m_TextAgent;
};