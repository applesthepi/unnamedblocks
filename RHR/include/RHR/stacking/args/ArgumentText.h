#pragma once
#include "Argument.h"
#include "RHR/ui/Field.h"
#include "RHR/registries/UIRegistry.h"

#include <Espresso/Global.h>
#include <Espresso/InputHandler.h>
#include <SFML/Window/Clipboard.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <Cappuccino/Logger.h>
#include <cstdlib>

class ArgumentText : public Argument
{
public:
	ArgumentText(bool canSwitch)
		:Argument()
	{
		m_text = sf::Text("", Global::Font, Global::BlockHeight - Global::BlockBorder);
		m_text.setFillColor(sf::Color::Black);

		UpdateVertexArray();
	}

	BlockArgumentType GetType() override
	{
		return BlockArgumentType::TEXT;
	}

	uint32_t GetWidth() override
	{
		return m_text.getLocalBounds().width;
	}

	bool HasData() override
	{
		return false;
	}

	void UpdateVertexArray() override
	{
		uint16_t height = Global::BlockHeight - Global::BlockBorder;
		const uint32_t width = m_text.getLocalBounds().width;

		sf::Color col;

		if (m_mode == BlockArgumentVariableMode::RAW)
			col = MOD_BUTTON_TEXT_BG;
		else if (m_mode == BlockArgumentVariableMode::VAR)
			col = MOD_BUTTON_TEXT_BG_ACCENT_STRONG;

		// ===================================================================================================
		// =============== Prepare Vertex Buffer Texture
		// ===================================================================================================

		uint32_t textWidth = width;
		uint32_t textHeight = height;

		if (textWidth == 0 || textHeight == 0)
		{
			textWidth = 1;
			textHeight = Global::BlockHeight - Global::BlockBorder;
		}

		m_text.setPosition(0, -2);

		m_textRendered.create(textWidth, textHeight);
		m_textRendered.clear(sf::Color(0, 0, 0, 0));
		m_textRendered.draw(m_text);
		m_vertexArrayImage = m_textRendered.getTexture().copyToImage();

		// ===================================================================================================
		// =============== Update Vertex Array; see "dev/string_geometry.png"
		// ===================================================================================================

		ResetVertices(6);

		const sf::Vector2f vertices0[] = {
			sf::Vector2f(0, 0),
			sf::Vector2f(width, 0),
			sf::Vector2f(width, height)
		};

		const sf::Vector2f vertices1[] = {
			sf::Vector2f(0, 0),
			sf::Vector2f(width, height),
			sf::Vector2f(0, height)
		};

		const sf::Vector2f textureCoords0[] = {
			sf::Vector2f(0, 0),
			sf::Vector2f(width, 0),
			sf::Vector2f(width, height)
		};

		const sf::Vector2f textureCoords1[] = {
			sf::Vector2f(0, 0),
			sf::Vector2f(width, height),
			sf::Vector2f(0, height)
		};

		AddTriangle(vertices0, textureCoords0);
		AddTriangle(vertices1, textureCoords1);
	}

	bool UseVertexArrayTexture() override
	{
		return true;
	}

	void UpdateData() override
	{
		m_text.setString(m_data);
		UpdateVertexArray();
	}
private:
	sf::Text m_text;
	std::function<void()> m_tab;
	sf::RenderTexture m_textRendered;
};
