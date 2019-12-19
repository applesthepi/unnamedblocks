#include "Button.h"
#include "Global.h"

Button::Button(sf::Vector2i position, sf::Vector2u size, std::function<void()>* functionCallback)
{
	m_wasDown = false;
	m_position = position;
	m_size = size;
	m_functionCallback = functionCallback;
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
		}
	}

	m_mode = ButtonMode::Color;
	m_modeColor = new sf::RectangleShape(sf::Vector2f(m_size.x, m_size.y));
	m_modeColor->setFillColor(color);
	m_modeColor->setPosition(m_position.x, m_position.y);
}

void Button::SetButtonModeText(std::string text, sf::Color backgroundColor, unsigned short charSize)
{
	if (m_mode != ButtonMode::Unconfigured)
	{
		if (m_mode == ButtonMode::Color)
			delete m_modeColor;
		else if (m_mode == ButtonMode::Image)
		{
			delete m_modeImageTexture;
			delete m_modeImageSprite;
		}
	}

	m_mode = ButtonMode::Text;
	m_modeText = new sf::Text();

	m_modeText->setFont(*Global::Font);
	m_modeText->setFillColor(sf::Color::Black);
	m_modeText->setPosition(m_position.x + 2, m_position.y);
	m_modeText->setString(text);
	m_modeText->setCharacterSize(charSize);

	m_modeColor = new sf::RectangleShape(sf::Vector2f(m_size.x, m_size.y));
	m_modeColor->setFillColor(backgroundColor);
	m_modeColor->setPosition(m_position.x, m_position.y);
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
		}
	}

	m_mode = ButtonMode::Image;
	m_modeImageTexture = new sf::Texture();
	m_modeImageSprite = new sf::Sprite();
	
	m_modeImageTexture->loadFromFile("res/" + path);

	m_modeImageSprite->setTexture(*m_modeImageTexture);
	m_modeImageSprite->setPosition(m_position.x, m_position.y);
}

void Button::FrameUpdate(sf::RenderWindow* window)
{
	if (m_mode == ButtonMode::Color)
	{
		m_modeColor->setPosition((sf::Vector2f)m_position);
		m_modeColor->setSize((sf::Vector2f)m_size);
	}
	else if (m_mode == ButtonMode::Image)
	{
		m_modeImageSprite->setPosition((sf::Vector2f)m_position);
		m_modeImageSprite->setScale((sf::Vector2f)m_size);
	}
	else if (m_mode == ButtonMode::Text)
	{
		m_modeColor->setPosition((sf::Vector2f)m_position);
		m_modeColor->setSize((sf::Vector2f)m_size);

		m_modeText->setPosition(m_position.x + 2, m_position.y);
	}
}

void Button::Render(sf::RenderWindow* window)
{
	if (m_mode == ButtonMode::Color)
	{
		window->draw(*m_modeColor);
	}
	else if (m_mode == ButtonMode::Image)
	{
		window->draw(*m_modeImageSprite);
	}
	else if (m_mode == ButtonMode::Text)
	{
		window->draw(*m_modeColor);
		window->draw(*m_modeText);
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
	if (button == sf::Mouse::Left)
	{
		bool over = position.x > m_position.x&& position.x < (int32_t)(m_position.x + m_size.x) && position.y > m_position.y&& position.y < (int32_t)(m_position.y + m_size.y);
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

