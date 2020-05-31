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
		std::function<void(const uint8_t&)>* callback);

	static void Disable();
private:
	static std::atomic<bool> m_enabled;

	static std::vector<std::function<void()>*> m_buttonCallbacks;
	static std::function<void(const uint8_t&)>* m_contextCallback;
	static std::vector<ButtonText*> m_buttons;
};