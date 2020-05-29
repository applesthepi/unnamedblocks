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

#define ARGUMENT_DECOR_REACH 5

class ArgumentReal : public Argument
{
public:
	ArgumentReal(const sf::Vector2u& relitivePosition, const BlockArgumentVariableMode& mode, const bool& canSwitch)
		:Argument(relitivePosition), m_field("", canSwitch, FieldType::NUMBER)
	{
		m_leftTop = sf::ConvexShape(3);
		m_leftTop.setPoint(0, sf::Vector2f(0, 0));
		m_leftTop.setPoint(1, sf::Vector2f(ARGUMENT_DECOR_REACH, (Global::BlockHeight - Global::BlockBorder) / 2));
		m_leftTop.setPoint(2, sf::Vector2f(ARGUMENT_DECOR_REACH, 0));

		m_leftBottom = sf::ConvexShape(3);
		m_leftBottom.setPoint(0, sf::Vector2f(ARGUMENT_DECOR_REACH, (Global::BlockHeight - Global::BlockBorder) / 2));
		m_leftBottom.setPoint(1, sf::Vector2f(0, Global::BlockHeight - Global::BlockBorder));
		m_leftBottom.setPoint(2, sf::Vector2f(ARGUMENT_DECOR_REACH, Global::BlockHeight - Global::BlockBorder));

		m_rightTop = sf::ConvexShape(3);
		m_rightTop.setPoint(0, sf::Vector2f(0, 0));
		m_rightTop.setPoint(1, sf::Vector2f(ARGUMENT_DECOR_REACH, 0));
		m_rightTop.setPoint(2, sf::Vector2f(0, (Global::BlockHeight - Global::BlockBorder) / 2));

		m_rightBottom = sf::ConvexShape(3);
		m_rightBottom.setPoint(0, sf::Vector2f(0, (Global::BlockHeight - Global::BlockBorder) / 2));
		m_rightBottom.setPoint(1, sf::Vector2f(ARGUMENT_DECOR_REACH, Global::BlockHeight - Global::BlockBorder));
		m_rightBottom.setPoint(2, sf::Vector2f(0, Global::BlockHeight - Global::BlockBorder));

		m_tab = [this]()
		{
			m_next = true;
		};

		m_field.SetTab(&m_tab);
	}

	const BlockArgumentType GetType() override
	{
		return BlockArgumentType::REAL;
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

	void frameUpdateArgument(const double& deltaTime) override
	{
		m_leftTop.setPosition(GetRelitivePosition().x, GetRelitivePosition().y + (int)(Global::BlockBorder / 2));
		m_leftBottom.setPosition(GetRelitivePosition().x, GetRelitivePosition().y + (int)(Global::BlockBorder / 2));
		m_rightTop.setPosition(GetRelitivePosition().x + GetWidth() - ARGUMENT_DECOR_REACH, GetRelitivePosition().y + (int)(Global::BlockBorder / 2));
		m_rightBottom.setPosition(GetRelitivePosition().x + GetWidth() - ARGUMENT_DECOR_REACH, GetRelitivePosition().y + (int)(Global::BlockBorder / 2));

		m_field.frameUpdate(deltaTime);
	}
	
	const bool mouseButton(const bool& down, const sf::Vector2i& position, const sf::Mouse::Button& button) override
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
		target.draw(m_leftTop);
		target.draw(m_leftBottom);
		target.draw(m_rightTop);
		target.draw(m_rightBottom);

		target.draw(m_field);
	}
private:
	Field m_field;
	std::function<void()> m_tab;

	sf::ConvexShape m_leftTop;
	sf::ConvexShape m_leftBottom;
	sf::ConvexShape m_rightTop;
	sf::ConvexShape m_rightBottom;
};
