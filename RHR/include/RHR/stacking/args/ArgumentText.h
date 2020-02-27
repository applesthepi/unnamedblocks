#pragma once

#include "Argument.h"
#include "Global.h"

class ArgumentText : public Argument
{
public:
	ArgumentText(sf::Vector2u relitivePosition)
		:Argument(relitivePosition)
	{
		m_TextAgent = sf::Text(m_Text, *Global::Font, Global::BlockHeight - 10);

		m_TextAgent.setFillColor(sf::Color::Black);

		m_TextAgent.setString(m_Text);
		m_TextAgent.setCharacterSize(Global::BlockHeight - Global::BlockBorder);
		m_TextAgent.setPosition(GetAbsolutePosition().x, GetAbsolutePosition().y);
	}

	void FrameUpdate() override
	{
		m_TextAgent.setString(m_Text);
		m_TextAgent.setCharacterSize(Global::BlockHeight - Global::BlockBorder);
		//m_TextAgent.setPosition(GetAbsolutePosition().x, GetAbsolutePosition().y);
		m_TextAgent.setPosition(GetRelitivePosition().x, GetRelitivePosition().y);
	}

	void Render(sf::RenderTexture* render) override
	{
		render->draw(m_TextAgent);
	}

	unsigned int GetArgumentRawWidth() override
	{
		return m_TextAgent.getLocalBounds().width;
	}

	void SetData(std::string data) override
	{
		m_Text = data;
		m_TextAgent.setString(m_Text);
	}

	BlockArgumentVariableMode* GetMode() override
	{
		m_VMode = BlockArgumentVariableMode::RAW;
		return &m_VMode;
	}

private:
	std::string m_Text;
	BlockArgumentVariableMode m_VMode;
	sf::Text m_TextAgent;
};
