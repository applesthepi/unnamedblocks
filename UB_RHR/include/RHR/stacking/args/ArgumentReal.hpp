#pragma once
#include "config.h"

#include "stacking/args/Argument.hpp"
#include "registries/UIRegistry.hpp"

#include <Cappuccino/Utils.hpp>
#include <Espresso/InputHandler.hpp>

#define REAL_GEOMETRY_REACH 5

class ArgumentReal : public Argument
{
public:
	ArgumentReal(bool canSwitch)
		:Argument(), m_field("", canSwitch, FieldType::NUMBER)
	{
		UpdateVertexArray();

		m_tab = [this]()
		{
			m_next = true;
		};

		m_field.SetTab(&m_tab);
	}

	BlockArgumentType GetType() override
	{
		return BlockArgumentType::REAL;
	}

	uint32_t GetWidth() override
	{
		if (m_widthNeedsUpdate)
		{
			m_widthNeedsUpdate = false;
			m_width = 0;

			for (uint32_t i = 0; i < m_field.GetText().length(); i++)
			{
				Global::FontBold.getGlyph(m_field.GetText()[i], Global::BlockHeight, false);
				m_width += Global::Font.getGlyph(m_field.GetText()[i], Global::BlockHeight - Global::BlockBorder, false).advance;
			}

			return m_width + (REAL_GEOMETRY_REACH * 2) + (Global::BlockBorder * 2);
		}
		else
			return m_width + (REAL_GEOMETRY_REACH * 2) + (Global::BlockBorder * 2);
	}

	bool HasData() override
	{
		return true;
	}

	void Select() override
	{
		if (m_field.GetSelected())
		{
			//m_field.mouseButton(true, Global::MousePosition, sf::Mouse::Button::Left);
		}
		else
		{
			UIRegistry::GetRegistry().AddComponent(&m_field);
			m_field.SelectAll();
		}
	}

	void UnSelect() override
	{
		UIRegistry::GetRegistry().RemoveComponent(&m_field);
	}

	void UpdateVertexArray() override
	{
		uint32_t textHeight = Global::BlockHeight - Global::BlockBorder;
		uint32_t textWidth = GetWidth() - (REAL_GEOMETRY_REACH * 2);

		sf::Color col;
		const std::string& text = m_field.GetText();

		if (m_mode == BlockArgumentVariableMode::RAW)
			col = MOD_BUTTON_TEXT_BG;
		else if (m_mode == BlockArgumentVariableMode::VAR)
			col = MOD_BUTTON_TEXT_BG_ACCENT_STRONG;
		
		// ===================================================================================================
		// =============== Vanity Geometry
		// ===================================================================================================

		const sf::Vector2f vertices0[] = {
			sf::Vector2f(0, 0),
			sf::Vector2f(REAL_GEOMETRY_REACH, 0),
			sf::Vector2f(REAL_GEOMETRY_REACH, textHeight / 2.0f),
		};

		const sf::Vector2f vertices1[] = {
			sf::Vector2f(0, textHeight),
			sf::Vector2f(REAL_GEOMETRY_REACH, textHeight / 2.0f),
			sf::Vector2f(REAL_GEOMETRY_REACH, textHeight),
		};

		const sf::Vector2f vertices2[] = {
			sf::Vector2f(textWidth + REAL_GEOMETRY_REACH + REAL_GEOMETRY_REACH, 0),
			sf::Vector2f(textWidth + REAL_GEOMETRY_REACH, textHeight / 2.0f),
			sf::Vector2f(textWidth + REAL_GEOMETRY_REACH, 0),
		};

		const sf::Vector2f vertices3[] = {
			sf::Vector2f(textWidth + REAL_GEOMETRY_REACH + REAL_GEOMETRY_REACH, textHeight),
			sf::Vector2f(textWidth + REAL_GEOMETRY_REACH, textHeight),
			sf::Vector2f(textWidth + REAL_GEOMETRY_REACH, textHeight / 2.0f)
		};

		const sf::Color colors[] = {
			col,
			col,
			col
		};

		GetVAO().AddTriangle(vertices0, colors);
		GetVAO().AddTriangle(vertices1, colors);
		GetVAO().AddTriangle(vertices2, colors);
		GetVAO().AddTriangle(vertices3, colors);
		
		// ===================================================================================================
		// =============== Core Geometry
		// ===================================================================================================

		const sf::Vector2f vertices4[] = {
			sf::Vector2f(REAL_GEOMETRY_REACH, 0),
			sf::Vector2f(textWidth + REAL_GEOMETRY_REACH, 0),
			sf::Vector2f(textWidth + REAL_GEOMETRY_REACH, textHeight)
		};
		
		const sf::Vector2f vertices5[] = {
			sf::Vector2f(REAL_GEOMETRY_REACH, 0),
			sf::Vector2f(textWidth + REAL_GEOMETRY_REACH, textHeight),
			sf::Vector2f(REAL_GEOMETRY_REACH, textHeight)
		};

		GetVAO().AddTriangle(vertices4, colors);
		GetVAO().AddTriangle(vertices5, colors);

		uint32_t offset = REAL_GEOMETRY_REACH + Global::BlockBorder;

		for (uint32_t i = 0; i < m_field.GetText().length(); i++)
		{
			Global::FontBold.getGlyph(m_field.GetText()[i], Global::BlockHeight, false);
			sf::Glyph gl = Global::Font.getGlyph(m_field.GetText()[i], Global::BlockHeight - Global::BlockBorder, false);
			GetVAO().AddChar(gl, m_field.GetText()[i], &offset, MOD_BUTTON_TEXT_FG);
		}
	}

	void UpdateData() override
	{
		m_field.SetText(m_data);
		m_widthNeedsUpdate = true;
		UpdateVertexArray();
	}
private:
	Field m_field;
	uint32_t m_width;
	bool m_widthNeedsUpdate;
	std::function<void()> m_tab;
};
