#include "UIRegistry.hpp"

// void UIRegistry::AddComponent(IUI* ui)
// {
// 	m_ui.push_back(ui);
// }

// void UIRegistry::RemoveComponent(IUI* ui)
// {
// 	for (uint64_t i = 0; i < m_ui.size(); i++)
// 	{
// 		if (m_ui[i] == ui)
// 		{
// 			m_ui.erase(m_ui.begin() + i);
// 			return;
// 		}
// 	}

// 	Logger::Error("failed to remove ui component from UIRegistry");
// }

// UIRegistry& UIRegistry::GetRegistry()
// {
// 	return m_registry;
// }

// void UIRegistry::frameUpdate(double deltaTime)
// {
// 	for (uint64_t i = 0; i < m_ui.size(); i++)
// 		m_ui[i]->frameUpdate(deltaTime);
// }

// bool UIRegistry::mouseButton(bool down, const sf::Vector2i& position, const sf::Mouse::Button& button)
// {
// 	for (uint64_t i = 0; i < m_ui.size(); i++)
// 	{
// 		if (m_ui[i]->mouseButton(down, position, button))
// 			return true;
// 	}

// 	return false;
// }

// void UIRegistry::draw(sf::RenderTarget& target, sf::RenderStates states) const
// {
// 	for (uint64_t i = 0; i < m_ui.size(); i++)
// 		target.draw(*m_ui[i]);
// }

// UIRegistry UIRegistry::m_registry;