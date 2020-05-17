#pragma once
#include "Argument.h"

#include <Espresso/Global.h>
#include <Espresso/InputHandler.h>
#include <SFML/Window/Clipboard.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <Cappuccino/Logger.h>

#define ARGUMENT_DECOR_REACH 5

class ArgumentReal : public Argument
{
public:
	ArgumentReal(sf::Vector2u relitivePosition)
		:Argument(relitivePosition)
	{
		m_isDown = false;
		m_variableMode = false;
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
			{
				if (m_variableMode)
					InputHandler::RunTextProccess(&m_text, &m_textLocHigh, &m_textLoc, nullptr, nullptr, ev);
				else
					InputHandler::RunNumberProccess(&m_text, &m_textLocHigh, &m_textLoc, nullptr, nullptr, ev);
			}
		};

		// decor
		
		m_leftTop = new sf::ConvexShape(3);
		m_leftTop->setPoint(0, sf::Vector2f(0, 0));
		m_leftTop->setPoint(1, sf::Vector2f(ARGUMENT_DECOR_REACH, (Global::BlockHeight - Global::BlockBorder) / 2));
		m_leftTop->setPoint(2, sf::Vector2f(ARGUMENT_DECOR_REACH, 0));

		m_leftBottom = new sf::ConvexShape(3);
		m_leftBottom->setPoint(0, sf::Vector2f(ARGUMENT_DECOR_REACH, (Global::BlockHeight - Global::BlockBorder) / 2));
		m_leftBottom->setPoint(1, sf::Vector2f(0, Global::BlockHeight - Global::BlockBorder));
		m_leftBottom->setPoint(2, sf::Vector2f(ARGUMENT_DECOR_REACH, Global::BlockHeight - Global::BlockBorder));

		m_rightTop = new sf::ConvexShape(3);
		m_rightTop->setPoint(0, sf::Vector2f(0, 0));
		m_rightTop->setPoint(1, sf::Vector2f(ARGUMENT_DECOR_REACH, 0));
		m_rightTop->setPoint(2, sf::Vector2f(0, (Global::BlockHeight - Global::BlockBorder) / 2));

