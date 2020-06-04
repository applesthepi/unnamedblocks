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

#define BOOL_GEOMETRY_REACH ((Global::BlockHeight - Global::BlockBorder) / 2.0f)

class ArgumentBool : public Argument
{
public:
	ArgumentBool(bool canSwitch)
		:Argument(), m_field("", canSwitch, FieldType::TEXT)
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
		return BlockArgumentType::BOOL;
	}

	uint32_t GetWidth() override
	{
		if (m_mode == BlockArgumentVariableMode::RAW)
			return Global::BlockHeight - Global::BlockBorder;
		else
			return m_field.GetWidth() + (BOOL_GEOMETRY_REACH * 2);
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

	sf::Vector2f LCir(float angle)
	{
		float halfHeight = (Global::BlockHeight - Global::BlockBorder) / 2.0f;

		float x = halfHeight * std::cosf(angle * (3.14159f / 180.0f));
		float y = halfHeight * std::sinf(angle * (3.14159f / 180.0f));

		return sf::Vector2f(x, y);
	}

	sf::Vector2f RCir(float angle)
	{
		float halfHeight = (Global::BlockHeight - Global::BlockBorder) / 2.0f;

		float x;
		float y = halfHeight * std::sinf(angle * (3.14159f / 180.0f));
		
		if (m_mode == BlockArgumentVariableMode::RAW)
			x = (halfHeight + (Global::BlockHeight - Global::BlockBorder)) * std::cosf(angle * (3.14159f / 180.0f));
		else
			x = (halfHeight + m_field.GetWidth()) * std::cosf(angle * (3.14159f / 180.0f));

		return sf::Vector2f(x, y);
	}

	void UpdateVertexArray() override
	{
		uint16_t height = Global::BlockHeight - Global::BlockBorder;
		const uint32_t width = m_field.GetWidth();

		sf::Color col;
		const std::string& text = m_field.GetText();

		if (m_mode == BlockArgumentVariableMode::RAW)
		{
			if (m_data == "1")
				col = sf::Color(0, 200, 0);
			else
				col = sf::Color(200, 0, 0);
		}
		else if (m_mode == BlockArgumentVariableMode::VAR)
			col = MOD_BUTTON_TEXT_BG_ACCENT_STRONG;

		// ===================================================================================================
		// =============== Prepare Vertex Buffer Texture
		// ===================================================================================================

		if (m_mode == BlockArgumentVariableMode::VAR)
		{
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
		}

		// ===================================================================================================
		// =============== Update Vertex Array; see "dev/bool_geometry.png"
		// ===================================================================================================

		ResetVertices(42);

		const float Langles[] = {
			0.0f, 30.0f, 60.0f, 90.0f,
			120.0f, 150.0f, 180.0f
		};

		const float Rangles[] = {
			0.0f, -30.0f, -60.0f, -90.0f,
			-120.0f, -150.0f, -180.0f
		};

		float halfHeight = (Global::BlockHeight - Global::BlockBorder) / 2.0f;

		const sf::Vector2f center(halfHeight, halfHeight);

		// ===================================================================================================
		// =============== Vanity Geometry Left
		// ===================================================================================================

		const sf::Vector2f vertices0[] = {
			LCir(Langles[1]),
			LCir(Langles[0]),
			center
		};

		const sf::Vector2f vertices1[] = {
			LCir(Langles[2]),
			LCir(Langles[1]),
			center
		};

		const sf::Vector2f vertices2[] = {
			LCir(Langles[3]),
			LCir(Langles[2]),
			center
		};

		const sf::Vector2f vertices3[] = {
			LCir(Langles[4]),
			LCir(Langles[3]),
			center
		};

		const sf::Vector2f vertices4[] = {
			LCir(Langles[5]),
			LCir(Langles[4]),
			center
		};

		const sf::Vector2f vertices5[] = {
			LCir(Langles[6]),
			LCir(Langles[5]),
			center
		};

		// ===================================================================================================
		// =============== Vanity Geometry Right
		// ===================================================================================================

		const sf::Vector2f vertices6[] = {
			RCir(Rangles[1]),
			RCir(Rangles[0]),
			center
		};

		const sf::Vector2f vertices7[] = {
			RCir(Rangles[2]),
			RCir(Rangles[1]),
			center
		};

		const sf::Vector2f vertices8[] = {
			RCir(Rangles[3]),
			RCir(Rangles[2]),
			center
		};

		const sf::Vector2f vertices9[] = {
			RCir(Rangles[4]),
			RCir(Rangles[3]),
			center
		};

		const sf::Vector2f vertices10[] = {
			RCir(Rangles[5]),
			RCir(Rangles[4]),
			center
		};

		const sf::Vector2f vertices11[] = {
			RCir(Rangles[6]),
			RCir(Rangles[5]),
			center
		};

		// ===================================================================================================
		// =============== Core Geometry
		// ===================================================================================================

		const sf::Vector2f vertices12[] = {
			sf::Vector2f(halfHeight, 0),
			sf::Vector2f(width + halfHeight, 0),
			sf::Vector2f(width + halfHeight, height)
		};

		const sf::Vector2f vertices13[] = {
			sf::Vector2f(halfHeight, 0),
			sf::Vector2f(width + halfHeight, height),
			sf::Vector2f(halfHeight, height)
		};

		const sf::Vector2f textureCoords0[] = {
			sf::Vector2f(halfHeight, 0),
			sf::Vector2f(width + halfHeight, 0),
			sf::Vector2f(width + halfHeight, height)
		};

		const sf::Vector2f textureCoords1[] = {
			sf::Vector2f(halfHeight, 0),
			sf::Vector2f(width + halfHeight, height),
			sf::Vector2f(halfHeight, height)
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
		AddTriangle(vertices4, colors);
		AddTriangle(vertices5, colors);

		AddTriangle(vertices6, colors);
		AddTriangle(vertices7, colors);
		AddTriangle(vertices8, colors);
		AddTriangle(vertices9, colors);
		AddTriangle(vertices10, colors);
		AddTriangle(vertices11, colors);

		if (m_mode == BlockArgumentVariableMode::RAW)
		{
			AddTriangle(vertices12, colors);
			AddTriangle(vertices13, colors);
		}
		else
		{
			AddTriangle(vertices12, textureCoords0);
			AddTriangle(vertices13, textureCoords1);
		}
	}

	bool UseVertexArrayTexture() override
	{
		return m_mode == BlockArgumentVariableMode::VAR;
	}

	void UpdateData() override
	{
		if (m_mode == BlockArgumentVariableMode::VAR)
			m_field.SetText(m_data);

		UpdateVertexArray();
	}
private:
	Field m_field;
	std::function<void()> m_tab;
	sf::RenderTexture m_textRendered;
};
