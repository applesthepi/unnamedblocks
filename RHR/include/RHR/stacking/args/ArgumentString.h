#pragma once

#include "Argument.h"
#include "RHR/Global.h"
#include "RHR/handlers/InputHandler.h"

#include <SFML/Window/Clipboard.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <Cappuccino/Logger.h>

class ArgumentString : public Argument
{
public:
	ArgumentString(sf::Vector2u relitivePosition)
		:Argument(relitivePosition)
	{
		m_variableMode = false;

		m_TextAgent = sf::Text(m_Text, *Global::Font, Global::BlockHeight - (Global::BlockBorder * 2));
		m_selected = false;

		m_TextAgent.setFillColor(sf::Color::Black);
		m_TextAgent.setString(m_Text);
		m_TextAgent.setCharacterSize(Global::BlockHeight - Global::BlockBorder);
		m_TextAgent.setPosition(GetAbsolutePosition().x + (int)Global::BlockBorder, GetAbsolutePosition().y);

		m_background = sf::RectangleShape(sf::Vector2f(m_TextAgent.getLocalBounds().width + (float)(Global::BlockBorder * 2), Global::BlockHeight - Global::BlockBorder));
		m_background.setFillColor(sf::Color::White);
		m_background.setPosition(GetAbsolutePosition().x, GetAbsolutePosition().y + (int)(Global::BlockBorder / 2));
		m_background.setOutlineColor(sf::Color::Black);
		m_background.setOutlineThickness(1.0f);

		m_varLeft = sf::RectangleShape(sf::Vector2f(2, Global::BlockHeight - Global::BlockBorder));
		m_varLeft.setFillColor(sf::Color(115, 140, 172));

		m_varRight = sf::RectangleShape(sf::Vector2f(2, Global::BlockHeight - Global::BlockBorder));
		m_varRight.setFillColor(sf::Color(115, 140, 172));

		m_textMarker = sf::RectangleShape(sf::Vector2f(2, Global::BlockHeight - Global::BlockBorder));
		m_textMarker.setFillColor(sf::Color::Black);

		m_textSelect = sf::RectangleShape(sf::Vector2f(0, Global::BlockHeight));
		m_textSelect.setFillColor(sf::Color(35, 60, 117, 80));

		m_functionTextCallback = new std::function<void(const sf::Event::TextEvent& event)>();
		m_functionKeyCallback = new std::function<void(const sf::Event::KeyEvent& event)>();
		*m_functionKeyCallback = [&](const sf::Event::KeyEvent& event)
		{
			if (event.code == sf::Keyboard::Key::Tab)
			{
				Next = true;
			}
			else if (event.code == sf::Keyboard::Left)
			{
				if (event.shift)
				{
					if (m_textMarkerPosition > 0)
						m_textMarkerPosition--;
				}
				else
				{
					if (m_textMarkerPosition == m_textTrailedStart)
					{
						if (m_textMarkerPosition > 0)
						{
							m_textMarkerPosition--;
							m_textTrailedStart--;
						}
					}
					else
					{
						m_textMarkerPosition = std::min(m_textMarkerPosition, m_textTrailedStart);
						m_textTrailedStart = m_textMarkerPosition;
					}
				}
			}
			else if (event.code == sf::Keyboard::Right)
			{
				if (event.shift)
				{
					if (m_textMarkerPosition < m_Text.length())
						m_textMarkerPosition++;
				}
				else
				{
					if (m_textMarkerPosition == m_textTrailedStart)
					{
						if (m_textMarkerPosition < m_Text.length())
						{
							m_textMarkerPosition++;
							m_textTrailedStart++;
						}
					}
					else
					{
						m_textMarkerPosition = std::max(m_textMarkerPosition, m_textTrailedStart);
						m_textTrailedStart = m_textMarkerPosition;
					}
				}
			}
			else if (event.code == sf::Keyboard::BackSpace)
			{
				if (m_textMarkerPosition == m_textTrailedStart)
				{
					if (m_textMarkerPosition > 0)
					{
						m_Text.erase(--m_textMarkerPosition, 1);
						m_TextAgent.setString(m_Text);
						m_textTrailedStart--;
					}
				}
				else
				{
					size_t start = std::min(m_textMarkerPosition, m_textTrailedStart);
					size_t end = std::max(m_textMarkerPosition, m_textTrailedStart);

					m_Text.erase(start, end-start);
					m_TextAgent.setString(m_Text);

					m_textMarkerPosition = std::min(m_textMarkerPosition, m_textTrailedStart);
					m_textTrailedStart = m_textMarkerPosition;
				}
			}
			else if (event.code == sf::Keyboard::Delete)
			{
				if (m_textMarkerPosition == m_textTrailedStart)
				{
					if (m_textMarkerPosition + 1 < m_Text.length())
					{
						m_Text.erase(m_textMarkerPosition, 1);
						m_TextAgent.setString(m_Text);
					}
				}
				else
				{
					size_t start = std::min(m_textMarkerPosition, m_textTrailedStart);
					size_t end = std::max(m_textMarkerPosition, m_textTrailedStart);

					m_Text.erase(start, end-start);
					m_TextAgent.setString(m_Text);

					m_textMarkerPosition = std::min(m_textMarkerPosition, m_textTrailedStart);
					m_textTrailedStart = m_textMarkerPosition;
				}
			}
			else if (event.code == sf::Keyboard::A && event.control)
			{
				m_textTrailedStart = 0;
				m_textMarkerPosition = m_Text.length();
			}
			else if (event.code == sf::Keyboard::C && event.control)
			{
				if (m_textMarkerPosition != m_textTrailedStart)
				{
					std::string cpy;

					for (unsigned int i = std::min(m_textMarkerPosition, m_textTrailedStart); i < std::max(m_textMarkerPosition, m_textTrailedStart); i++)
						cpy += m_Text[i];

					sf::Clipboard::setString(cpy);
				}
			}
			else if (event.code == sf::Keyboard::V && event.control)
			{
				if (m_textMarkerPosition == m_textTrailedStart)
				{
					std::string clip = sf::Clipboard::getString();
					m_Text.insert(m_textMarkerPosition, clip);
					m_TextAgent.setString(m_Text);

					m_textMarkerPosition += clip.length();
					m_textTrailedStart += clip.length();
				}
				else
				{
					size_t start = std::min(m_textMarkerPosition, m_textTrailedStart);
					size_t end = std::max(m_textMarkerPosition, m_textTrailedStart);

					m_Text.erase(start, end-start);

					std::string clip = sf::Clipboard::getString();
					m_textMarkerPosition = std::min(m_textMarkerPosition, m_textTrailedStart) + clip.length();
					m_textTrailedStart = m_textMarkerPosition;

					m_Text.insert(m_textMarkerPosition, clip);
					m_TextAgent.setString(m_Text);

				}
			}
		};
		*m_functionTextCallback = [&](const sf::Event::TextEvent& text)
		{
			// If the character is not printable return
			if(!isprint(static_cast<int32_t>(text.unicode))) return;

			if (m_textMarkerPosition == m_textTrailedStart)
			{
				m_Text.insert(m_Text.begin() + m_textMarkerPosition, (char)text.unicode);
				m_TextAgent.setString(m_Text);

				m_textMarkerPosition++;
				m_textTrailedStart++;
			}
			else
			{
				size_t start = std::min(m_textMarkerPosition, m_textTrailedStart);
				size_t end = std::max(m_textMarkerPosition, m_textTrailedStart);

				m_Text.erase(start, end-start);

				m_Text.insert(m_Text.begin()+m_textMarkerPosition, (char)text.unicode);
				m_TextAgent.setString(m_Text);

				m_textMarkerPosition = std::min(m_textMarkerPosition, m_textTrailedStart) + 1;
				m_textTrailedStart = m_textMarkerPosition;
			}
		};
	}

