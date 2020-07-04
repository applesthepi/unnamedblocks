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
	ArgumentText()
		:Argument()
	{
		m_widthNeedsUpdate = false;
		UpdateVertexArray();
	}

	BlockArgumentType GetType() override
	{
		return BlockArgumentType::TEXT;
	}

	uint32_t GetWidth() override
	{
		if (m_widthNeedsUpdate)
		{
			m_widthNeedsUpdate = false;
			m_width = 0;

			for (uint32_t i = 0; i < m_text.length(); i++)
				m_width += Global::Font.getGlyph(m_text[i], Global::BlockHeight, false).advance;

			return m_width;
		}
		else
			return m_width;
	}

	bool HasData() override
	{
		return false;
	}

	void UpdateVertexArray() override
	{
		sf::Color col;

		if (m_mode == BlockArgumentVariableMode::RAW)
			col = MOD_BUTTON_TEXT_BG;
		else if (m_mode == BlockArgumentVariableMode::VAR)
			col = MOD_BUTTON_TEXT_BG_ACCENT_STRONG;

		// ===================================================================================================
		// =============== Prepare Vertex Buffer Texture
		// ===================================================================================================

		uint32_t textHeight = Global::BlockHeight * 2;
		uint32_t textWidth = GetWidth() * 2;

		if (textWidth == 0 || textHeight == 0)
		{
			textWidth = 1;
			textHeight = Global::BlockHeight - Global::BlockBorder;
		}

		//m_text.setPosition(static_cast<float>(Global::BlockBorder) / 2.0f, -6.0f);
		//
		//m_textRendered.create(textWidth, textHeight);
		//m_textRendered.clear(sf::Color(0, 0, 0, 0));
		//m_textRendered.draw(m_text);
		//m_vertexArrayImage = m_textRendered.getTexture().copyToImage();

		// ===================================================================================================
		// =============== Update Vertex Array; see "dev/string_geometry.png"
		// ===================================================================================================

		//ResetVertices(6);

		//const sf::Vector2f vertices0[] = {
		//	sf::Vector2f(0, 0),
		//	sf::Vector2f(textWidth / 2, 0),
		//	sf::Vector2f(textWidth / 2, textHeight / 2)
		//};
		//
		//const sf::Vector2f vertices1[] = {
		//	sf::Vector2f(0, 0),
		//	sf::Vector2f(textWidth / 2, textHeight / 2),
		//	sf::Vector2f(0, textHeight / 2)
		//};

		//const sf::Vector2f textureCoords0[] = {
		//	sf::Vector2f(0, 0),
		//	sf::Vector2f(textWidth, 0),
		//	sf::Vector2f(textWidth, textHeight)
		//};
		//
		//const sf::Vector2f textureCoords1[] = {
		//	sf::Vector2f(0, 0),
		//	sf::Vector2f(textWidth, textHeight),
		//	sf::Vector2f(0, textHeight)
		//};
		//
		//AddTriangle(vertices0, textureCoords0);
		//AddTriangle(vertices1, textureCoords1);

		uint32_t offset = 0;

		for (uint32_t i = 0; i < m_text.length(); i++)
		{
			sf::Glyph gl = Global::Font.getGlyph(m_text[i], Global::BlockHeight, false);
			GetVAO().AddChar(gl, m_text[i], &offset);
		}
	}

	//bool UseVertexArrayTexture() override
	//{
	//	return true;
	//}

	void UpdateData() override
	{
		m_text = m_data;
		m_widthNeedsUpdate = true;
		UpdateVertexArray();
	}
private:
	std::string m_text;
	uint32_t m_width;
	bool m_widthNeedsUpdate;
	std::function<void()> m_tab;
	sf::RenderTexture m_textRendered;
};
