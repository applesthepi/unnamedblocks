#pragma once
#include "Argument.h"
#include "RHR/ui/Field.h"

#include <Espresso/Global.h>
#include <Espresso/InputHandler.h>
#include <SFML/Window/Clipboard.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <Cappuccino/Logger.h>

class ArgumentString : public Argument
{
public:
	ArgumentString(const sf::Vector2u& relitivePosition, const BlockArgumentVariableMode& mode, bool canSwitch)
		:Argument(relitivePosition), m_field("", canSwitch, FieldType::TEXT)
	{
		m_tab = [this]()
		{
			m_next = true;
		};

		m_field.SetTab(&m_tab);
	}

	const BlockArgumentType GetType() override
	{
		return BlockArgumentType::STRING;
	}

	const uint32_t GetWidth() override
	{
		return m_field.GetWidth();
	}

	const bool HasData() override
	{
		return true;
	}

	void Select() override
	{
		SelectGlobaly();

		if (m_field.GetSelected())
			m_field.mouseButton(true, Global::MousePosition, sf::Mouse::Button::Left);
		else
		{
			m_field.SelectAll();
		}
	}

	void frameUpdateArgument(double deltaTime) override
	{
		m_field.frameUpdate(deltaTime);
	}

	const bool mouseButton(bool down, const sf::Vector2i& position, const sf::Mouse::Button& button) override
	{
		if (!m_field.mouseButton(down, position, button) && button == sf::Mouse::Left)
		{
			if (Global::SelectedArgument == this)
			{
				Global::SelectedStack = nullptr;
				Global::SelectedBlock = nullptr;
				Global::SelectedArgument = nullptr;
				Global::Dragging = false;

				(*m_functionUpdatePreTexture)();
			}
		}
	}

	void reloadData() override
	{
		m_field.SetText(m_data);
	}

	void draw(sf::RenderTarget& target, sf::RenderStates state) const override
	{
		target.draw(m_field);
	}
private:
	Field m_field;
	std::function<void()> m_tab;
};
