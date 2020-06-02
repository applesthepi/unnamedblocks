#pragma once
#include "Argument.h"

#include <Espresso/Global.h>
/*
class ArgumentText : public Argument
{
public:
	ArgumentText(const sf::Vector2u& relitivePosition)
		:Argument(relitivePosition)
	{
		m_textAgent = sf::Text(m_text, *Global::Font, Global::BlockHeight - 10);

		m_textAgent.setFillColor(sf::Color::Black);

		m_textAgent.setString(m_text);
		m_textAgent.setCharacterSize(Global::BlockHeight - Global::BlockBorder);
		m_textAgent.setPosition(GetAbsolutePosition().x, GetAbsolutePosition().y);
	}

	void FrameUpdate() override
	{
		m_textAgent.setString(m_text);
		m_textAgent.setCharacterSize(Global::BlockHeight - Global::BlockBorder);
		m_textAgent.setPosition(GetRelitivePosition().x, GetRelitivePosition().y);
	}

	void Render(sf::RenderTexture* render) override
	{
		render->draw(m_textAgent);
	}

	unsigned int GetArgumentRawWidth() override
	{
		return m_textAgent.getLocalBounds().width;
	}

	void SetData(const std::string& data) override
	{
		m_text = data;
		m_textAgent.setString(m_text);
	}

	const BlockArgumentVariableMode GetMode() override
	{
		return BlockArgumentVariableMode::RAW;
	}

	const BlockArgumentType GetType() override
	{
		return BlockArgumentType::TEXT;
	}
private:
	std::string m_text;
	sf::Text m_textAgent;
};
*/