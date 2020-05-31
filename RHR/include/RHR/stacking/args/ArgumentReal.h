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
	ArgumentReal(const BlockArgumentVariableMode& mode, const bool& canSwitch)
		:Argument(), m_field("", canSwitch, FieldType::NUMBER)
	{
		UpdateVertexArray();

		m_tab = [this]()
		{
			m_next = true;
		};

		m_field.SetTab(&m_tab);
	}

	const BlockArgumentType GetType() override
	{
		return BlockArgumentType::REAL;
	}

	const uint32_t GetWidth() override
	{
		return m_field.GetWidth();
	}

	const bool HasData() override
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
		const uint32_t width = GetWidth();
		const std::string& text = m_field.GetText();

		if (m_mode == BlockArgumentVariableMode::RAW)
			col = MOD_BUTTON_TEXT_BG;
		else if (m_mode == BlockArgumentVariableMode::VAR)
			col = MOD_BUTTON_TEXT_BG_ACCENT_STRONG;

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
		
		sf::Text textRecreation = sf::Text(text, Global::Font, height);

		m_textRendered.create(textRecreation.getLocalBounds().width, textRecreation.getLocalBounds().height);
		m_textRendered.draw(textRecreation);
		m_vertexArrayImage = m_textRendered.getTexture().copyToImage();

		// ===================================================================================================
		// =============== Update Vertex Array; see "dev/real_geometry.png"
		// ===================================================================================================

		m_vertexArray.clear();

		// 0

		m_vertexArray.append(sf::Vertex(sf::Vector2f(0, 0), col));
		m_vertexArray.append(sf::Vertex(sf::Vector2f(REAL_GEOMETRY_REACH, 0), col));
		m_vertexArray.append(sf::Vertex(sf::Vector2f(REAL_GEOMETRY_REACH, height / 2), col));

		// 1

		m_vertexArray.append(sf::Vertex(sf::Vector2f(0, height), col));
		m_vertexArray.append(sf::Vertex(sf::Vector2f(REAL_GEOMETRY_REACH, height / 2), col));
		m_vertexArray.append(sf::Vertex(sf::Vector2f(REAL_GEOMETRY_REACH, height), col));

		// 2

		m_vertexArray.append(sf::Vertex(sf::Vector2f(width + REAL_GEOMETRY_REACH + REAL_GEOMETRY_REACH, 0), col));
		m_vertexArray.append(sf::Vertex(sf::Vector2f(width + REAL_GEOMETRY_REACH, height / 2), col));
		m_vertexArray.append(sf::Vertex(sf::Vector2f(width + REAL_GEOMETRY_REACH, 0), col));

		// 3

		m_vertexArray.append(sf::Vertex(sf::Vector2f(width + REAL_GEOMETRY_REACH + REAL_GEOMETRY_REACH, height), col));
		m_vertexArray.append(sf::Vertex(sf::Vector2f(width + REAL_GEOMETRY_REACH, height), col));
		m_vertexArray.append(sf::Vertex(sf::Vector2f(width + REAL_GEOMETRY_REACH, height / 2), col));

		// 4

		m_vertexArray.append(sf::Vertex(sf::Vector2f(REAL_GEOMETRY_REACH, 0), sf::Vector2f(0, 0)));
		m_vertexArray.append(sf::Vertex(sf::Vector2f(width + REAL_GEOMETRY_REACH, 0), sf::Vector2f(m_vertexArrayImage.getSize().x, 0)));
		m_vertexArray.append(sf::Vertex(sf::Vector2f(width + REAL_GEOMETRY_REACH, height), sf::Vector2f(m_vertexArrayImage.getSize().x, m_vertexArrayImage.getSize().y)));

		// 5

		m_vertexArray.append(sf::Vertex(sf::Vector2f(REAL_GEOMETRY_REACH, 0), sf::Vector2f(0, 0)));
		m_vertexArray.append(sf::Vertex(sf::Vector2f(width + REAL_GEOMETRY_REACH, height), sf::Vector2f(m_vertexArrayImage.getSize().x, m_vertexArrayImage.getSize().y)));
		m_vertexArray.append(sf::Vertex(sf::Vector2f(REAL_GEOMETRY_REACH, height), sf::Vector2f(0, m_vertexArrayImage.getSize().y)));
	}
private:
	Field m_field;
	std::function<void()> m_tab;
	sf::RenderTexture m_textRendered;
};
