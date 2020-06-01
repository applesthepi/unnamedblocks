#pragma once
#include "RHR/ui/IUI.h"

#include <vector>
#include <SFML/Graphics.hpp>

class UIRegistry : public IUI
{
public:
	void AddComponent(IUI* ui);
	void RemoveComponent(IUI* ui);

	static UIRegistry& GetRegistry();

	void frameUpdate(double deltaTime);
	const bool mouseButton(bool down, const sf::Vector2i& position, const sf::Mouse::Button& button);
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	static UIRegistry m_registry;

	std::vector<IUI*> m_ui;
};