		m_rightBottom = new sf::ConvexShape(3);
		m_rightBottom->setPoint(0, sf::Vector2f(0, (Global::BlockHeight - Global::BlockBorder) / 2));
		m_rightBottom->setPoint(1, sf::Vector2f(ARGUMENT_DECOR_REACH, Global::BlockHeight - Global::BlockBorder));
		m_rightBottom->setPoint(2, sf::Vector2f(0, Global::BlockHeight - Global::BlockBorder));
	}

	~ArgumentReal()
	{
		delete m_leftTop;
		delete m_leftBottom;

		delete m_rightTop;
		delete m_rightBottom;
	}

	void FrameUpdate() override
	{
		if (m_selected && !m_fullSelect)
			InputHandler::RunMouseProccessFrame(&m_text, sf::Vector2i(m_realAbsolutePosition.x, m_realAbsolutePosition.y + (Global::BlockBorder / 2)), sf::Vector2u(m_inputBackground.getSize().x + (ARGUMENT_DECOR_REACH * 2), m_inputBackground.getSize().y), &m_textLoc, &m_isDown, Global::MousePosition, Global::BlockHeight - Global::BlockBorder, ARGUMENT_DECOR_REACH);

		m_input.setString(m_text);
		m_input.setPosition(GetRelitivePosition().x + (float)Global::BlockBorder + ARGUMENT_DECOR_REACH, GetRelitivePosition().y);

		m_inputBackground.setSize(sf::Vector2f(m_input.getLocalBounds().width + (float)(Global::BlockBorder * 2), Global::BlockHeight - Global::BlockBorder));
		m_inputBackground.setPosition(GetRelitivePosition().x + ARGUMENT_DECOR_REACH, GetRelitivePosition().y + (int)(Global::BlockBorder / 2));

		m_leftTop->setPosition(GetRelitivePosition().x, GetRelitivePosition().y + (int)(Global::BlockBorder / 2));
		m_leftBottom->setPosition(GetRelitivePosition().x, GetRelitivePosition().y + (int)(Global::BlockBorder / 2));
		m_rightTop->setPosition(GetRelitivePosition().x + GetArgumentRawWidth() - ARGUMENT_DECOR_REACH, GetRelitivePosition().y + (int)(Global::BlockBorder / 2));
		m_rightBottom->setPosition(GetRelitivePosition().x + GetArgumentRawWidth() - ARGUMENT_DECOR_REACH, GetRelitivePosition().y + (int)(Global::BlockBorder / 2));

		if (m_variableMode)
		{
			m_inputBackground.setFillColor(MOD_VAR);

			m_leftTop->setFillColor(MOD_VAR);
			m_leftBottom->setFillColor(MOD_VAR);
			m_rightTop->setFillColor(MOD_VAR);
			m_rightBottom->setFillColor(MOD_VAR);

			m_input.setFillColor(sf::Color::Black);
		}
		else
		{
			m_inputBackground.setFillColor(MOD_BUTTON_TEXT_BG);

			m_leftTop->setFillColor(MOD_BUTTON_TEXT_BG);
			m_leftBottom->setFillColor(MOD_BUTTON_TEXT_BG);
			m_rightTop->setFillColor(MOD_BUTTON_TEXT_BG);
			m_rightBottom->setFillColor(MOD_BUTTON_TEXT_BG);

			m_input.setFillColor(MOD_BUTTON_TEXT_FG);
		}

		if (m_selected && (Global::SelectedArgument != this || Global::Dragging))
		{
			InputHandler::UnregisterKeyCallback(&m_textCallback);
			m_selected = false;
		}

		if (m_selected)
		{
			m_inputLoc.setPosition(ARGUMENT_DECOR_REACH + GetRelitivePosition().x + sf::Text(m_text.substr(0, m_textLoc), *Global::Font, Global::BlockHeight - Global::BlockBorder).getLocalBounds().width + (int)Global::BlockBorder, GetRelitivePosition().y + (Global::BlockBorder / 2));
			m_inputLocHigh.setPosition(ARGUMENT_DECOR_REACH + GetRelitivePosition().x + sf::Text(m_text.substr(0, std::min(m_textLocHigh, m_textLoc)), *Global::Font, Global::BlockHeight - Global::BlockBorder).getLocalBounds().width + (int)Global::BlockBorder, GetRelitivePosition().y + (Global::BlockBorder / 2));
			m_inputLocHigh.setSize(sf::Vector2f(sf::Text(m_text.substr(std::min(m_textLocHigh, m_textLoc), std::max(m_textLocHigh, m_textLoc) - std::min(m_textLocHigh, m_textLoc)), *Global::Font, Global::BlockHeight - Global::BlockBorder).getLocalBounds().width, Global::BlockHeight - Global::BlockBorder));
		}
	}

	void Render(sf::RenderTexture* render) override
	{
		render->draw(m_inputBackground);

		render->draw(*m_leftTop);
		render->draw(*m_leftBottom);
		render->draw(*m_rightTop);
		render->draw(*m_rightBottom);

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
		return m_inputBackground.getSize().x + (ARGUMENT_DECOR_REACH * 2);
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

		if (button == sf::Mouse::Middle && down &&
			position.x >= m_realAbsolutePosition.x && position.x <= (ARGUMENT_DECOR_REACH * 2) + m_realAbsolutePosition.x + m_inputBackground.getSize().x &&
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
			TextSystem tSys(&m_text, &m_textLocHigh, &m_textLoc, &m_isDown);

			if (InputHandler::RunMouseProccess(tSys, sf::Vector2i(m_realAbsolutePosition.x, m_realAbsolutePosition.y + (Global::BlockBorder / 2)), sf::Vector2u(m_inputBackground.getSize().x + (ARGUMENT_DECOR_REACH * 2), m_inputBackground.getSize().y), down, position, Global::BlockHeight - Global::BlockBorder, ARGUMENT_DECOR_REACH))
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
		return BlockArgumentType::REAL;
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
	
	sf::ConvexShape* m_leftTop;
	sf::ConvexShape* m_leftBottom;
	sf::ConvexShape* m_rightTop;
	sf::ConvexShape* m_rightBottom;
	
	bool m_isDown;
	bool m_selected;
	bool m_variableMode;
	bool m_fullSelect;

	std::function<void(const sf::Event::KeyEvent&)> m_textCallback;
};
