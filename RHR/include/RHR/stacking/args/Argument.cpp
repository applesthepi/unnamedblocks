#include "Argument.h"

#include <Espresso/Global.h>
#include <Cappuccino/Logger.h>

Argument::Argument(const sf::Vector2u& relitivePosition)
	:m_relitivePosition(relitivePosition), m_functionUpdatePreTexture(nullptr), m_functionSelect(nullptr), m_next(false) {}

void Argument::SetupInBlock(sf::Vector2i* blockRelitive, sf::Vector2i* blockAbsolute, std::function<void()>* functionUpdatePreTexture, std::function<void()>* functionSelect)
{
	m_blockRelitive = blockRelitive;
	m_blockAbsolute = blockAbsolute;
	m_functionUpdatePreTexture = functionUpdatePreTexture;
	m_functionSelect = functionSelect;

	frameUpdate(0.0);
}

unsigned int Argument::GetArgumentRawWidth()
{
	return 0;
}

const bool Argument::HasData()
{
	return false;
}

void Argument::SetData(const std::string&)
{

}

void Argument::SetMode(const BlockArgumentVariableMode&)
{

}

void Argument::Select()
{

}

void Argument::ReInspectData()
{

}

const std::string& Argument::GetData()
{
	return nullptr;
}

const std::string& Argument::GetDataRaw()
{
	return nullptr;
}

const BlockArgumentVariableMode Argument::GetMode()
{
	return BlockArgumentVariableMode::RAW;
}

const BlockArgumentType Argument::GetType()
{
	return BlockArgumentType::TEXT;
}

void Argument::preFrameUpdate(const bool& global)
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
}

void Argument::frameUpdate(const double& deltaTime)
{
	frameUpdateArgument(deltaTime);
	UpdateTexture();
}

void Argument::SetRelitivePosition(const sf::Vector2u& relitivePosition)
{
	m_relitivePosition = relitivePosition;
}

const bool Argument::PullNext()
{
	if (m_next)
	{
		m_next = false;
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
		gArg->frameUpdate(0.0);
		gArg->UpdateTexture();
	}
	else
	{
		Global::SelectedArgument = (void*)this;
	}

	(*m_functionSelect)();
}

const sf::Vector2i& Argument::GetAbsolutePosition()
{
	return m_absolutePosition;
}

const sf::Vector2i& Argument::GetRealAbsolutePosition()
{
	return m_realAbsolutePosition;
}

const sf::Vector2u& Argument::GetRelitivePosition()
{
	return m_relitivePosition;
}

void Argument::frameUpdateArgument(const double& deltaTime)
{

}
