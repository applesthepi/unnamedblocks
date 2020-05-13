#pragma once

#include "Argument.h"
#include "RHR/Global.h"
#include "RHR/handlers/InputHandler.h"

#include <SFML/Window/Clipboard.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <Cappuccino/Logger.h>

class ArgumentBoolean : public Argument
{
public:
	ArgumentBoolean(sf::Vector2u relitivePosition)
		:Argument(relitivePosition)
	{
		m_isDown = false;
		m_variableMode = false;
		m_selected = false;
		m_fullSelect = false;

		m_textLoc = 0;
		m_textLocHigh = 0;
		m_text = "0";

		m_input = sf::Text(m_text, *Global::Font, Global::BlockHeight - Global::BlockBorder);
		m_input.setFillColor(MOD_BUTTON_TEXT_FG);
		m_input.setPosition(GetAbsolutePosition().x + (int)Global::BlockBorder, GetAbsolutePosition().y);

		m_inputBackground = sf::RectangleShape(sf::Vector2f(m_input.getLocalBounds().width + (float)(Global::BlockBorder * 2), Global::BlockHeight - Global::BlockBorder));
		m_inputBackground.setFillColor(MOD_BUTTON_TEXT_BG);
		m_inputBackground.setPosition(GetAbsolutePosition().x, GetAbsolutePosition().y + (int)(Global::BlockBorder / 2));
		//m_inputBackground.setOutlineColor(MOD_BUTTON_TEXT_FG);
		//m_inputBackground.setOutlineThickness(1.0f);

		m_inputLoc = sf::RectangleShape(sf::Vector2f(1, Global::BlockHeight - Global::BlockBorder));
		m_inputLoc.setFillColor(MOD_BUTTON_TEXT_FG);

		m_inputLocHigh = sf::RectangleShape(sf::Vector2f(0, Global::BlockHeight));
		m_inputLocHigh.setFillColor(MOD_HIGHLIGHT_COLOR);

		m_textCallback = [&](const sf::Event::KeyEvent& ev)
		{
			if (ev.code == sf::Keyboard::Key::Tab)
				Next = true;
			else
			{
				if (m_variableMode)
					InputHandler::RunTextProccess(&m_text, &m_textLocHigh, &m_textLoc, nullptr, nullptr, ev);
				else
					InputHandler::RunNumberProccess(&m_text, &m_textLocHigh, &m_textLoc, nullptr, nullptr, ev);
			}
		};

		m_left = new sf::CircleShape((Global::BlockHeight - Global::BlockBorder) / 2, 10);
		m_right = new sf::CircleShape((Global::BlockHeight - Global::BlockBorder) / 2, 10);
	}

	void FrameUpdate() override
	{
		if (m_selected && !m_fullSelect)
			InputHandler::RunMouseProccessFrame(&m_text, sf::Vector2i(((Global::BlockHeight - Global::BlockBorder) / 2) + GetRealAbsolutePosition().x + (int)Global::BlockBorder, GetRealAbsolutePosition().y + (Global::BlockBorder / 2)), (sf::Vector2u)m_inputBackground.getSize(), &m_textLoc, &m_isDown, Global::MousePosition, Global::BlockHeight - Global::BlockBorder);

		if (m_variableMode)
		{
			m_input.setString(m_text);
			m_input.setPosition(GetRelitivePosition().x + (float)Global::BlockBorder + ((Global::BlockHeight - Global::BlockBorder) / 2), GetRelitivePosition().y);

			m_inputBackground.setSize(sf::Vector2f(m_input.getLocalBounds().width + (float)(Global::BlockBorder * 2), Global::BlockHeight - Global::BlockBorder));

			m_inputBackground.setFillColor(MOD_VAR);
			m_left->setFillColor(MOD_VAR);
			m_right->setFillColor(MOD_VAR);

			m_input.setFillColor(sf::Color::Black);

			if (m_selected)
			{
				m_inputLoc.setPosition(((Global::BlockHeight - Global::BlockBorder) / 2) + GetRelitivePosition().x + sf::Text(m_text.substr(0, m_textLoc), *Global::Font, Global::BlockHeight - Global::BlockBorder).getLocalBounds().width + (int)Global::BlockBorder, GetRelitivePosition().y + (Global::BlockBorder / 2));
				m_inputLocHigh.setPosition(((Global::BlockHeight - Global::BlockBorder) / 2) + GetRelitivePosition().x + sf::Text(m_text.substr(0, std::min(m_textLocHigh, m_textLoc)), *Global::Font, Global::BlockHeight - Global::BlockBorder).getLocalBounds().width + (int)Global::BlockBorder, GetRelitivePosition().y + (Global::BlockBorder / 2));
				m_inputLocHigh.setSize(sf::Vector2f(sf::Text(m_text.substr(std::min(m_textLocHigh, m_textLoc), std::max(m_textLocHigh, m_textLoc) - std::min(m_textLocHigh, m_textLoc)), *Global::Font, Global::BlockHeight - Global::BlockBorder).getLocalBounds().width, Global::BlockHeight - Global::BlockBorder));
			}
		}
		else
		{
			if (m_text == "1")
			{
				m_inputBackground.setFillColor(sf::Color::Green);
				m_left->setFillColor(sf::Color::Green);
				m_right->setFillColor(sf::Color::Green);
			}
			else if (m_text == "0")
			{
				m_inputBackground.setFillColor(sf::Color(250, 50, 40));
				m_left->setFillColor(sf::Color(250, 50, 40));
				m_right->setFillColor(sf::Color(250, 50, 40));
			}
			else
				Logger::Warn("invalid boolean value \"" + m_text + "\"");
		}

		m_inputBackground.setPosition(GetRelitivePosition().x + ((Global::BlockHeight - Global::BlockBorder) / 2), GetRelitivePosition().y + (int)(Global::BlockBorder / 2));

		if (m_selected && (Global::SelectedArgument != this || Global::Dragging))
		{
			InputHandler::UnregisterKeyCallback(&m_textCallback);
			m_selected = false;
		}

		m_left->setPosition(GetRelitivePosition().x, GetRelitivePosition().y + (int)(Global::BlockBorder / 2));
		m_right->setPosition(GetRelitivePosition().x + GetArgumentRawWidth() - (Global::BlockHeight - Global::BlockBorder), GetRelitivePosition().y + (int)(Global::BlockBorder / 2));
	}

