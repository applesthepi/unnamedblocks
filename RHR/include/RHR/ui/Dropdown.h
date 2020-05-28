#pragma once
#include "ButtonText.h"
#include "RHR/registries/ButtonRegistry.h"
#include "UBRenderable.h"
#include "MouseUpdatable.h"

#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>

class Dropdown : public UBRenderable, public sf::Transformable, public MouseUpdatable
{
public:
	Dropdown(const sf::Vector2i& position);

	void SetEnabled(const bool& enabled);
	void SetOpen(const bool& open);
	void Select(const uint64_t& idx);
	void Toggle();
	void Reload();
	void SetElements(const std::vector<std::string>& elements, const uint64_t& idx);

	const uint16_t GetSelected();
	const uint64_t GetWidth();

	void frameUpdate(const double& deltaTime) override;
	const bool mouseButton(const bool& down, const sf::Vector2i& position, const sf::Mouse::Button& button) override;
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