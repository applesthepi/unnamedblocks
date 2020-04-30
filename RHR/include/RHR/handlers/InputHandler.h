#pragma once

#include "config.h"

#include <SFML/Window/Event.hpp>
#include <vector>
#include <functional>

class InputHandler
{
public:
	static void Initialization();
	// Register a keyboard event callback
	static void RegisterKeyCallback(std::function<void(const sf::Event::KeyEvent&)>* fun);
	// Unregister a keyboard event callback
	static void UnregisterKeyCallback(std::function<void(const sf::Event::KeyEvent&)>* fun);
	// Register a text event callback
	static void RegisterTextCallback(std::function<void(const sf::Event::TextEvent&)>* fun);
	// Unregister a text event callback
	static void UnregisterTextCallback(std::function<void(const sf::Event::TextEvent&)>* fun);
	// Fire a text input event
	static void FireTextEvent(sf::Event::TextEvent event);
	// Fire a key press event
	static void FireKeyEvent(sf::Event::KeyEvent event);
private:
	static std::vector<std::function<void(const sf::Event::TextEvent&)>*>* text_callbacks;
	static std::vector<std::function<void(const sf::Event::KeyEvent&)>*>* key_callbacks;
};
