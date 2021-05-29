#pragma once
#include "config.h"

#include <Cappuccino/Utils.hpp>

class UIRegistry : public IUI
{
public:
	void AddComponent(IUI* ui);
	void RemoveComponent(IUI* ui);

	static UIRegistry& GetRegistry();

	void frameUpdate(double deltaTime) override;
	bool mouseButton(bool down, const sf::Vector2i& position, const sf::Mouse::Button& button) override;
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	static UIRegistry m_registry;

	std::vector<IUI*> m_ui;
};
