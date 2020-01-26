#include "Argument.h"
#include "Global.h"

Argument::Argument(sf::Vector2u relitivePosition)
{
	m_relitivePosition = relitivePosition;
	Next = false;
}

void Argument::SetupInBlock(sf::Vector2i* blockRelitive, sf::Vector2i* blockAbsolute)
{
	m_blockRelitive = blockRelitive;
	m_blockAbsolute = blockAbsolute;
}

void Argument::Deallocate()
{

}

void Argument::Render(sf::RenderTexture* render, sf::RenderWindow* window)
{
	if (render == nullptr)
	{
		sf::RectangleShape basic = sf::RectangleShape(sf::Vector2f(GetArgumentRawWidth(), Global::BlockHeight));
		basic.setFillColor(sf::Color::Magenta);
		basic.setPosition(m_absolutePosition.x, m_absolutePosition.y);

		window->draw(basic);
	}
	else
	{
		sf::RectangleShape basic = sf::RectangleShape(sf::Vector2f(GetArgumentRawWidth(), Global::BlockHeight));
		basic.setFillColor(sf::Color::Magenta);
		basic.setPosition(m_absolutePosition.x, m_absolutePosition.y);

		render->draw(basic);
	}
}

void Argument::FrameUpdate(sf::RenderWindow* window)
{

}

unsigned int Argument::GetArgumentRawWidth()
{
	return 0;
}

bool Argument::MouseButton(bool down, sf::Vector2i position, sf::Mouse::Button button)
{
	return false;
}

bool Argument::HasData()
{
	return false;
}

void Argument::SetData(std::string data)
{

}

void Argument::Select()
{

}

std::string Argument::GetData()
{
	return std::string();
}

void Argument::Update(sf::RenderWindow* window, bool global)
{
	if (global)
	{
		m_absolutePosition.x = (int)m_relitivePosition.x + m_blockAbsolute->x;
		m_absolutePosition.y = (int)m_relitivePosition.y + m_blockAbsolute->y;
	}
	else
	{
		m_absolutePosition.x = (int)m_relitivePosition.x + m_blockRelitive->x;
		m_absolutePosition.y = (int)m_relitivePosition.y + m_blockRelitive->y;
	}

	m_realAbsolutePosition.x = (int)m_relitivePosition.x + m_blockAbsolute->x;
	m_realAbsolutePosition.y = (int)m_relitivePosition.y + m_blockAbsolute->y;

	FrameUpdate(window);
}

void Argument::SetRelitivePosition(sf::Vector2u relitivePosition)
{
	m_relitivePosition = relitivePosition;
}

bool Argument::GetNext()
{
	if (Next)
	{
		Next = false;
		return true;
	}
	else
		return false;
}

sf::Vector2i Argument::GetAbsolutePosition()
{
	return m_absolutePosition;
}

sf::Vector2i Argument::GetRealAbsolutePosition()
{
	return m_realAbsolutePosition;
}

sf::Vector2u Argument::GetRelitivePosition()
{
	return m_relitivePosition;
}
