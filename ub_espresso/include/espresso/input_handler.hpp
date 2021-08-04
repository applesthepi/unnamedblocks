#pragma once
#include "config.h"

#include <cappuccino/utils.hpp>

class TextSystem
{
public:
	TextSystem(std::string* text, u64* locHigh, u64* loc, bool* isDown)
		:Text(text), LocHigh(locHigh), Loc(loc), IsDown(isDown) {}

	std::string* Text;
	u64* LocHigh;
	u64* Loc;
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

	static void RegisterKeyCallback(void(*callback)(i16 key, bool down, bool shift, void* data), void* data);
	static void UnregisterKeyCallback(void(*callback)(i16 key, bool down, bool shift, void* data));

	static void RegisterTextCallback(void(*callback)(i16 key, bool shift, void* data), void* data);
	static void UnregisterTextCallback(void(*callback)(i16 key, bool shift, void* data));

	static void RegisterMouseCallback(void(*callback)(glm::vec<2, i32> position, f32 scroll, MouseOperation operation, void* data), void* data);
	static void UnregisterMouseCallback(void(*callback)(glm::vec<2, i32> position, f32 scroll, MouseOperation operation, void* data));

	static void RegisterBullishMouseCallback(bool(*callback)(glm::vec<2, i32> position, f32 scroll, MouseOperation operation, void* data), u8 layer, void* data);
	static void UnregisterBullishMouseCallback(bool(*callback)(glm::vec<2, i32> position, f32 scroll, MouseOperation operation, void* data), u8 layer);

	// run events though window
	static void FireKey(i16 key, u8 operation);
	static void FireMouseButton(u8 button, u8 operation);
	static void FireMouseMove(glm::vec<2, i32> position);
	static void FireMouseScroll(f32 scroll);
	static glm::vec<2, i32> GetMousePosition();

	// abstracted inputs
	// static void RunTextProccess(std::string* text, u64* locHigh, u64* loc, std::function<void()>* enter, std::function<void()>* escape, const sf::Event::KeyEvent& ev);
	// static void RunNumberProccess(std::string* text, u64* locHigh, u64* loc, std::function<void()>* enter, std::function<void()>* escape, const sf::Event::KeyEvent& ev);

	// run once per mouse update events on window
	// static bool RunMouseProccess(sf::Text* text, u64* locHigh, u64* loc, bool* isDown, bool down, const sf::Vector2i& pos, u64 yOverride = 0);
	// static bool RunMouseProccess(TextSystem& system, const sf::Vector2i& tPos, const sf::Vector2u& tSize, bool down, const sf::Vector2i& pos, u16 fontSize, i16 vanityOffset);

	// run every frame
	// static void RunMouseProccessFrame(sf::Text* text, u64* loc, bool* isDown, const sf::Vector2i& pos, u64 yOverride = 0);
	// static void RunMouseProccessFrame(std::string* text, const sf::Vector2i& tPos, const sf::Vector2u& tSize, u64* loc, bool* isDown, const sf::Vector2i& pos, u16 fontSize, i16 vanityOffset);

	static u8 BullishLayerPopups;
	static u8 BullishLayerUI;
	static u8 BullishLayerArguments;
	static u8 BullishLayerBackground;
private:
	static bool m_MouseWasDown;
	static bool m_ShiftDown;
	static glm::vec<2, i32> m_MousePosition;
	static std::shared_mutex m_KeyTextMutex;
	static std::shared_mutex m_MouseMutex;

	static std::vector<void(*)(i16 key, bool down, bool shift, void* data)> m_KeyCallbacks;
	static std::vector<void*> m_KeyDatas;
	static std::vector<void(*)(i16 key, bool shift, void* data)> m_TextCallbacks;
	static std::vector<void*> m_TextDatas;
	static std::vector<void(*)(glm::vec<2, i32> position, f32 scroll, MouseOperation operation, void* data)> m_MouseCallbacks;
	static std::vector<void*> m_MouseDatas;
	static std::vector<std::vector<bool(*)(glm::vec<2, i32> position, f32 scroll, MouseOperation operation, void* data)>> m_BullishMouseCallbacks;
	static std::vector<std::vector<void*>> m_BullishMouseDatas;
};
