#include "ButtonImage.h"

#include <Cappuccino/Logger.h>

ButtonImage::ButtonImage(std::function<void()>* callback, const std::string& path, const sf::Vector2f& size)
	:Button(callback, size), m_wasDown(false)
{
	SetImage(path);
}

void ButtonImage::SetImage(const std::string& path)
{
	if (!m_texture.loadFromFile(path))
	{
		Logger::Error("failed to load image \"" + path + "\"");
		m_broken = true;

		return;
	}

	m_sprite.setTexture(m_texture);
}

void ButtonImage::frameUpdate(double deltaTime)
{
	if (m_broken)
		return;

	m_sprite.setScale(getScale());
	m_sprite.setPosition(getPosition());

	setSize(sf::Vector2u(m_texture.getSize().x * getScale().x, m_texture.getSize().y * getScale().y));

	if (Global::MousePosition.x > getPosition().x && Global::MousePosition.x < getPosition().x + static_cast<int32_t>(getSize().x) &&
		Global::MousePosition.y > getPosition().y && Global::MousePosition.y < getPosition().y + static_cast<int32_t>(getSize().y))
		m_sprite.setColor(sf::Color(0, 0, 0, (1.0 - HOVOR_SHADE_LIGHT) * 255));
	else
		m_sprite.setColor(sf::Color::Transparent);
}

const bool ButtonImage::mouseButton(bool down, const sf::Vector2i& position, const sf::Mouse::Button& button)
{
	if (!getEnabled() || m_broken)
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

void ButtonImage::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	if (m_broken)
		return;

	target.draw(m_sprite);
}
