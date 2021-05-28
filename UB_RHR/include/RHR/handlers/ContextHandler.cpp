#include "ContextHandler.h"
#include "RHR/registries/UIRegistry.h"

const std::atomic<bool>& ContextHandler::IsEnabled()
{
	return m_enabled;
}

void ContextHandler::Enable(const sf::Vector2f& position, std::function<void(uint8_t)>* callback)
{
	if (m_enabled)
		return;

	m_enabled = true;

	m_buttonCallbacks.clear();
	m_buttons.clear();

	m_contextCallback = nullptr;

	// context callback is from external classes that is called with the index of the context

	m_contextCallback = callback;

	// button callback

	m_buttonCallbacks.push_back(new std::function<void()>());
	*m_buttonCallbacks.back() = [callback]() { (*callback)(0); };
	
	// button

	m_buttons.push_back(new ButtonText(m_buttonCallbacks.back(), "stack duplicate", 12, sf::Vector2f(300, 16), sf::Color(CONTEXT_COLOR), sf::Color::Black));
	UIRegistry::GetRegistry().AddComponent((IUI*)m_buttons.back());

	// button callback

	m_buttonCallbacks.push_back(new std::function<void()>());
	*m_buttonCallbacks.back() = [callback]() { (*callback)(1); };

	// button

	m_buttons.push_back(new ButtonText(m_buttonCallbacks.back(), "stack delete", 12, sf::Vector2f(300, 16), sf::Color(CONTEXT_COLOR), sf::Color::Black));
	UIRegistry::GetRegistry().AddComponent((IUI*)m_buttons.back());

	// button callback

	m_buttonCallbacks.push_back(new std::function<void()>());
	*m_buttonCallbacks.back() = [callback]() { (*callback)(2); };

	// button

	m_buttons.push_back(new ButtonText(m_buttonCallbacks.back(), "block duplicate", 12, sf::Vector2f(300, 16), sf::Color(CONTEXT_COLOR), sf::Color::Black));
	UIRegistry::GetRegistry().AddComponent((IUI*)m_buttons.back());

	// button callback

	m_buttonCallbacks.push_back(new std::function<void()>());
	*m_buttonCallbacks.back() = [callback]() { (*callback)(3); };

	// button

	m_buttons.push_back(new ButtonText(m_buttonCallbacks.back(), "block delete", 12, sf::Vector2f(300, 16), sf::Color(CONTEXT_COLOR), sf::Color::Black));
	UIRegistry::GetRegistry().AddComponent((IUI*)m_buttons.back());
}

void ContextHandler::Disable()
{
	if (!m_enabled)
		return;

	m_enabled = false;

	for (uint8_t i = 0; i < m_buttons.size(); i++)
	{
		UIRegistry::GetRegistry().RemoveComponent(m_buttons[i]);

		delete m_buttons[i];
		delete m_buttonCallbacks[i];
	}

	m_buttonCallbacks.clear();
	m_buttons.clear();

	// dont deallocate this
	m_contextCallback = nullptr;
}

std::atomic<bool> ContextHandler::m_enabled;

std::vector<std::function<void()>*> ContextHandler::m_buttonCallbacks;

std::function<void(uint8_t)>* ContextHandler::m_contextCallback;

std::vector<ButtonText*> ContextHandler::m_buttons;
