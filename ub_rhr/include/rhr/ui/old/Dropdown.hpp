#pragma once
#include "ButtonText.h"
#include "IUI.h"

#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>

class Dropdown : public IUI, public sf::Transformable
{
public:
	Dropdown(const sf::Vector2i& position);

	void SetEnabled(bool enabled);
	void SetOpen(bool open);
	void SetElements(const std::vector<std::string>& elements, uint64_t idx);

	void Select(uint64_t idx);
	void Toggle();
	void Reload();

	uint16_t GetSelected();
	uint64_t GetWidth();

	void frameUpdate(double deltaTime) override;
	bool mouseButton(bool down, const sf::Vector2i& position, const sf::Mouse::Button& button) override;
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates state) const override;
private:
	bool m_enabled;
	bool m_open;
	uint16_t m_selected;
	const sf::Vector2i& m_position;
	uint32_t m_width;

	std::vector<ButtonText> m_buttons;
	std::vector<std::string> m_elements;
	std::vector<std::function<void()>> m_buttonCallbacks;
};
