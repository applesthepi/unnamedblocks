#pragma once

#include "config.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vector>
#include <functional>

class TextSystem
{
public:
	TextSystem(std::string* text, uint64_t* locHigh, uint64_t* loc, bool* isDown)
		:Text(text), LocHigh(locHigh), Loc(loc), IsDown(isDown) {}
	
	std::string* Text;
	uint64_t* LocHigh;
	uint64_t* Loc;
	bool* IsDown;
};

class InputHandler
{
public:
	static void Initialization();

	// any key event; gives sf::Key
	static void RegisterKeyCallback(std::function<void(const sf::Event::KeyEvent&)>* fun);
	static void UnregisterKeyCallback(std::function<void(const sf::Event::KeyEvent&)>* fun);
	
	// text events; gives ascii
	static void RegisterTextCallback(std::function<void(const sf::Event::TextEvent&)>* fun);
	static void UnregisterTextCallback(std::function<void(const sf::Event::TextEvent&)>* fun);

	// run events though window
	static void FireTextEvent(sf::Event::TextEvent event);
	static void FireKeyEvent(sf::Event::KeyEvent event);

	// abstracted inputs
	static void RunTextProccess(std::string* text, uint64_t* locHigh, uint64_t* loc, std::function<void()>* enter, std::function<void()>* escape, const sf::Event::KeyEvent& ev);
	static void RunNumberProccess(std::string* text, uint64_t* locHigh, uint64_t* loc, std::function<void()>* enter, std::function<void()>* escape, const sf::Event::KeyEvent& ev);
	
	// run once per mouse update events on window
	static bool RunMouseProccess(sf::Text* text, uint64_t* locHigh, uint64_t* loc, bool* isDown, bool down, const sf::Vector2i& pos, uint64_t yOverride = 0);
	static bool RunMouseProccess(TextSystem& system, const sf::Vector2i& tPos, const sf::Vector2u& tSize, bool down, const sf::Vector2i& pos, uint16_t fontSize, const int64_t& vanityOffset);
	
	// run every frame
	static void RunMouseProccessFrame(sf::Text* text, uint64_t* loc, bool* isDown, const sf::Vector2i& pos, uint64_t yOverride = 0);
	static void RunMouseProccessFrame(std::string* text, const sf::Vector2i& tPos, const sf::Vector2u& tSize, uint64_t* loc, bool* isDown, const sf::Vector2i& pos, uint16_t fontSize, const int64_t& vanityOffset);
private:
	static std::vector<std::function<void(const sf::Event::TextEvent&)>*>* m_textCallbacks;
	static std::vector<std::function<void(const sf::Event::KeyEvent&)>*>* m_keyCallbacks;
};
