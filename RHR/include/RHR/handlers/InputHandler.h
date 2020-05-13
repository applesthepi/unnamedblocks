#pragma once

#include "config.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Text.hpp>
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
	// Fire a text input event
	static void FireTextEvent(sf::Event::TextEvent event);
	// Fire a key press event
	static void FireKeyEvent(sf::Event::KeyEvent event);

	// abstracted inputs
	static void RunTextProccess(std::string* text, uint64_t* locHigh, uint64_t* loc, std::function<void()>* enter, std::function<void()>* escape, const sf::Event::KeyEvent& ev);
	static void RunNumberProccess(std::string* text, uint64_t* locHigh, uint64_t* loc, std::function<void()>* enter, std::function<void()>* escape, const sf::Event::KeyEvent& ev);
	// run once per mouse update events on window
	static bool RunMouseProccess(sf::Text* text, uint64_t* locHigh, uint64_t* loc, bool* isDown, const bool& down, const sf::Vector2i& pos, const uint64_t& yOverride = 0);
	static bool RunMouseProccess(std::string* text, const sf::Vector2i& tPos, const sf::Vector2u& tSize, uint64_t* locHigh, uint64_t* loc, bool* isDown, const bool& down, const sf::Vector2i& pos, const uint16_t& fontSize);
	// run every frame
	static void RunMouseProccessFrame(sf::Text* text, uint64_t* loc, bool* isDown, const sf::Vector2i& pos, const uint64_t& yOverride = 0);
	static void RunMouseProccessFrame(std::string* text, const sf::Vector2i& tPos, const sf::Vector2u& tSize, uint64_t* loc, bool* isDown, const sf::Vector2i& pos, const uint16_t& fontSize);
private:
	static std::vector<std::function<void(const sf::Event::TextEvent&)>*>* text_callbacks;
	static std::vector<std::function<void(const sf::Event::KeyEvent&)>*>* key_callbacks;
};
