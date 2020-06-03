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

#define REAL_GEOMETRY_REACH 5

class ArgumentReal : public Argument
{
public:
	ArgumentReal(const BlockArgumentVariableMode& mode, bool canSwitch)
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
		return m_field.GetWidth() + (REAL_GEOMETRY_REACH * 2);
	}

	bool HasData() override
	{
		return true;
	}

	void Select() override
	{
		if (m_field.GetSelected())
			m_field.mouseButton(true, Global::MousePosition, sf::Mouse::Button::Left);
		else
		{
			m_field.SelectAll();
		}
	}

	void UnSelect() override
	{
		UIRegistry::GetRegistry().RemoveComponent(&m_field);
	}

	void UpdateVertexArray() override
	{
		sf::Color col;
		uint16_t height = Global::BlockHeight - Global::BlockBorder;
		const uint32_t width = m_field.GetWidth();
		const std::string& text = m_field.GetText();

		if (m_mode == BlockArgumentVariableMode::RAW)
			col = MOD_BUTTON_TEXT_BG;
		else if (m_mode == BlockArgumentVariableMode::VAR)
			col = MOD_BUTTON_TEXT_BG_ACCENT_STRONG;

		col = sf::Color::Green;

		// ===================================================================================================
		// =============== Prepare Vertex Buffer Texture
		// ===================================================================================================

		//wchar_t* wstr = (wchar_t*)malloc(sizeof(wchar_t) * text.length());
		//std::mbstowcs(wstr, text.c_str(), text.length());
		//
		//for (uint64_t i = 0; i < text.length(); i++)
		//{
		//	Global::Font.getGlyph(wstr[i], Global::BlockHeight - Global::BlockBorder, false).textureRect;
		//}
		
		//sf::Text textRecreation = sf::Text(text, Global::Font, height);
		//textRecreation.setFillColor(sf::Color(0, 0, 0, 255));
		//
		//uint32_t textWidth = textRecreation.getLocalBounds().width;
		//uint32_t textHeight = textRecreation.getLocalBounds().height;
		//
		//if (textWidth == 0 || textHeight == 0)
		//{
		//	textWidth = 1;
		//	textHeight = Global::BlockHeight - Global::BlockBorder;
		//}

		m_textRendered.create(100, Global::BlockHeight - Global::BlockBorder);
		m_textRendered.clear(sf::Color(50, 50, 150, 255));
		//m_textRendered.draw(textRecreation);
		m_vertexArrayImage = m_textRendered.getTexture().copyToImage();

		// ===================================================================================================
		// =============== Update Vertex Array; see "dev/real_geometry.png"
		// ===================================================================================================

		//ResetVertices(18);
		ResetVertices(12);
		
		const sf::Vector2f vertices0[] = {
			sf::Vector2f(0, 0),
			sf::Vector2f(REAL_GEOMETRY_REACH, 0),
			sf::Vector2f(REAL_GEOMETRY_REACH, height / 2),
		};

		const sf::Vector2f vertices1[] = {
			sf::Vector2f(0, height),
			sf::Vector2f(REAL_GEOMETRY_REACH, height / 2),
			sf::Vector2f(REAL_GEOMETRY_REACH, height),
		};

		const sf::Vector2f vertices2[] = {
			sf::Vector2f(width + REAL_GEOMETRY_REACH + REAL_GEOMETRY_REACH, 0),
			sf::Vector2f(width + REAL_GEOMETRY_REACH, height / 2),
			sf::Vector2f(width + REAL_GEOMETRY_REACH, 0),
		};

		const sf::Vector2f vertices3[] = {
			sf::Vector2f(width + REAL_GEOMETRY_REACH + REAL_GEOMETRY_REACH, height),
			sf::Vector2f(width + REAL_GEOMETRY_REACH, height),
			sf::Vector2f(width + REAL_GEOMETRY_REACH, height / 2)
		};

		const sf::Color colors[] = {
			col,
			col,
			col
		};

		AddTriangle(vertices0, colors);
		AddTriangle(vertices1, colors);
		AddTriangle(vertices2, colors);
		AddTriangle(vertices3, colors);
	}

	bool UseVertexArrayTexture() override
	{
		return true;
	}
protected:
	void UpdateData() override
	{
		m_field.SetText(m_data);
	}
private:
	Field m_field;
	std::function<void()> m_tab;
	sf::RenderTexture m_textRendered;
};
