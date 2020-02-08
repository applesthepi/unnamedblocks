#pragma once

#include "Argument.h"
#include "Global.h"
#include "ui/TypingSystem.h"

#include <iostream>

class ArgumentBoolean : public Argument
{
public:
	ArgumentBoolean(sf::Vector2u relitivePosition)
		:Argument(relitivePosition)
	{
		m_value = false;
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

		m_functionTextCallback = new std::function<void(int)>();
		*m_functionTextCallback = [&](int key)
		{
			if (key == 129)
				m_shiftEnabled = !m_shiftEnabled;
			else if (key == 9)
			{
				Next = true;
			}
			else if (key == -2)
			{
				if (m_shiftEnabled)
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
			else if (key == -4)
			{
				if (m_shiftEnabled)
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
			else if (key == 8)
			{
				if (m_textMarkerPosition == m_textTrailedStart)
				{
					if (m_textMarkerPosition > 0)
					{
						std::string newText = std::string();

						for (unsigned int i = 0; i < m_textMarkerPosition - 1; i++)
							newText += m_Text[i];

						for (unsigned int i = m_textMarkerPosition; i < m_Text.length(); i++)
							newText += m_Text[i];

						m_Text = newText;
						m_TextAgent.setString(m_Text);

						m_textMarkerPosition--;
						m_textTrailedStart--;
					}
				}
				else
				{
					std::string newText = std::string();

					for (unsigned int i = 0; i < std::min(m_textMarkerPosition, m_textTrailedStart); i++)
						newText += m_Text[i];

					for (unsigned int i = std::max(m_textMarkerPosition, m_textTrailedStart); i < m_Text.length(); i++)
						newText += m_Text[i];

					m_Text = newText;
					m_TextAgent.setString(m_Text);

					m_textMarkerPosition = std::min(m_textMarkerPosition, m_textTrailedStart);
					m_textTrailedStart = m_textMarkerPosition;
				}
			}
			else if (key == 127)
			{
				if (m_textMarkerPosition == m_textTrailedStart)
				{
					if (m_textMarkerPosition + 1 < m_Text.length())
					{
						std::string newText = std::string();

						for (unsigned int i = 0; i < m_textMarkerPosition; i++)
							newText += m_Text[i];

						for (unsigned int i = m_textMarkerPosition + 1; i < m_Text.length(); i++)
							newText += m_Text[i];

						m_Text = newText;
						m_TextAgent.setString(m_Text);
					}
				}
				else
				{
					std::string newText = std::string();

					for (unsigned int i = 0; i < std::min(m_textMarkerPosition, m_textTrailedStart); i++)
						newText += m_Text[i];

					for (unsigned int i = std::max(m_textMarkerPosition, m_textTrailedStart); i < m_Text.length(); i++)
						newText += m_Text[i];

					m_Text = newText;
					m_TextAgent.setString(m_Text);

					m_textMarkerPosition = std::min(m_textMarkerPosition, m_textTrailedStart);
					m_textTrailedStart = m_textMarkerPosition;
				}
			}
			else if (key == 130)
			{
				m_textTrailedStart = 0;
				m_textMarkerPosition = m_Text.length();
			}
			else if (key == 131)
			{
				if (m_textMarkerPosition != m_textTrailedStart)
				{
					std::string cpy;

					for (unsigned int i = std::min(m_textMarkerPosition, m_textTrailedStart); i < std::max(m_textMarkerPosition, m_textTrailedStart); i++)
						cpy += m_Text[i];

					sf::Clipboard::setString(cpy);
				}
			}
			else if (key == 132)
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
					std::string newText;
					for (unsigned int i = 0; i < std::min(m_textMarkerPosition, m_textTrailedStart); i++)
						newText += m_Text[i];

					std::string clip = sf::Clipboard::getString();

					newText += clip;

					for (unsigned int i = std::max(m_textMarkerPosition, m_textTrailedStart); i < m_Text.length(); i++)
						newText += m_Text[i];

					m_Text = newText;
					m_TextAgent.setString(m_Text);

					m_textMarkerPosition = std::min(m_textMarkerPosition, m_textTrailedStart) + clip.length();
					m_textTrailedStart = m_textMarkerPosition;
				}
			}
			else
			{
				if (m_textMarkerPosition == m_textTrailedStart)
				{
					m_Text.insert(m_Text.begin() + m_textMarkerPosition, (char)key);
					m_TextAgent.setString(m_Text);

					m_textMarkerPosition++;
					m_textTrailedStart++;
				}
				else
				{
					std::string newText = std::string();
					for (unsigned int i = 0; i < std::min(m_textMarkerPosition, m_textTrailedStart); i++)
						newText += m_Text[i];

					newText += (char)key;

					for (unsigned int i = std::max(m_textMarkerPosition, m_textTrailedStart); i < m_Text.length(); i++)
						newText += m_Text[i];

					m_Text = newText;
					m_TextAgent.setString(m_Text);

					m_textMarkerPosition = std::min(m_textMarkerPosition, m_textTrailedStart) + 1;
					m_textTrailedStart = m_textMarkerPosition;
				}
			}
		};
	}

