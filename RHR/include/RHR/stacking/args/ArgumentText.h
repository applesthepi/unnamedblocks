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
			{
				Global::FontBold.getGlyph(m_text[i], Global::BlockHeight, false);
				m_width += Global::Font.getGlyph(m_text[i], Global::BlockHeight, false).advance;
			}

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

		uint32_t offset = 0;

		for (uint32_t i = 0; i < m_text.length(); i++)
		{
			Global::FontBold.getGlyph(m_text[i], Global::BlockHeight, false);
			sf::Glyph gl = Global::Font.getGlyph(m_text[i], Global::BlockHeight, false);
			GetVAO().AddChar(gl, m_text[i], &offset, sf::Color::Black);
		}
	}

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
};
