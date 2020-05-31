#include "Field.h"

#include <Espresso/Global.h>
#include <Espresso/InputHandler.h>

Field::Field(const std::string& text, const bool& canSwitch, const FieldType& fieldType)
	:m_textLoc(0), m_textLocHigh(0), m_enabled(false), m_wasDown(false), m_selected(false), m_variableMode(false), m_canSwitch(canSwitch)
{
	m_tab = nullptr;

	m_input = sf::Text(m_text, Global::Font, Global::BlockHeight - Global::BlockBorder);
	m_inputBackground = sf::RectangleShape(sf::Vector2f(m_input.getLocalBounds().width + (float)(Global::BlockBorder * 2), Global::BlockHeight - Global::BlockBorder));

	m_inputLoc = sf::RectangleShape(sf::Vector2f(1, Global::BlockHeight - Global::BlockBorder));
	m_inputLoc.setFillColor(MOD_BUTTON_TEXT_FG);

	m_inputLocHigh = sf::RectangleShape(sf::Vector2f(0, Global::BlockHeight));
	m_inputLocHigh.setFillColor(MOD_HIGHLIGHT_COLOR);

	m_textCallback = [&](const sf::Event::KeyEvent& ev)
	{
		if (ev.code == sf::Keyboard::Key::Tab)
		{
			if (m_tab != nullptr)
				(*m_tab)();
		}
		else
			InputHandler::RunTextProccess(&m_text, &m_textLocHigh, &m_textLoc, nullptr, nullptr, ev);
	};

	SetEnabled(true);
}

void Field::SetText(const std::string& text)
{
	m_text = text;
}

void Field::SetEnabled(const bool& enabled)
{
	m_enabled = enabled;

	if (!enabled && m_selected)
	{
		m_selected = false;
		InputHandler::UnregisterKeyCallback(&m_textCallback);
	}
}

void Field::SetVariableMode(const bool& variableMode)
{
	m_variableMode = variableMode;
}

void Field::SetCalculationOffset(const sf::Vector2f& calculationOffset)
{
	m_calculationOffset = calculationOffset;
}

void Field::SetTab(std::function<void()>* tab)
{
	m_tab = tab;
}

void Field::SelectAll()
{
	if (!m_enabled)
		return;

	m_selected = true;
	m_textLocHigh = m_text.length();
	m_textLoc = 0;
}

const bool Field::GetSelected()
{
	return m_selected;
}

const uint32_t Field::GetWidth()
{
	return m_inputBackground.getSize().x;
}

const std::string& Field::GetText()
{
	return m_text;
}

void Field::frameUpdate(const double& deltaTime)
{
	if (!m_enabled)
		return;

	if (m_selected)
		InputHandler::RunMouseProccessFrame(&m_text, (sf::Vector2i)(getPosition() + sf::Vector2f(0, Global::BlockBorder / 2)), (sf::Vector2u)m_inputBackground.getSize(), &m_textLoc, &m_wasDown, Global::MousePosition, Global::BlockHeight - Global::BlockBorder, 0);

	m_input.setString(m_text);
	m_input.setPosition(getPosition() + sf::Vector2f(Global::BlockBorder, 0.0f));

	m_inputBackground.setSize(sf::Vector2f(m_input.getLocalBounds().width + (float)(Global::BlockBorder * 2), Global::BlockHeight - Global::BlockBorder));
	m_inputBackground.setPosition(getPosition() + sf::Vector2f(0.0f, Global::BlockBorder / 2));

	if (m_variableMode)
	{
		m_inputBackground.setFillColor(MOD_BUTTON_TEXT_BG_ACCENT_STRONG);
		m_input.setFillColor(MOD_BUTTON_TEXT_FG);
	}
	else
	{
		m_inputBackground.setFillColor(MOD_BUTTON_TEXT_BG);
		m_input.setFillColor(MOD_BUTTON_TEXT_FG);
	}

	if (m_selected)
	{
		m_inputLoc.setPosition(getPosition().x + sf::Text(m_text.substr(0, m_textLoc), Global::Font, Global::BlockHeight - Global::BlockBorder).getLocalBounds().width + (int)Global::BlockBorder, getPosition().y + (Global::BlockBorder / 2));
		m_inputLocHigh.setPosition(getPosition().x + sf::Text(m_text.substr(0, std::min(m_textLocHigh, m_textLoc)), Global::Font, Global::BlockHeight - Global::BlockBorder).getLocalBounds().width + (int)Global::BlockBorder, getPosition().y + (Global::BlockBorder / 2));
		m_inputLocHigh.setSize(sf::Vector2f(sf::Text(m_text.substr(std::min(m_textLocHigh, m_textLoc), std::max(m_textLocHigh, m_textLoc) - std::min(m_textLocHigh, m_textLoc)), Global::Font, Global::BlockHeight - Global::BlockBorder).getLocalBounds().width, Global::BlockHeight - Global::BlockBorder));
	}
}

const bool Field::mouseButton(const bool& down, const sf::Vector2i& position, const sf::Mouse::Button& button)
{
	if (!m_enabled)
		return;

	if (m_canSwitch && button == sf::Mouse::Middle && down &&
		Global::MousePosition.x >= getPosition().x + m_calculationOffset.x && Global::MousePosition.x <= getPosition().x + m_inputBackground.getSize().x + m_calculationOffset.x &&
		Global::MousePosition.y >= getPosition().y + m_calculationOffset.y && Global::MousePosition.y <= getPosition().y + m_inputBackground.getSize().y + m_calculationOffset.y)
	{
		m_variableMode = !m_variableMode;

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
		TextSystem tSys(&m_text, &m_textLocHigh, &m_textLoc, &m_wasDown);

		if (InputHandler::RunMouseProccess(tSys, (sf::Vector2i)(getPosition() + m_calculationOffset + sf::Vector2f(0.0f, Global::BlockBorder / 2)), sf::Vector2u(m_inputBackground.getSize().x, m_inputBackground.getSize().y), down, position, Global::BlockHeight - Global::BlockBorder, 0))
		{
			if (down)
			{
				if (m_selected)
					InputHandler::RunMouseProccess(&m_input, &m_textLocHigh, &m_textLoc, &m_wasDown, true, Global::MousePosition, Global::BlockHeight - Global::BlockBorder);
				else
				{
					InputHandler::RegisterKeyCallback(&m_textCallback);

					m_selected = true;
					m_textLocHigh = 0;
					m_textLoc = m_text.length() + 1;
				}
			}

			return true;
		}
		else
		{
			if (m_selected)
			{
				m_selected = false;
				InputHandler::UnregisterKeyCallback(&m_textCallback);
			}

			return false;
		}
	}

	return false;
}

void Field::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	if (!m_enabled)
		return;

	target.draw(m_inputBackground);
	target.draw(m_input);

	if (m_selected)
	{
		target.draw(m_inputLocHigh);
		target.draw(m_inputLoc);
	}
}