	void FrameUpdate() override
	{
		m_TextAgent.setString(m_Text);
		m_TextAgent.setCharacterSize(Global::BlockHeight - Global::BlockBorder);
		m_TextAgent.setPosition(GetRelitivePosition().x + (int)Global::BlockBorder, GetRelitivePosition().y);
		
		m_background.setSize(sf::Vector2f(m_TextAgent.getLocalBounds().width + (float)(Global::BlockBorder * 2), Global::BlockHeight - Global::BlockBorder));
		m_background.setPosition(GetRelitivePosition().x, GetRelitivePosition().y + (int)(Global::BlockBorder / 2));

		if (m_variableMode)
		{
			m_varLeft.setPosition(GetRelitivePosition().x, GetRelitivePosition().y + (Global::BlockBorder / 2));
			m_varRight.setPosition(GetRelitivePosition().x + m_TextAgent.getLocalBounds().width + 6, GetRelitivePosition().y + (Global::BlockBorder / 2));

			m_background.setFillColor(sf::Color(255, 217, 168));
		}
		else
		{
			m_background.setFillColor(sf::Color::White);
		}

		if (m_selected && (Global::SelectedArgument != this || Global::Dragging))
		{
			InputHandler::UnregisterTextCallback(m_functionTextCallback);
			InputHandler::UnregisterKeyCallback(m_functionKeyCallback);
			m_selected = false;
		}

		if (m_selected)
		{
			std::string segStr0 = m_Text.substr(0, m_textMarkerPosition);
			sf::Text seg0 = sf::Text(segStr0, *Global::Font, Global::BlockHeight - Global::BlockBorder);

			std::string segStr1 = m_Text.substr(0, m_textTrailedStart);
			sf::Text seg1 = sf::Text(segStr1, *Global::Font, Global::BlockHeight - Global::BlockBorder);

			std::string segStr2 = m_Text.substr(std::min(m_textTrailedStart, m_textMarkerPosition), std::abs((int)m_textTrailedStart - (int)m_textMarkerPosition));
			sf::Text seg2 = sf::Text(segStr2, *Global::Font, Global::BlockHeight - Global::BlockBorder);

			m_textMarker.setPosition(GetRelitivePosition().x + seg0.getLocalBounds().width + (int)Global::BlockBorder, GetRelitivePosition().y + (Global::BlockBorder / 2));
			m_textSelect.setPosition(GetRelitivePosition().x + std::min(seg1.getLocalBounds().width, seg0.getLocalBounds().width) + (int)Global::BlockBorder, GetRelitivePosition().y + (Global::BlockBorder / 2));
			m_textSelect.setSize(sf::Vector2f(seg2.getLocalBounds().width, Global::BlockHeight - Global::BlockBorder));
		}
	}

