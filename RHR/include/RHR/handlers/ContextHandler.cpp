#include "ContextHandler.h"
#include "RHR/registries/UIRegistry.h"

const std::atomic<bool>& ContextHandler::IsEnabled()
{
	return m_enabled;
}

void ContextHandler::Enable(const sf::Vector2f& position, std::function<void(const uint8_t&)>* c0,
	std::function<void(const uint8_t&)>* c1,
	std::function<void(const uint8_t&)>* c2,
	std::function<void(const uint8_t&)>* c3)
{
	if (m_enabled)
		return;

	m_enabled = true;

	m_buttonCallbacks.clear();
	m_contextCallbacks.clear();
	m_buttons.clear();

	// context callbacks are from external classes that are called with the index of the context

	m_contextCallbacks.push_back(c0);
	m_contextCallbacks.push_back(c1);
	m_contextCallbacks.push_back(c2);
	m_contextCallbacks.push_back(c3);

	// make button callbacks and buttons

	std::vector<std::function<void(const uint8_t&)>*>* contextPointer = &m_contextCallbacks;

	// button callback

	m_buttonCallbacks.push_back(new std::function<void()>());
	*m_buttonCallbacks.back() = [contextPointer]() { (*(contextPointer->at(0)))(0); };
	
	// button

	m_buttons.push_back(new ButtonText(m_buttonCallbacks.back(), "stack duplicate", 12, sf::Vector2f(300, 16), sf::Color(CONTEXT_COLOR), sf::Color::Black));
	UIRegistry::GetRegistry().AddComponent((IUI*)m_buttons.back());

	// button callback

	m_buttonCallbacks.push_back(new std::function<void()>());
	*m_buttonCallbacks.back() = [contextPointer]() { (*(contextPointer->at(1)))(1); };

	// button

	m_buttons.push_back(new ButtonText(m_buttonCallbacks.back(), "stack delete", 12, sf::Vector2f(300, 16), sf::Color(CONTEXT_COLOR), sf::Color::Black));
	UIRegistry::GetRegistry().AddComponent((IUI*)m_buttons.back());

	// button callback

	m_buttonCallbacks.push_back(new std::function<void()>());
	*m_buttonCallbacks.back() = [contextPointer]() { (*(contextPointer->at(2)))(2); };

	// button

	m_buttons.push_back(new ButtonText(m_buttonCallbacks.back(), "block duplicate", 12, sf::Vector2f(300, 16), sf::Color(CONTEXT_COLOR), sf::Color::Black));
	UIRegistry::GetRegistry().AddComponent((IUI*)m_buttons.back());

	// button callback

	m_buttonCallbacks.push_back(new std::function<void()>());
	*m_buttonCallbacks.back() = [contextPointer]() { (*(contextPointer->at(3)))(3); };

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
		delete m_contextCallbacks[i];
	}

	m_buttonCallbacks.clear();
	m_contextCallbacks.clear();
	m_buttons.clear();
}

std::atomic<bool> ContextHandler::m_enabled;

std::vector<std::function<void()>*> ContextHandler::m_buttonCallbacks;

std::vector<std::function<void(const uint8_t&)>*> ContextHandler::m_contextCallbacks;

std::vector<ButtonText*> ContextHandler::m_buttons;
