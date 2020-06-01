#include "ButtonText.h"

ButtonText::ButtonText(std::function<void()>* callback, const std::string& text, const uint16_t& charSize, const sf::Vector2f& size, const sf::Color& background, const sf::Color& forground)
	:Button(callback, size), m_wasDown(false), m_colorBackground(background), m_colorForground(forground)
{
	m_background.setFillColor(background);

	m_text = sf::Text(text, Global::Font, charSize);
	m_text.setFillColor(forground);
}

void ButtonText::SetText(const std::string& text)
{
	m_text.setString(text);
}

void ButtonText::frameUpdate(const double& deltaTime)
{
	m_background.setSize((sf::Vector2f)getSize());
	m_background.setPosition(getPosition());

	m_text.setPosition(getPosition() + sf::Vector2f(2.0f, 0.0f));

	if (Global::MousePosition.x > getPosition().x && Global::MousePosition.x < getPosition().x + static_cast<int32_t>(getSize().x) &&
		Global::MousePosition.y > getPosition().y && Global::MousePosition.y < getPosition().y + static_cast<int32_t>(getSize().y))
	{
		m_background.setFillColor(sf::Color(m_colorBackground.r * HOVOR_SHADE_LIGHT, m_colorBackground.g * HOVOR_SHADE_LIGHT, m_colorBackground.b * HOVOR_SHADE_LIGHT));
		m_background.setOutlineColor(sf::Color(m_colorBackground.r * HOVOR_SHADE_HARD, m_colorBackground.g * HOVOR_SHADE_HARD, m_colorBackground.b * HOVOR_SHADE_HARD));
		m_background.setOutlineThickness(1.0f);
	}
	else
	{
		m_background.setFillColor(m_colorBackground);
		m_background.setOutlineThickness(0.0f);
	}
}

const bool ButtonText::mouseButton(const bool& down, const sf::Vector2i& position, const sf::Mouse::Button& button)
{
	if (!getEnabled())
		return false;

	if (button == sf::Mouse::Left)
	{
		bool over = position.x > getPosition().x && position.x < getPosition().x + static_cast<int32_t>(getSize().x) && position.y > getPosition().y && position.y < getPosition().y + static_cast<int32_t>(getSize().y);
		if (over && down && !m_wasDown)
		{
			m_wasDown = true;
			return true;
		}
		else if (over && !down && m_wasDown)
		{
			m_wasDown = false;
			(*m_callback)();

			return true;
		}
		else if (!over && !down)
		{
			m_wasDown = false;
			return false;
		}
	}

	return false;
}

void ButtonText::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	target.draw(m_background);
	target.draw(m_text);
}