	void Render(sf::RenderTexture* render) override
	{
		render->draw(m_background);

		if (m_selected)
		{
			render->draw(m_textSelect);
			render->draw(m_TextAgent);
			render->draw(m_textMarker);
		}
		else
			render->draw(m_TextAgent);

		if (m_variableMode)
		{
			render->draw(m_varLeft);
			render->draw(m_varRight);
		}
	}

	unsigned int GetArgumentRawWidth() override
	{
		return m_background.getLocalBounds().width;
	}

	bool MouseButton(bool down, sf::Vector2i position, sf::Mouse::Button button) override
	{
		if (down)
		{
			if (position.x > (int32_t)GetRealAbsolutePosition().x&& position.x < (int32_t)(GetRealAbsolutePosition().x + GetArgumentRawWidth()) &&
				position.y > (int32_t)GetRealAbsolutePosition().y&& position.y < (int32_t)(GetRealAbsolutePosition().y + Global::BlockHeight))
			{
				if (button == sf::Mouse::Left)
				{
					Select();

					return true;
				}
				else if (button == sf::Mouse::Middle)
				{
					SelectGlobaly();
					m_variableMode = !m_variableMode;

					if (m_selected)
					{
						m_textTrailedStart = 0;
						m_textMarkerPosition = m_Text.length();
					}
					else
					{
						InputHandler::RegisterTextCallback(m_functionTextCallback);
						InputHandler::RegisterKeyCallback(m_functionKeyCallback);

						m_selected = true;
						m_textTrailedStart = 0;
						m_textMarkerPosition = m_Text.length();
					}

					return true;
				}
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
		m_Text = data;
		m_TextAgent.setString(m_Text);
	}

	void SetMode(BlockArgumentVariableMode mode) override
	{
		m_variableMode = mode == BlockArgumentVariableMode::VAR;
	}

	std::string* GetData() override
	{
		m_VText = (m_variableMode ? '1' : '0') + m_Text;
		return &m_VText;
	}

	std::string* GetDataRaw() override
	{
		return &m_Text;
	}

	BlockArgumentVariableMode* GetMode() override
	{
		if (m_variableMode)
			m_VMode = BlockArgumentVariableMode::VAR;
		else
			m_VMode = BlockArgumentVariableMode::RAW;

		return &m_VMode;
	}

	void Deallocate() override
	{
		delete m_functionTextCallback;
	}

	void Select() override
	{
		SelectGlobaly();

		if (m_selected)
		{
			int mouseX = Global::MousePosition.x;

			unsigned int closestX = 0;
			unsigned int closestIndex = 0;

			for (unsigned int i = 0; i < m_Text.length(); i++)
			{
				unsigned int distance = std::abs((m_TextAgent.findCharacterPos(i).x + (GetRealAbsolutePosition().x - GetRelitivePosition().x)) - mouseX);
				
				if (distance < closestX || closestX == 0)
				{
					closestX = distance;
					closestIndex = i;

					if (distance == 0)
						break;
				}
			}

			m_textMarkerPosition = closestIndex;
			m_textTrailedStart = m_textMarkerPosition;
		}
		else
		{
			InputHandler::RegisterTextCallback(m_functionTextCallback);
			InputHandler::RegisterKeyCallback(m_functionKeyCallback);

			m_selected = true;
			m_textTrailedStart = 0;
			m_textMarkerPosition = m_Text.length();
		}
	}

	void ReInspectData() override
	{
		m_background.setSize(sf::Vector2f(m_TextAgent.getLocalBounds().width + (float)(Global::BlockBorder * 2), Global::BlockHeight - Global::BlockBorder));
	}

	BlockArgumentType GetType() override
	{
		return BlockArgumentType::STRING;
	}

private:
	std::string m_Text;
	std::string m_VText;
	BlockArgumentVariableMode m_VMode;
	sf::Text m_TextAgent;
	sf::RectangleShape m_background;
	sf::RectangleShape m_textSelect;
	sf::RectangleShape m_textMarker;
	sf::RectangleShape m_varLeft;
	sf::RectangleShape m_varRight;
	unsigned int m_textMarkerPosition;
	unsigned int m_textTrailedStart;
	bool m_selected;
	bool m_variableMode;

	std::function<void(const sf::Event::TextEvent&)>* m_functionTextCallback;
	std::function<void(const sf::Event::KeyEvent&)>* m_functionKeyCallback;
};
