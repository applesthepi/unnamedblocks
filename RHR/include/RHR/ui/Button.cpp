#include "Button.h"

#include <Espresso/Global.h>

/*

#define HOVOR_SHADE_HARD 0.7
#define HOVOR_SHADE_LIGHT 0.85

Button::Button(sf::Vector2i position, sf::Vector2u size, std::function<void()>* functionCallback)
{
	m_wasDown = false;
	m_position = position;
	m_size = size;
	m_mode = ButtonMode::Unconfigured;
	m_functionCallback = functionCallback;
	m_enabled = true;
}

void Button::SetButtonModeColor(sf::Color color)
{
	if (m_mode != ButtonMode::Unconfigured)
	{
		if (m_mode == ButtonMode::Text)
		{
			delete m_modeText;
			delete m_modeColor;
		}
		else if (m_mode == ButtonMode::Image)
		{
			delete m_modeImageTexture;
			delete m_modeImageSprite;
			delete m_modeColor;
		}
	}

	m_mode = ButtonMode::Color;
	m_modeColor = new sf::RectangleShape(sf::Vector2f(m_size.x, m_size.y));
	m_modeColor->setFillColor(color);
	m_modeColor->setPosition(m_position.x, m_position.y);

	m_backgroundColor = color;
}

void Button::SetButtonModeText(std::string text, const sf::Color& background, const sf::Color& forground, unsigned short charSize)
{
	if (m_mode != ButtonMode::Unconfigured)
	{
		if (m_mode == ButtonMode::Color)
			delete m_modeColor;
		else if (m_mode == ButtonMode::Image)
		{
			delete m_modeImageTexture;
			delete m_modeImageSprite;
			delete m_modeColor;
		}
	}

	m_mode = ButtonMode::Text;
	m_modeText = new sf::Text();

	m_modeText->setFont(*Global::Font);
	m_modeText->setFillColor(forground);
	m_modeText->setPosition(m_position.x + 2, m_position.y);
	m_modeText->setString(text);
	m_modeText->setCharacterSize(charSize);

	m_modeColor = new sf::RectangleShape(sf::Vector2f(m_size.x, m_size.y));
	m_modeColor->setFillColor(background);
	m_modeColor->setPosition(m_position.x, m_position.y);

	m_backgroundColor = background;
}

void Button::SetButtonModeImage(std::string path)
{
	if (m_mode != ButtonMode::Unconfigured)
	{
		if (m_mode == ButtonMode::Color)
			delete m_modeColor;
		else if (m_mode == ButtonMode::Text)
		{
			delete m_modeText;
			delete m_modeColor;
			delete m_modeColor;
		}
	}

	m_mode = ButtonMode::Image;
	m_modeImageTexture = new sf::Texture();
	m_modeImageSprite = new sf::Sprite();
	
	m_modeImageTexture->loadFromFile(path);

	m_modeImageSprite->setTexture(*m_modeImageTexture);
	m_modeImageSprite->setPosition(m_position.x, m_position.y);

	m_modeColor = new sf::RectangleShape(sf::Vector2f(m_size.x, m_size.y));
	m_modeColor->setPosition(m_position.x, m_position.y);
}

void Button::FrameUpdate()
{
	if (!m_enabled)
		return;

	sf::Vector2i pos = Global::MousePosition;

	if (m_mode == ButtonMode::Color)
	{
		m_modeColor->setPosition((sf::Vector2f)m_position);
		m_modeColor->setSize((sf::Vector2f)m_size);

		if (pos.x > m_position.x && pos.x < m_position.x + static_cast<int32_t>(m_size.x) && pos.y > m_position.y && pos.y < m_position.y + static_cast<int32_t>(m_size.y))
		{
			m_modeColor->setFillColor(sf::Color(m_backgroundColor.r * HOVOR_SHADE_LIGHT, m_backgroundColor.g * HOVOR_SHADE_LIGHT, m_backgroundColor.b * HOVOR_SHADE_LIGHT));
			m_modeColor->setOutlineColor(sf::Color(m_backgroundColor.r * HOVOR_SHADE_HARD, m_backgroundColor.g * HOVOR_SHADE_HARD, m_backgroundColor.b * HOVOR_SHADE_HARD));
			m_modeColor->setOutlineThickness(1.0f);
		}
		else
		{
			m_modeColor->setFillColor(m_backgroundColor);
			m_modeColor->setOutlineThickness(0.0f);
		}
	}
	else if (m_mode == ButtonMode::Image)
	{
		m_modeImageSprite->setPosition((sf::Vector2f)m_position);

		m_modeColor->setPosition((sf::Vector2f)m_position);
		m_modeColor->setSize((sf::Vector2f)m_size);

		if (pos.x > m_position.x && pos.x < m_position.x + static_cast<int32_t>(m_size.x) && pos.y > m_position.y && pos.y < m_position.y + static_cast<int32_t>(m_size.y))
		{
			m_modeColor->setFillColor(sf::Color(0, 0, 0, (1 - HOVOR_SHADE_LIGHT) * 255));
			m_modeColor->setOutlineColor(sf::Color(0, 0, 0, (1 - HOVOR_SHADE_HARD) * 255));
			m_modeColor->setOutlineThickness(1.0f);
		}
		else
		{
			m_modeColor->setFillColor(sf::Color(0, 0, 0, 0));
			m_modeColor->setOutlineThickness(0.0f);
		}
	}
	else if (m_mode == ButtonMode::Text)
	{
		m_modeColor->setPosition((sf::Vector2f)m_position);
		m_modeColor->setSize((sf::Vector2f)m_size);

		m_modeText->setPosition(m_position.x + 2, m_position.y);

		if (pos.x > m_position.x && pos.x < m_position.x + static_cast<int32_t>(m_size.x) && pos.y > m_position.y && pos.y < m_position.y + static_cast<int32_t>(m_size.y))
		{
			m_modeColor->setFillColor(sf::Color(m_backgroundColor.r * HOVOR_SHADE_LIGHT, m_backgroundColor.g * HOVOR_SHADE_LIGHT, m_backgroundColor.b * HOVOR_SHADE_LIGHT));
			m_modeColor->setOutlineColor(sf::Color(m_backgroundColor.r * HOVOR_SHADE_HARD, m_backgroundColor.g * HOVOR_SHADE_HARD, m_backgroundColor.b * HOVOR_SHADE_HARD));
			m_modeColor->setOutlineThickness(1.0f);
		}
		else
		{
			m_modeColor->setFillColor(m_backgroundColor);
			m_modeColor->setOutlineThickness(0.0f);
		}
	}
}

void Button::Render(sf::RenderTarget& target)
{
	if (!m_enabled)
		return;

	if (m_mode == ButtonMode::Color)
	{
		target.draw(*m_modeColor);
	}
	else if (m_mode == ButtonMode::Image)
	{
		target.draw(*m_modeColor);
		target.draw(*m_modeImageSprite);
	}
	else if (m_mode == ButtonMode::Text)
	{
		target.draw(*m_modeColor);
		target.draw(*m_modeText);
	}
}

void Button::SetSize(sf::Vector2u size)
{
	m_size = size;
}

void Button::SetPosition(sf::Vector2i position)
{
	m_position = position;
}

bool Button::MouseButton(bool down, sf::Vector2i position, sf::Mouse::Button button)
{
	if (!m_enabled)
		return false;

	if (button == sf::Mouse::Left)
	{
		bool over = position.x > m_position.x&& position.x < m_position.x + static_cast<int32_t>(m_size.x) && position.y > m_position.y&& position.y < m_position.y + static_cast<int32_t>(m_size.y);
		if (over && down && !m_wasDown)
		{
			m_wasDown = true;
			return true;
		}
		else if (over && !down && m_wasDown)
		{
			m_wasDown = false;
			(*m_functionCallback)();
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

void Button::SetEnabled(const bool& enabled)
{
	m_enabled = enabled;
}

void Button::SetImage(const std::string& path)
{
	m_modeImageTexture->loadFromFile(path);
	m_modeImageSprite->setTexture(*m_modeImageTexture);
}

void Button::SetText(const std::string& text)
{
	m_modeText->setString(text);
}

*/