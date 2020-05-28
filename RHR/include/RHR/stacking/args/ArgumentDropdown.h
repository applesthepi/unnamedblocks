#pragma once
#include "Argument.h"
#include "RHR/ui/Dropdown.h"

#include <Espresso/Global.h>
#include <Espresso/InputHandler.h>
#include <SFML/Window/Clipboard.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <Cappuccino/Logger.h>

class ArgumentDropdown : public Argument
{
public:
	ArgumentDropdown(const sf::Vector2u& relitivePosition)
		:Argument(relitivePosition)
	{
		/*
		m_isDown = false;
		m_selected = false;
		m_fullSelect = false;

		m_textLoc = 0;
		m_textLocHigh = 0;

		m_input = sf::Text(m_text, *Global::Font, Global::BlockHeight - Global::BlockBorder);
		m_input.setFillColor(MOD_BUTTON_TEXT_FG);
		m_input.setPosition(GetAbsolutePosition().x + (int)Global::BlockBorder, GetAbsolutePosition().y);

		m_inputBackground = sf::RectangleShape(sf::Vector2f(m_input.getLocalBounds().width + (float)(Global::BlockBorder * 2), Global::BlockHeight - Global::BlockBorder));
		m_inputBackground.setFillColor(MOD_BUTTON_TEXT_BG);
		m_inputBackground.setPosition(GetAbsolutePosition().x, GetAbsolutePosition().y + (int)(Global::BlockBorder / 2));

		m_inputLoc = sf::RectangleShape(sf::Vector2f(1, Global::BlockHeight - Global::BlockBorder));
		m_inputLoc.setFillColor(MOD_BUTTON_TEXT_FG);

		m_inputLocHigh = sf::RectangleShape(sf::Vector2f(0, Global::BlockHeight));
		m_inputLocHigh.setFillColor(MOD_HIGHLIGHT_COLOR);

		m_textCallback = [&](const sf::Event::KeyEvent& ev)
		{
			if (ev.code == sf::Keyboard::Key::Tab)
				Next = true;
			else
				InputHandler::RunTextProccess(&m_text, &m_textLocHigh, &m_textLoc, nullptr, nullptr, ev);
		};
		*/
	}

	void ProcessData(const std::string& data)
	{
		m_elementText = data;

		std::vector<std::string> elements;
		std::string element;

		element.reserve(16);
		elements.reserve(4);

		for (uint64_t i = 0; i < data.length(); i++)
		{
			if (data[i] == ';')
				elements.push_back(element);
			else
				element += data[i];
		}

		m_dropdown.SetElements(elements, 0);
	}

	void FrameUpdate() override
	{
		/*
		if (m_selected && !m_fullSelect)
			InputHandler::RunMouseProccessFrame(&m_text, sf::Vector2i(m_realAbsolutePosition.x, m_realAbsolutePosition.y + (Global::BlockBorder / 2)), (sf::Vector2u)m_inputBackground.getSize(), &m_textLoc, &m_isDown, Global::MousePosition, Global::BlockHeight - Global::BlockBorder, 0);

		m_input.setString(m_text);
		m_input.setPosition(GetRelitivePosition().x + (float)Global::BlockBorder, GetRelitivePosition().y);

		m_inputBackground.setSize(sf::Vector2f(m_input.getLocalBounds().width + (float)(Global::BlockBorder * 2), Global::BlockHeight - Global::BlockBorder));
		m_inputBackground.setPosition(GetRelitivePosition().x, GetRelitivePosition().y + (int)(Global::BlockBorder / 2));

		if (m_variableMode == BlockArgumentVariableMode::VAR)
		{
			m_inputBackground.setFillColor(MOD_BUTTON_TEXT_BG_ACCENT_STRONG);
			m_input.setFillColor(MOD_BUTTON_TEXT_FG);
		}
		else
		{
			m_inputBackground.setFillColor(MOD_BUTTON_TEXT_BG);
			m_input.setFillColor(MOD_BUTTON_TEXT_FG);
		}

		if (m_selected && (Global::SelectedArgument != this || Global::Dragging))
		{
			InputHandler::UnregisterKeyCallback(&m_textCallback);
			m_selected = false;
		}

		if (m_selected)
		{
			m_inputLoc.setPosition(GetRelitivePosition().x + sf::Text(m_text.substr(0, m_textLoc), *Global::Font, Global::BlockHeight - Global::BlockBorder).getLocalBounds().width + (int)Global::BlockBorder, GetRelitivePosition().y + (Global::BlockBorder / 2));
			m_inputLocHigh.setPosition(GetRelitivePosition().x + sf::Text(m_text.substr(0, std::min(m_textLocHigh, m_textLoc)), *Global::Font, Global::BlockHeight - Global::BlockBorder).getLocalBounds().width + (int)Global::BlockBorder, GetRelitivePosition().y + (Global::BlockBorder / 2));
			m_inputLocHigh.setSize(sf::Vector2f(sf::Text(m_text.substr(std::min(m_textLocHigh, m_textLoc), std::max(m_textLocHigh, m_textLoc) - std::min(m_textLocHigh, m_textLoc)), *Global::Font, Global::BlockHeight - Global::BlockBorder).getLocalBounds().width, Global::BlockHeight - Global::BlockBorder));
		}
		*/
	}

	void Render(sf::RenderTexture* render) override
	{
		m_dropdown.Render(*render)
			sf::RenderWindow.
	}

	unsigned int GetArgumentRawWidth() override
	{
		return m_dropdown.GetWidth();
	}

	const bool MouseButton(const bool& down, const sf::Vector2i& position, const sf::Mouse::Button& button) override
	{
		if (!m_dropdown.MouseButton(down, position, button))
			return false;
		else
		{
			m_data = "0" + std::to_string(m_dropdown.GetSelected());
			m_rawData = std::to_string(m_dropdown.GetSelected());
		}
	}

	const bool HasData() override
	{
		return true;
	}

	void SetData(const std::string& data) override
	{
		ProcessData(data);
	}

	const std::string& GetData() override
	{
		return m_data;
	}

	const std::string& GetDataRaw() override
	{
		return m_rawData;
	}

	const BlockArgumentVariableMode GetMode() override
	{
		return BlockArgumentVariableMode::RAW;
	}

	void Select() override
	{
		SelectGlobaly();
		m_dropdown.SetEnabled(true);
	}

	void ReInspectData() override
	{
		ProcessData(m_elementText);
	}

	const BlockArgumentType GetType() override
	{
		return BlockArgumentType::STRING;
	}
private:
	//bool m_isDown;
	//bool m_selected;
	//bool m_fullSelect;

	bool m_isOpen;
	Dropdown m_dropdown;
	std::string m_data;
	std::string m_rawData;
	std::string m_elementText;

	//std::function<void(const sf::Event::KeyEvent&)> m_textCallback;
};
