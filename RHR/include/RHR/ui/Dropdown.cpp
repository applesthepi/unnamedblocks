#include "Dropdown.h"

#include <Espresso/Global.h>

#define DEFAULT_WIDTH 50

Dropdown::Dropdown(const sf::Vector2i& position)
	:m_enabled(false), m_open(false), m_selected(0), m_position(position), m_width(DEFAULT_WIDTH)
{
	
}

void Dropdown::FrameUpdate()
{
	if (m_buttons.size() == 0)
		return;

	if (m_enabled && m_open)
	{
		for (uint16_t i = 0; i < m_buttons.size(); i++)
			m_buttons[i].FrameUpdate();
	}
	else
		m_buttons.front().FrameUpdate();
}

void Dropdown::SetEnabled(const bool& enabled)
{
	m_enabled = enabled;

	if (!enabled)
		SetOpen(false);
}

void Dropdown::SetOpen(const bool& open)
{
	m_open = open;
}

void Dropdown::Select(const uint64_t& idx)
{
	if (m_enabled && m_open)
	{
		m_selected = idx;

		SetOpen(false);
		Reload();
	}
}

void Dropdown::Toggle()
{
	SetOpen(!m_open);
}

void Dropdown::Reload()
{
	if (m_buttons.size() > 0)
		m_buttons.front().SetText(m_elements[m_selected]);
}

void Dropdown::SetElements(const std::vector<std::string>& elements, const uint64_t& idx)
{
	m_elements = elements;

	m_buttonCallbacks.clear();
	m_buttons.clear();
	m_selected = idx;
	m_enabled = false;
	m_open = false;
	m_width = DEFAULT_WIDTH;

	// keeps pointers the same
	m_buttonCallbacks.reserve(elements.size());
	m_buttons.reserve(elements.size());

	for (uint16_t i = 0; i < elements.size(); i++)
	{
		uint16_t width = sf::Text(elements[i], *Global::Font, Global::BlockHeight - Global::BlockBorder).getLocalBounds().width;

		if (width > m_width)
			m_width = width;
	}

	{
		m_buttonCallbacks.push_back(std::function<void()>());
		m_buttonCallbacks.back() = [this]()
		{
			Toggle();
		};

		m_buttons.push_back(Button(m_position, sf::Vector2u(m_width, Global::BlockHeight - Global::BlockBorder), &m_buttonCallbacks.back()));
		m_buttons.back().SetButtonModeText(elements[idx], MOD_BUTTON_TEXT_BG_ACCENT_WEAK, MOD_BUTTON_TEXT_FG, Global::BlockHeight - Global::BlockBorder);
	}

	for (uint16_t i = 0; i < elements.size(); i++)
	{
		m_buttonCallbacks.push_back(std::function<void()>());
		m_buttonCallbacks.back() = [this, i]()
		{
			Select(i);
		};

		m_buttons.push_back(Button(m_position, sf::Vector2u(m_width, Global::BlockHeight - Global::BlockBorder), &m_buttonCallbacks.back()));
		m_buttons.back().SetButtonModeText(elements[i], MOD_BUTTON_TEXT_BG_ACCENT_WEAK, MOD_BUTTON_TEXT_FG, Global::BlockHeight - Global::BlockBorder);
	}

	Reload();
	SetEnabled(true);
}

const uint16_t Dropdown::GetSelected()
{
	return m_selected;
}

const uint64_t Dropdown::GetWidth()
{
	return m_width;
}

const bool Dropdown::MouseButton(const bool& down, const sf::Vector2i& position, const sf::Mouse::Button& button)
{
	if (!m_enabled || m_buttons.size() == 0)
		return false;

	if (m_open)
	{
		bool hit = false;

		for (uint16_t i = 0; i < m_buttons.size(); i++)
		{
			if (m_buttons[i].MouseButton(down, position, button))
				hit = true;
		}

		return hit;
	}
	else
	{
		return m_buttons.front().MouseButton(down, position, button);
	}
}

void Dropdown::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	if (m_buttons.size() == 0)
		return;

	if (m_enabled && m_open)
	{
		for (uint16_t i = 0; i < m_buttons.size(); i++)
			target.draw(m_buttons[i]);
	}
	else
		target.draw(m_buttons.front());
}
