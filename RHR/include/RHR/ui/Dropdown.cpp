#include "Dropdown.h"

#include <Espresso/Global.h>

Dropdown::Dropdown(const std::vector<std::string>& elements, const uint64_t& idx, const sf::Vector2i& position)
	:m_enabled(false), m_open(false), m_selected(idx), m_position(position)
{
	m_buttonCallbacks.reserve(elements.size());
	m_buttons.reserve(elements.size());

	uint64_t highestWidth = 50;

	for (uint64_t i = 0; i < elements.size(); i++)
	{
		uint64_t width = sf::Text(elements[i], *Global::Font, Global::BlockHeight - Global::BlockBorder).getLocalBounds().width;

		if (width > highestWidth)
			highestWidth = width;
	}

	for (uint64_t i = 0; i < elements.size(); i++)
	{
		m_buttonCallbacks.push_back(std::function<void()>());
		m_buttonCallbacks.back() = [this, i]()
		{
			Select(i);
		};

		m_buttons.push_back(Button(m_position, sf::Vector2u(highestWidth, Global::BlockHeight - Global::BlockBorder), &m_buttonCallbacks.back()));
	}

	Reload();
}

void Dropdown::Render(sf::RenderTexture* render)
{

}

void Dropdown::SetEnabled(const bool& enabled)
{
	m_enabled = enabled;
}

void Dropdown::Select(const uint64_t& idx)
{
	if (m_enabled && m_open)
	{
		m_selected = idx;
		m_open = false;

		Reload();
	}
}

void Dropdown::Reload()
{

}

const uint64_t Dropdown::GetSelected()
{

}

const uint64_t Dropdown::GetWidth()
{

}

