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
			return (Global::BlockHeight - Global::BlockBorder) * 2;
		else
			return m_field.GetWidth() + (Global::BlockHeight - Global::BlockBorder);
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

		float x = halfHeight * sinf(angle * (3.14159f / 180.0f)) * -1 + halfHeight;
		float y = halfHeight * cosf(angle * (3.14159f / 180.0f)) * -1 + halfHeight;

		return sf::Vector2f(x, y);
	}

	sf::Vector2f RCir(float angle)
	{
		uint16_t height = Global::BlockHeight - Global::BlockBorder;
		uint32_t width = m_field.GetWidth();

		float halfHeight = static_cast<float>(height) / 2.0f;
		float centerFar;

		if (m_mode == BlockArgumentVariableMode::RAW)
			centerFar = static_cast<float>(height) + halfHeight;
		else
			centerFar = halfHeight + static_cast<float>(width);

		float x = halfHeight * sinf(angle * (3.14159f / 180.0f)) + centerFar;
		float y = halfHeight * cosf(angle * (3.14159f / 180.0f)) * -1 + halfHeight;

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
			//sf::Text textRecreation = sf::Text(text, Global::Font, height);
			//textRecreation.setFillColor(MOD_BUTTON_TEXT_FG);
			//textRecreation.setPosition(4, -1);

			uint32_t textWidth = width;
			uint32_t textHeight = Global::BlockHeight;

			if (textWidth == 0 || textHeight == 0)
			{
				textWidth = 1;
				textHeight = Global::BlockHeight - Global::BlockBorder;
			}

			//m_textRendered.create(textWidth, textHeight);
			//m_textRendered.clear(col);
			//m_textRendered.draw(textRecreation);
			//m_vertexArrayImage = m_textRendered.getTexture().copyToImage();
		}

		// ===================================================================================================
		// =============== Update Vertex Array; see "dev/bool_geometry.png"
		// ===================================================================================================

		//ResetVertices(42);

		const float angles[] = {
			0.0f, 30.0f, 60.0f, 90.0f,
			120.0f, 150.0f, 180.0f
		};

		float halfHeight = (Global::BlockHeight - Global::BlockBorder) / 2.0f;

		const sf::Vector2f center(halfHeight, halfHeight);
		sf::Vector2f centerFar;

		if (m_mode == BlockArgumentVariableMode::RAW)
			centerFar = sf::Vector2f(height + halfHeight, halfHeight);
		else
			centerFar = sf::Vector2f(halfHeight + width, halfHeight);

		// ===================================================================================================
		// =============== Vanity Geometry Left
		// ===================================================================================================

		const sf::Vector2f vertices0[] = {
			LCir(angles[1]),
			LCir(angles[0]),
			center
		};

		const sf::Vector2f vertices1[] = {
			LCir(angles[2]),
			LCir(angles[1]),
			center
		};

		const sf::Vector2f vertices2[] = {
			LCir(angles[3]),
			LCir(angles[2]),
			center
		};

		const sf::Vector2f vertices3[] = {
			LCir(angles[4]),
			LCir(angles[3]),
			center
		};

		const sf::Vector2f vertices4[] = {
			LCir(angles[5]),
			LCir(angles[4]),
			center
		};

		const sf::Vector2f vertices5[] = {
			LCir(angles[6]),
			LCir(angles[5]),
			center
		};

		// ===================================================================================================
		// =============== Vanity Geometry Right
		// ===================================================================================================
		
		const sf::Vector2f vertices6[] = {
			RCir(angles[1]),
			RCir(angles[0]),
			centerFar
		};

		const sf::Vector2f vertices7[] = {
			RCir(angles[2]),
			RCir(angles[1]),
			centerFar
		};

		const sf::Vector2f vertices8[] = {
			RCir(angles[3]),
			RCir(angles[2]),
			centerFar
		};

		const sf::Vector2f vertices9[] = {
			RCir(angles[4]),
			RCir(angles[3]),
			centerFar
		};

		const sf::Vector2f vertices10[] = {
			RCir(angles[5]),
			RCir(angles[4]),
			centerFar
		};

		const sf::Vector2f vertices11[] = {
			RCir(angles[6]),
			RCir(angles[5]),
			centerFar
		};
		
		// ===================================================================================================
		// =============== Core Geometry
		// ===================================================================================================

		sf::Vector2f verticesCore0[3];
		sf::Vector2f verticesCore1[3];

		//sf::Vector2f textureCore0[3];
		//sf::Vector2f textureCore1[3];

		if (m_mode == BlockArgumentVariableMode::RAW)
		{
			verticesCore0[0] = sf::Vector2f(halfHeight, 0);
			verticesCore0[1] = sf::Vector2f(halfHeight + height, 0);
			verticesCore0[2] = sf::Vector2f(halfHeight + height, height);

			verticesCore1[0] = sf::Vector2f(halfHeight, 0);
			verticesCore1[1] = sf::Vector2f(halfHeight + height, height);
			verticesCore1[2] = sf::Vector2f(halfHeight, height);

			//textureCore0[0] = sf::Vector2f(0, 0);
			//textureCore0[1] = sf::Vector2f(height, 0);
			//textureCore0[2] = sf::Vector2f(height, height);
			//
			//textureCore1[0] = sf::Vector2f(0, 0);
			//textureCore1[1] = sf::Vector2f(height, height);
			//textureCore1[2] = sf::Vector2f(0, height);
		}
		else
		{
			verticesCore0[0] = sf::Vector2f(halfHeight, 0);
			verticesCore0[1] = sf::Vector2f(halfHeight + width, 0);
			verticesCore0[2] = sf::Vector2f(halfHeight + width, height);

			verticesCore1[0] = sf::Vector2f(halfHeight, 0);
			verticesCore1[1] = sf::Vector2f(halfHeight + width, height);
			verticesCore1[2] = sf::Vector2f(halfHeight, height);

			//textureCore0[0] = sf::Vector2f(0, 0);
			//textureCore0[1] = sf::Vector2f(width, 0);
			//textureCore0[2] = sf::Vector2f(width, height);
			//
			//textureCore1[0] = sf::Vector2f(0, 0);
			//textureCore1[1] = sf::Vector2f(width, height);
			//textureCore1[2] = sf::Vector2f(0, height);
		}

		const sf::Color colors[] = {
			col,
			col,
			col
		};

		GetVAO().AddTriangle(vertices0, colors);
		GetVAO().AddTriangle(vertices1, colors);
		GetVAO().AddTriangle(vertices2, colors);
		GetVAO().AddTriangle(vertices3, colors);
		GetVAO().AddTriangle(vertices4, colors);
		GetVAO().AddTriangle(vertices5, colors);

		GetVAO().AddTriangle(vertices6, colors);
		GetVAO().AddTriangle(vertices7, colors);
		GetVAO().AddTriangle(vertices8, colors);
		GetVAO().AddTriangle(vertices9, colors);
		GetVAO().AddTriangle(vertices10, colors);
		GetVAO().AddTriangle(vertices11, colors);

		if (m_mode == BlockArgumentVariableMode::RAW)
		{
			GetVAO().AddTriangle(verticesCore0, colors);
			GetVAO().AddTriangle(verticesCore1, colors);
		}
		else
		{
			//AddTriangle(verticesCore0, textureCore0);
			//AddTriangle(verticesCore1, textureCore1);

			//for (uint32_t i = 0; i < text.length(); i++)
			//	GetVAO().AddChar(sf::FloatRect(i * Global::BlockHeight, 0, Global::BlockHeight, Global::BlockHeight), text[i]);
		}
	}

	//bool UseVertexArrayTexture() override
	//{
	//	return m_mode == BlockArgumentVariableMode::VAR;
	//}

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