	void FrameUpdate() override
	{
		m_TextAgent.setString(m_Text);
		m_TextAgent.setCharacterSize(Global::BlockHeight - Global::BlockBorder);
		m_TextAgent.setPosition(GetRelitivePosition().x + (int)Global::BlockBorder, GetRelitivePosition().y);

		if (m_variableMode)
			m_background.setSize(sf::Vector2f(m_TextAgent.getLocalBounds().width + (float)(Global::BlockBorder * 2), Global::BlockHeight - Global::BlockBorder));
		else
			m_background.setSize(sf::Vector2f(Global::BlockHeight - Global::BlockBorder, Global::BlockHeight - Global::BlockBorder));

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
			TypingSystem::RemoveKeypressRegister(m_functionTextCallback);
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

		if (!m_variableMode)
		{
			if (m_value)
			{
				m_background.setFillColor(sf::Color::Green);
			}
			else
			{
				m_background.setFillColor(sf::Color::Red);
			}
		}
	}

	void Render(sf::RenderTexture* render) override
	{
		render->draw(m_background);

		if (m_variableMode)
		{
			if (m_selected)
			{
				render->draw(m_textSelect);
				render->draw(m_TextAgent);
				render->draw(m_textMarker);
			}
			else
				render->draw(m_TextAgent);
		}
	}

	unsigned int GetArgumentRawWidth() override
	{
		return m_background.getSize().x;
	}

	bool MouseButton(bool down, sf::Vector2i position, sf::Mouse::Button button) override
	{
		if (down)
		{
			if (position.x > (int32_t)GetRealAbsolutePosition().x&& position.x < (int32_t)(GetRealAbsolutePosition().x + GetArgumentRawWidth()) &&
				position.y > GetRealAbsolutePosition().y&& position.y < (int32_t)(GetRealAbsolutePosition().y + Global::BlockHeight))
			{
				if (button == sf::Mouse::Left)
				{
					if (m_variableMode)
					{
						Select();
					}
					else
					{
						m_value = !m_value;
					}

					return true;
				}
				else if (button == sf::Mouse::Middle)
				{
					Global::SelectedArgument = (void*)this;
					m_variableMode = !m_variableMode;

					if (m_selected)
					{
						m_shiftEnabled = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);

						m_textTrailedStart = 0;
						m_textMarkerPosition = m_Text.length();
					}
					else
					{
						TypingSystem::AddKeypressRegister(m_functionTextCallback);

						m_shiftEnabled = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
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
		if (m_variableMode)
		{
			m_value = false;
			m_Text = data;
			m_TextAgent.setString(m_Text);
		}
		else
		{
			m_value = data == "1";
			m_Text = "";
			m_TextAgent.setString(m_Text);
		}
	}

	void SetMode(BlockArgumentVariableMode mode) override
	{
		m_variableMode = mode == BlockArgumentVariableMode::VAR;
	}

	std::string GetData() override
	{
		if (m_variableMode)
			return "1" + m_Text;
		else
			return m_value ? "01" : "00";
	}

	void Deallocate() override
	{
		delete m_functionTextCallback;
	}

	void Select() override
	{
		Global::SelectedArgument = (void*)this;

		if (m_selected)
		{
			m_shiftEnabled = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);

			int mouseX = Global::MousePosition.x;

			unsigned int closestX = 0;
			unsigned int closestIndex = 0;

			for (unsigned int i = 0; i < m_Text.length(); i++)
			{
				unsigned int distance = std::abs((m_TextAgent.findCharacterPos(i).x + (GetRealAbsolutePosition().x - GetAbsolutePosition().x)) - mouseX);

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
			TypingSystem::AddKeypressRegister(m_functionTextCallback);

			m_shiftEnabled = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
			m_selected = true;
			m_textTrailedStart = 0;
			m_textMarkerPosition = m_Text.length();
		}
	}

	void ReInspectData() override
	{
		if (m_variableMode)
			m_background.setSize(sf::Vector2f(m_TextAgent.getLocalBounds().width + (float)(Global::BlockBorder * 2), Global::BlockHeight - Global::BlockBorder));
		else
			m_background.setSize(sf::Vector2f(Global::BlockHeight - Global::BlockBorder, Global::BlockHeight - Global::BlockBorder));
	}

private:
	bool m_value;
	std::string m_Text;
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
	bool m_shiftEnabled;

	std::function<void(int)>* m_functionTextCallback;
};
