#pragma once
#include "config.h"

#include <Cappuccino/Utils.hpp>

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

enum class MouseOperation
{
	HoverOn,
	HoverOff,
	Click,
	Press,
	Release,
	Move,
	Scroll
};

class InputHandler
{
public:
	static void Initialization();

	static void RegisterKeyCallback(void(*callback)(int16_t key, bool down, bool shift));
	static void UnregisterKeyCallback(void(*callback)(int16_t key, bool down, bool shift));

	static void RegisterTextCallback(void(*callback)(int16_t key, bool shift));
	static void UnregisterTextCallback(void(*callback)(int16_t key, bool shift));

	static void RegisterMouseCallback(void(*callback)(glm::vec<2, int32_t> position, float scroll, MouseOperation operation));
	static void UnregisterMouseCallback(void(*callback)(glm::vec<2, int32_t> position, float scroll, MouseOperation operation));

	// run events though window
	static void FireKey(int16_t key, uint8_t operation);
	static void FireMouseButton(uint8_t button, uint8_t operation);
	static void FireMouseMove(glm::vec<2, int32_t> position);
	static void FireMouseScroll(float scroll);
	static glm::vec<2, int32_t> GetMousePosition();

	// abstracted inputs
	// static void RunTextProccess(std::string* text, uint64_t* locHigh, uint64_t* loc, std::function<void()>* enter, std::function<void()>* escape, const sf::Event::KeyEvent& ev);
	// static void RunNumberProccess(std::string* text, uint64_t* locHigh, uint64_t* loc, std::function<void()>* enter, std::function<void()>* escape, const sf::Event::KeyEvent& ev);

	// run once per mouse update events on window
	// static bool RunMouseProccess(sf::Text* text, uint64_t* locHigh, uint64_t* loc, bool* isDown, bool down, const sf::Vector2i& pos, uint64_t yOverride = 0);
	// static bool RunMouseProccess(TextSystem& system, const sf::Vector2i& tPos, const sf::Vector2u& tSize, bool down, const sf::Vector2i& pos, uint16_t fontSize, int16_t vanityOffset);

	// run every frame
	// static void RunMouseProccessFrame(sf::Text* text, uint64_t* loc, bool* isDown, const sf::Vector2i& pos, uint64_t yOverride = 0);
	// static void RunMouseProccessFrame(std::string* text, const sf::Vector2i& tPos, const sf::Vector2u& tSize, uint64_t* loc, bool* isDown, const sf::Vector2i& pos, uint16_t fontSize, int16_t vanityOffset);
private:
	static bool m_MouseWasDown;
	static bool m_ShiftDown;
	static glm::vec<2, int32_t> m_MousePosition;
	static std::shared_mutex m_KeyTextMutex;
	static std::vector<void(*)(int16_t key, bool down, bool shift)> m_KeyCallbacks;
	static std::vector<void(*)(int16_t key, bool shift)> m_TextCallbacks;
	static std::shared_mutex m_MouseMutex;
	static std::vector<void(*)(glm::vec<2, int32_t> position, float scroll, MouseOperation operation)> m_MouseCallbacks;
};
