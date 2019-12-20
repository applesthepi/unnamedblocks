#pragma once
#include "ui/Button.h"

#include <vector>

class ButtonRegistry
{
public:
	static void Initialize();
	static void FrameUpdateUI(sf::RenderWindow* window);
	static void RenderUI(sf::RenderWindow* window);
	static bool MouseUpdateButtons(sf::Mouse::Button button, bool down);

	static void AddButton(Button* button);
	static void RemoveButton(Button* button);

	static void Push();
	static void Pop();
private:
	static std::vector<Button*>* m_buttons;
	static std::vector<std::vector<Button*>*> m_stashedButtons;
};