#include "InputHandler.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <chrono>
#include <iostream>
#include <GL/glew.h>

std::vector<std::function<void(const sf::Event::TextEvent&)>*>* InputHandler::text_callbacks;
std::vector<std::function<void(const sf::Event::KeyEvent&)>*>* InputHandler::key_callbacks;

void InputHandler::Initialization()
{
	text_callbacks = new std::vector<std::function<void(const sf::Event::TextEvent&)>*>();
	key_callbacks = new std::vector<std::function<void(const sf::Event::KeyEvent&)>*>();
}

void InputHandler::FireTextEvent(sf::Event::TextEvent event) {
	for(std::function<void(const sf::Event::TextEvent&)>* i : *InputHandler::text_callbacks) {
		(*i)(event);
	}
}

void InputHandler::FireKeyEvent(sf::Event::KeyEvent event) {
	for(std::function<void(const sf::Event::KeyEvent&)>* i : *InputHandler::key_callbacks) {
		(*i)(event);
	}
}

void InputHandler::RegisterTextCallback(std::function<void(const sf::Event::TextEvent&)>* fun)
{
	text_callbacks->push_back(fun);
}

void InputHandler::RegisterKeyCallback(std::function<void(const sf::Event::KeyEvent&)>* fun)
{
	key_callbacks->push_back(fun);
}

void InputHandler::UnregisterTextCallback(std::function<void(const sf::Event::TextEvent&)>* fun)
{
	for (uint64_t i = 0; i < text_callbacks->size(); i++)
	{
		if ((*text_callbacks)[i] == fun)
		{
			text_callbacks->erase(text_callbacks->begin() + static_cast<int32_t>(i));
			return;
		}
	}
}

void InputHandler::UnregisterKeyCallback(std::function<void(const sf::Event::KeyEvent&)>* fun)
{
	for (uint64_t i = 0; i < text_callbacks->size(); i++)
	{
		if ((*key_callbacks)[i] == fun)
		{
			key_callbacks->erase(key_callbacks->begin() + static_cast<int32_t>(i));
			return;
		}
	}
}