	void Render(sf::RenderTexture* render) override
	{
		render->draw(*m_left);
		render->draw(*m_right);

		render->draw(m_inputBackground);

		if (m_variableMode)
		{
			if (m_selected)
			{
				render->draw(m_input);
				render->draw(m_inputLocHigh);
				render->draw(m_inputLoc);
			}
			else
				render->draw(m_input);
		}
	}

	unsigned int GetArgumentRawWidth() override
	{
		return m_inputBackground.getSize().x + (Global::BlockHeight - Global::BlockBorder);
	}

	bool MouseButton(bool down, sf::Vector2i position, sf::Mouse::Button button) override
	{
		if (!down && m_fullSelect)
		{
			m_fullSelect = false;
			m_textLocHigh = 0;
			m_textLoc = m_text.length();
			m_isDown = false;

			return true;
		}

		uint64_t superWidthBounds;

		if (m_variableMode)
			superWidthBounds = m_inputBackground.getSize().x;
		else
			superWidthBounds = (Global::BlockHeight - Global::BlockBorder) * 2;

		if (button == sf::Mouse::Middle && down &&
			position.x >= m_realAbsolutePosition.x && position.x <= m_realAbsolutePosition.x + superWidthBounds &&
			position.y >= m_realAbsolutePosition.y && position.y <= m_realAbsolutePosition.y + m_inputBackground.getSize().y)
		{
			SelectGlobaly();
			m_variableMode = !m_variableMode;

			if (m_variableMode)
				m_text = m_lastVar;
			else
			{
				m_lastVar = m_text;
				m_text = "0";

				m_inputBackground.setSize(sf::Vector2f(Global::BlockHeight - Global::BlockBorder, Global::BlockHeight - Global::BlockBorder));
			}

			if (m_selected)
			{
				m_textLocHigh = 0;
				m_textLoc = m_text.length();
			}
			else
			{
				InputHandler::RegisterKeyCallback(&m_textCallback);

				m_selected = true;
				m_textLocHigh = 0;
				m_textLoc = m_text.length();
			}
		}
		else if (button == sf::Mouse::Left)
		{
			uint64_t widthBounds;
			int64_t posBounds;

			if (m_variableMode)
				widthBounds = m_input.getLocalBounds().width;
			else
				widthBounds = (Global::BlockHeight - Global::BlockBorder) * 2;

			if (m_variableMode)
				posBounds = ((Global::BlockHeight - Global::BlockBorder) / 2);
			else
				posBounds = 0;

			if (InputHandler::RunMouseProccess(&m_text, sf::Vector2i(posBounds + GetRealAbsolutePosition().x + (int)Global::BlockBorder, GetRealAbsolutePosition().y + (Global::BlockBorder / 2)), sf::Vector2u(widthBounds, m_inputBackground.getSize().y), &m_textLocHigh, &m_textLoc, &m_isDown, down, position, Global::BlockHeight - Global::BlockBorder))
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

	bool HasData() override
	{
		return true;
	}

	void SetData(std::string data) override
	{
		m_text = data;
		m_input.setString(m_text);
	}

	void SetMode(BlockArgumentVariableMode mode) override
	{
		m_variableMode = mode == BlockArgumentVariableMode::VAR;
	}

	std::string* GetData() override
	{
		m_vText = (m_variableMode ? '1' : '0') + m_text;
		return &m_vText;
	}

	std::string* GetDataRaw() override
	{
		return &m_text;
	}

	BlockArgumentVariableMode* GetMode() override
	{
		if (m_variableMode)
			m_vMode = BlockArgumentVariableMode::VAR;
		else
			m_vMode = BlockArgumentVariableMode::RAW;

		return &m_vMode;
	}

	void Deallocate() override
	{
		if (m_selected)
			InputHandler::UnregisterKeyCallback(&m_textCallback);
	}

	void Select() override
	{
		if (!m_variableMode)
		{
			if (m_text == "1")
				m_text = "0";
			else if (m_text == "0")
				m_text = "1";
			else
				Logger::Warn("invalid boolean value \"" + m_text + "\"");
		}

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

	BlockArgumentType GetType() override
	{
		return BlockArgumentType::STRING;
	}
private:
	std::string m_lastVar;
	std::string m_text;
	std::string m_vText;
	BlockArgumentVariableMode m_vMode;
	sf::Text m_input;
	sf::RectangleShape m_inputBackground;
	sf::RectangleShape m_inputLocHigh;
	sf::RectangleShape m_inputLoc;
	uint64_t m_textLoc;
	uint64_t m_textLocHigh;

	sf::CircleShape* m_left;
	sf::CircleShape* m_right;

	bool m_isDown;
	bool m_selected;
	bool m_variableMode;
	bool m_fullSelect;

	std::function<void(const sf::Event::KeyEvent&)> m_textCallback;
};
