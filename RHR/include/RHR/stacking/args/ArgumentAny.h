#pragma once
#include "Argument.h"

#include <Espresso/Global.h>
#include <Espresso/InputHandler.h>
#include <SFML/Window/Clipboard.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <Cappuccino/Logger.h>

class ArgumentAny : public Argument
{
public:
	ArgumentAny(const sf::Vector2u& relitivePosition)
		:Argument(relitivePosition)
	{
		m_isDown = false;
		m_variableMode = BlockArgumentVariableMode::VAR;
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
	}

	void FrameUpdate() override
	{
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
	}

	void Render(sf::RenderTexture* render) override
	{
		render->draw(m_inputBackground);

		if (m_selected)
		{
			render->draw(m_input);
			render->draw(m_inputLocHigh);
			render->draw(m_inputLoc);
		}
		else
			render->draw(m_input);
	}

	unsigned int GetArgumentRawWidth() override
	{
		return m_inputBackground.getSize().x;
	}

	const bool MouseButton(bool down, const sf::Vector2i& position, const sf::Mouse::Button& button) override
	{
		if (down && m_fullSelect)
		{
			m_textLocHigh = 0;
			m_textLoc = m_text.length();
			m_isDown = false;

			return true;
		}
		else if (!down && m_fullSelect)
		{
			m_fullSelect = false;
			m_textLocHigh = 0;
			m_textLoc = m_text.length();
			m_isDown = false;

			return true;
		}

		if (button == sf::Mouse::Left)
		{
			TextSystem tSys(&m_text, &m_textLocHigh, &m_textLoc, &m_isDown);

			if (InputHandler::RunMouseProccess(tSys, sf::Vector2i(m_realAbsolutePosition.x, m_realAbsolutePosition.y + (Global::BlockBorder / 2)), sf::Vector2u(m_inputBackground.getSize().x, m_inputBackground.getSize().y), down, position, Global::BlockHeight - Global::BlockBorder, 0))
			{
				if (down)
					Select();

				return true;
			}
			else
			{
				if (Global::SelectedArgument == this)
				{
					m_selected = false;
					Global::SelectedStack = nullptr;
					Global::SelectedBlock = nullptr;
					Global::SelectedArgument = nullptr;
					Global::Dragging = false;

					InputHandler::UnregisterKeyCallback(&m_textCallback);

					(*m_functionUpdatePreTexture)();
				}

				return false;
			}
		}

		return false;
	}

	const bool HasData() override
	{
		return true;
	}

	void SetData(const std::string& data) override
	{
		m_text = data;
		m_input.setString(m_text);
	}

	void SetMode(const BlockArgumentVariableMode& mode) override
	{
		m_variableMode = mode;
	}

	const std::string& GetData() override
	{
		return (m_variableMode == BlockArgumentVariableMode::VAR ? '1' : '0') + m_text;
	}

	const std::string& GetDataRaw() override
	{
		return m_text;
	}

	const BlockArgumentVariableMode GetMode() override
	{
		return BlockArgumentVariableMode::VAR;
	}

	void Deallocate() override
	{
		if (m_selected)
			InputHandler::UnregisterKeyCallback(&m_textCallback);
	}

	void Select() override
	{
		SelectGlobaly();

		if (m_selected)
			InputHandler::RunMouseProccess(&m_input, &m_textLocHigh, &m_textLoc, &m_isDown, true, Global::MousePosition, Global::BlockHeight - Global::BlockBorder);
		else
		{
			InputHandler::RegisterKeyCallback(&m_textCallback);

			m_selected = true;
			m_textLocHigh = 0;
			m_textLoc = 0;
			m_fullSelect = true;
		}
	}

	void ReInspectData() override
	{
		m_inputBackground.setSize(sf::Vector2f(m_input.getLocalBounds().width + (float)(Global::BlockBorder * 2), Global::BlockHeight - Global::BlockBorder));
	}

	const BlockArgumentType GetType() override
	{
		return BlockArgumentType::ANY;
	}
private:
	std::string m_text;
	sf::Text m_input;
	sf::RectangleShape m_inputBackground;
	sf::RectangleShape m_inputLocHigh;
	sf::RectangleShape m_inputLoc;
	uint64_t m_textLoc;
	uint64_t m_textLocHigh;

	BlockArgumentVariableMode m_variableMode;

	bool m_isDown;
	bool m_selected;
	bool m_fullSelect;

	std::function<void(const sf::Event::KeyEvent&)> m_textCallback;
};
