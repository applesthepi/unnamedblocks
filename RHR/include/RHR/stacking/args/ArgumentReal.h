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
		return m_field.GetWidth() + (REAL_GEOMETRY_REACH * 2);
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
		
		uint16_t height = Global::BlockHeight - Global::BlockBorder;
		const uint32_t width = m_field.GetWidth();

		sf::Color col;
		const std::string& text = m_field.GetText();

		if (m_mode == BlockArgumentVariableMode::RAW)
			col = MOD_BUTTON_TEXT_BG;
		else if (m_mode == BlockArgumentVariableMode::VAR)
			col = MOD_BUTTON_TEXT_BG_ACCENT_STRONG;

		// ===================================================================================================
		// =============== Prepare Vertex Buffer Texture
		// ===================================================================================================

		sf::Text textRecreation = sf::Text(text, Global::Font, height);
		textRecreation.setFillColor(MOD_BUTTON_TEXT_FG);
		textRecreation.setPosition(4, -2);
		
		uint32_t textWidth = width;
		uint32_t textHeight = Global::BlockHeight;
		
		if (textWidth == 0 || textHeight == 0)
		{
			textWidth = 1;
			textHeight = Global::BlockHeight - Global::BlockBorder;
		}

		m_textRendered.create(textWidth, textHeight);
		m_textRendered.clear(col);
		m_textRendered.draw(textRecreation);
		m_vertexArrayImage = m_textRendered.getTexture().copyToImage();

		// ===================================================================================================
		// =============== Update Vertex Array; see "dev/real_geometry.png"
		// ===================================================================================================

		ResetVertices(18);
		
		// ===================================================================================================
		// =============== Vanity Geometry
		// ===================================================================================================

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

		// ===================================================================================================
		// =============== Core Geometry
		// ===================================================================================================

		const sf::Vector2f vertices4[] = {
			sf::Vector2f(REAL_GEOMETRY_REACH, 0),
			sf::Vector2f(width + REAL_GEOMETRY_REACH, 0),
			sf::Vector2f(width + REAL_GEOMETRY_REACH, height)
		};

		const sf::Vector2f vertices5[] = {
			sf::Vector2f(REAL_GEOMETRY_REACH, 0),
			sf::Vector2f(width + REAL_GEOMETRY_REACH, height),
			sf::Vector2f(REAL_GEOMETRY_REACH, height)
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

		AddTriangle(vertices4, textureCoords0);
		AddTriangle(vertices5, textureCoords1);
	}

	bool UseVertexArrayTexture() override
	{
		return true;
	}

	void UpdateData() override
	{
		m_field.SetText(m_data);
		UpdateVertexArray();
	}
private:
	Field m_field;
	std::function<void()> m_tab;
	sf::RenderTexture m_textRendered;
};
