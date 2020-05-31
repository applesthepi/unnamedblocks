#pragma once
#include "RHR/ui/ButtonText.h"

#include <vector>
#include <functional>
#include <atomic>

#define CONTEXT_COLOR 180, 180, 180, 200

class ContextHandler
{
public:
	static const std::atomic<bool>& IsEnabled();

	static void Enable(const sf::Vector2f& position,
		std::function<void(const uint8_t&)>* c0,
		std::function<void(const uint8_t&)>* c1,
		std::function<void(const uint8_t&)>* c2,
		std::function<void(const uint8_t&)>* c3);

	static void Disable();
private:
	static std::atomic<bool> m_enabled;

	static std::vector<std::function<void()>*> m_buttonCallbacks;
	static std::vector<std::function<void(const uint8_t&)>*> m_contextCallbacks;
	static std::vector<ButtonText*> m_buttons;
};