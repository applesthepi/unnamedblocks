#pragma once
#include "Button.h"
#include "RHR/registries/ButtonRegistry.h"

#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>

class Dropdown : public sf::Drawable, public sf::Transformable
{
public:
	Dropdown(const sf::Vector2i& position);

	void FrameUpdate();
	void SetEnabled(const bool& enabled);
	void SetOpen(const bool& open);
	void Select(const uint64_t& idx);
	void Toggle();
	void Reload();
	void SetElements(const std::vector<std::string>& elements, const uint64_t& idx);

	const uint16_t GetSelected();
	const uint64_t GetWidth();

	const bool MouseButton(const bool& down, const sf::Vector2i& position, const sf::Mouse::Button& button);
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates state) const override;
private:
	bool m_enabled;
	bool m_open;
	uint16_t m_selected;
	const sf::Vector2i& m_position;
	uint32_t m_width;

	std::vector<Button> m_buttons;
	std::vector<std::string> m_elements;
	std::vector<std::function<void()>> m_buttonCallbacks;
};