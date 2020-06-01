#pragma once
#include "Argument.h"
#include "RHR/ui/Dropdown.h"

#include <Espresso/Global.h>
#include <Espresso/InputHandler.h>
#include <SFML/Window/Clipboard.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <Cappuccino/Logger.h>

class ArgumentDropdown : public Argument
{
public:
	ArgumentDropdown(const sf::Vector2u& relitivePosition)
		:Argument(relitivePosition)
	{
		
	}

	void ProcessData(const std::string& data)
	{
		m_elementText = data;

		std::vector<std::string> elements;
		std::string element;

		element.reserve(16);
		elements.reserve(4);

		for (uint64_t i = 0; i < data.length(); i++)
		{
			if (data[i] == ';')
				elements.push_back(element);
			else
				element += data[i];
		}

		m_dropdown.SetElements(elements, 0);
	}

	void FrameUpdate() override
	{
		
	}

	void Render(sf::RenderTexture* render) override
	{
		m_dropdown.Render(*render)
			sf::RenderWindow.
	}

	unsigned int GetArgumentRawWidth() override
	{
		return m_dropdown.GetWidth();
	}

	const bool MouseButton(bool down, const sf::Vector2i& position, const sf::Mouse::Button& button) override
	{
		if (!m_dropdown.MouseButton(down, position, button))
			return false;
		else
		{
			m_data = "0" + std::to_string(m_dropdown.GetSelected());
			m_rawData = std::to_string(m_dropdown.GetSelected());
		}
	}

	const bool HasData() override
	{
		return true;
	}

	void SetData(const std::string& data) override
	{
		ProcessData(data);
	}

	const std::string& GetData() override
	{
		return m_data;
	}

	const std::string& GetDataRaw() override
	{
		return m_rawData;
	}

	const BlockArgumentVariableMode GetMode() override
	{
		return BlockArgumentVariableMode::RAW;
	}

	void Select() override
	{
		SelectGlobaly();
		m_dropdown.SetEnabled(true);
	}

	void ReInspectData() override
	{
		ProcessData(m_elementText);
	}

	const BlockArgumentType GetType() override
	{
		return BlockArgumentType::STRING;
	}
private:
	//bool m_isDown;
	//bool m_selected;
	//bool m_fullSelect;

	bool m_isOpen;
	Dropdown m_dropdown;
	std::string m_data;
	std::string m_rawData;
	std::string m_elementText;

	//std::function<void(const sf::Event::KeyEvent&)> m_textCallback;
};
