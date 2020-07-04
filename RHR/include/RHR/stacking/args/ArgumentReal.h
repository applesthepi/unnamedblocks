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
		uint32_t textHeight = (Global::BlockHeight - Global::BlockBorder) * 2;
		uint32_t textWidth = m_field.GetWidth() * 2;

		sf::Color col;
		const std::string& text = m_field.GetText();

		if (m_mode == BlockArgumentVariableMode::RAW)
			col = MOD_BUTTON_TEXT_BG;
		else if (m_mode == BlockArgumentVariableMode::VAR)
			col = MOD_BUTTON_TEXT_BG_ACCENT_STRONG;

		// ===================================================================================================
		// =============== Prepare Vertex Buffer Texture
		// ===================================================================================================

		//sf::Text textRecreation = sf::Text(text, Global::Font, (Global::BlockHeight - Global::BlockBorder) * 2);
		//textRecreation.setFillColor(MOD_BUTTON_TEXT_FG);
		//textRecreation.setPosition(Global::BlockBorder * 2, -6.0f);
		
		if (textWidth == 0 || textHeight == 0)
		{
			textWidth = 1;
			textHeight = Global::BlockHeight - Global::BlockBorder;
		}

		//m_textRendered.create(textWidth, textHeight);
		//m_textRendered.clear(col);
		//m_textRendered.draw(textRecreation);
		//m_vertexArrayImage = m_textRendered.getTexture().copyToImage();

		// ===================================================================================================
		// =============== Update Vertex Array; see "dev/real_geometry.png"
		// ===================================================================================================

		//ResetVertices(18);
		
		// ===================================================================================================
		// =============== Vanity Geometry
		// ===================================================================================================

		const sf::Vector2f vertices0[] = {
			sf::Vector2f(0, 0),
			sf::Vector2f(REAL_GEOMETRY_REACH, 0),
			sf::Vector2f(REAL_GEOMETRY_REACH, textHeight / 4.0f),
		};

		const sf::Vector2f vertices1[] = {
			sf::Vector2f(0, textHeight / 2.0f),
			sf::Vector2f(REAL_GEOMETRY_REACH, textHeight / 4.0f),
			sf::Vector2f(REAL_GEOMETRY_REACH, textHeight / 2.0f),
		};

		const sf::Vector2f vertices2[] = {
			sf::Vector2f(textWidth / 2.0f + REAL_GEOMETRY_REACH + REAL_GEOMETRY_REACH, 0),
			sf::Vector2f(textWidth / 2.0f + REAL_GEOMETRY_REACH, textHeight / 4.0f),
			sf::Vector2f(textWidth / 2.0f + REAL_GEOMETRY_REACH, 0),
		};

		const sf::Vector2f vertices3[] = {
			sf::Vector2f(textWidth / 2.0f + REAL_GEOMETRY_REACH + REAL_GEOMETRY_REACH, textHeight / 2.0f),
			sf::Vector2f(textWidth / 2.0f + REAL_GEOMETRY_REACH, textHeight / 2.0f),
			sf::Vector2f(textWidth / 2.0f + REAL_GEOMETRY_REACH, textHeight / 4.0f)
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

		//const sf::Vector2f vertices4[] = {
		//	sf::Vector2f(REAL_GEOMETRY_REACH, 0),
		//	sf::Vector2f(textWidth / 2 + REAL_GEOMETRY_REACH, 0),
		//	sf::Vector2f(textWidth / 2 + REAL_GEOMETRY_REACH, textHeight / 2)
		//};
		//
		//const sf::Vector2f vertices5[] = {
		//	sf::Vector2f(REAL_GEOMETRY_REACH, 0),
		//	sf::Vector2f(textWidth / 2 + REAL_GEOMETRY_REACH, textHeight / 2),
		//	sf::Vector2f(REAL_GEOMETRY_REACH, textHeight / 2)
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

		//AddTriangle(vertices4, textureCoords0);
		//AddTriangle(vertices5, textureCoords1);

		//for (uint32_t i = 0; i < text.length(); i++)
		//	GetVAO().AddChar(sf::FloatRect(i * Global::BlockHeight, 0, Global::BlockHeight, Global::BlockHeight), text[i]);
	}

	//bool UseVertexArrayTexture() override
	//{
	//	return true;
	//}

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
