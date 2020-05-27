#pragma once
#include "Button.h"
#include "RHR/registries/ButtonRegistry.h"

#include <SFML/Window/Mouse.hpp>
#include <vector>
#include <functional>

class Dropdown
{
public:
	Dropdown(const std::vector<std::string>& elements, const uint64_t& idx, const sf::Vector2i& position);

	void Render(sf::RenderTexture* render);
	void SetEnabled(const bool& enabled);
	void Select(const uint64_t& idx);
	void Reload();

	const uint16_t GetSelected();
	const uint64_t GetWidth();
private:
	bool m_enabled;
	bool m_open;
	uint16_t m_selected;
	const sf::Vector2i& m_position;

	std::vector<Button> m_buttons;
	std::vector<std::function<void()>> m_buttonCallbacks;
};