#pragma once
#include "config.h"
/*
#include "stacking/args/Argument.hpp"
#include "registries/UIRegistry.hpp"

#include <Cappuccino/Utils.hpp>
#include <Espresso/InputHandler.h>

class ArgumentAny : public Argument
{
public:
	ArgumentAny()
		:Argument(), m_field("", false, FieldType::TEXT)
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
		return BlockArgumentType::STRING;
	}

	u32 GetWidth() override
	{
		return m_field.GetWidth();
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

		u16 height = Global::BlockHeight - Global::BlockBorder;
		const u32 width = m_field.GetWidth();

		sf::Color col(MOD_BUTTON_TEXT_BG_ACCENT_STRONG);
		const std::string& text = m_field.GetText();

		// ===================================================================================================
		// =============== Prepare rhr::render::vertex Buffer Texture
		// ===================================================================================================

		//sf::Text textRecreation = sf::Text(text, Global::Font, height);
		//textRecreation.setFillColor(MOD_BUTTON_TEXT_FG);
		//textRecreation.setPosition(4, -1);

		u32 textWidth = width;
		u32 textHeight = Global::BlockHeight;

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
		// =============== Update rhr::render::vertex Array; see "dev/string_geometry.png"
		// ===================================================================================================

		//ResetVertices(6);

		// ===================================================================================================
		// =============== Core Geometry
		// ===================================================================================================

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

		//const sf::Vector2f textureCoords0[] = {
		//	sf::Vector2f(0, 0),
		//	sf::Vector2f(width, 0),
		//	sf::Vector2f(width, height)
		//};
		//
		//const sf::Vector2f textureCoords1[] = {
		//	sf::Vector2f(0, 0),
		//	sf::Vector2f(width, height),
		//	sf::Vector2f(0, height)
		//};

		//AddTriangle(vertices0, textureCoords0);
		//AddTriangle(vertices1, textureCoords1);

		//for (u32 i = 0; i < text.length(); i++)
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
*/