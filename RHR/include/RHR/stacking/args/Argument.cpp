#include "Argument.h"
#include "Global.h"
#include "handlers/Logger.h"

Argument::Argument(sf::Vector2u relitivePosition)
{
	m_relitivePosition = relitivePosition;
	Next = false;
}

void Argument::SetupInBlock(sf::Vector2i* blockRelitive, sf::Vector2i* blockAbsolute, std::function<void()>* functionUpdatePreTexture, std::function<void()>* functionSelect)
{
	m_blockRelitive = blockRelitive;
	m_blockAbsolute = blockAbsolute;
	m_functionUpdatePreTexture = functionUpdatePreTexture;
	m_functionSelect = functionSelect;

	FrameUpdate();
}

void Argument::Deallocate()
{

}

void Argument::Render(sf::RenderTexture* render)
{
	sf::RectangleShape basic = sf::RectangleShape(sf::Vector2f(GetArgumentRawWidth(), Global::BlockHeight));
	basic.setFillColor(sf::Color::Magenta);
	basic.setPosition(m_relitivePosition.x, m_relitivePosition.y);

	render->draw(basic);
}

void Argument::FrameUpdate()
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

void Argument::SetMode(BlockArgumentVariableMode mode)
{

}

void Argument::Select()
{

}

void Argument::ReInspectData()
{

}

std::string* Argument::GetData()
{
	return nullptr;
}

std::string* Argument::GetDataRaw()
{
	return nullptr;
}

BlockArgumentVariableMode* Argument::GetMode()
{
	return nullptr;
}

void Argument::Update(bool global)
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

	FrameUpdate();
	UpdateTexture();
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

void Argument::UpdateTexture()
{
	(*m_functionUpdatePreTexture)();
}

void Argument::SelectGlobaly()
{
	if (Global::SelectedArgument != nullptr)
	{
		Argument* gArg = (Argument*)Global::SelectedArgument;
		Global::SelectedArgument = (void*)this;
		gArg->FrameUpdate();
		gArg->UpdateTexture();
	}
	else
		Global::SelectedArgument = (void*)this;

	(*m_functionSelect)();
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
