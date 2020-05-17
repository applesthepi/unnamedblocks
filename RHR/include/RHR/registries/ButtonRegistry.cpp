#include "ButtonRegistry.h"

#include <Espresso/Global.h>
#include <Cappuccino/Logger.h>

void ButtonRegistry::Initialize()
{
	m_buttons = new std::vector<Button*>();
}

void ButtonRegistry::FrameUpdateUI(sf::RenderWindow* window)
{
	for (unsigned int i = 0; i < m_buttons->size(); i++)
	{
		(*m_buttons)[i]->FrameUpdate(window);
	}
}

void ButtonRegistry::RenderUI(sf::RenderWindow* window)
{
	for (unsigned int i = 0; i < m_buttons->size(); i++)
	{
		(*m_buttons)[i]->Render(window);
	}
}

bool ButtonRegistry::MouseUpdateButtons(sf::Mouse::Button button, bool down)
{
	for (unsigned int i = 0; i < m_buttons->size(); i++)
	{
		if ((*m_buttons)[i]->MouseButton(down, Global::MousePosition, button))
			return true;
	}

	return false;
}

void ButtonRegistry::AddButton(Button* button)
{
	m_buttons->push_back(button);
}

void ButtonRegistry::RemoveButton(Button* button)
{
	for (unsigned int i = 0; i < m_buttons->size(); i++)
	{
		if ((*m_buttons)[i] == button)
		{
			m_buttons->erase(m_buttons->begin() + i);
			return;
		}
	}
}

void ButtonRegistry::Push()
{
	m_stashedButtons.push_back(m_buttons);
	m_buttons = new std::vector<Button*>();
}

void ButtonRegistry::Pop()
{
	if (m_stashedButtons.size() == 0)
	{
		Logger::Error("failed to pop stashed buttons!");
		return;
	}

	if (m_buttons->size() > 0)
		Logger::Warn("poping stashed buttons; the current button stack is not empty!");

	delete m_buttons;
	m_buttons = m_stashedButtons[0];
	m_stashedButtons.erase(m_stashedButtons.begin());
}

std::vector<Button*>* ButtonRegistry::m_buttons;

std::vector<std::vector<Button*>*> ButtonRegistry::m_stashedButtons;

