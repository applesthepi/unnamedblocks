#pragma once
#include "RHR/ui/Button.h"
#include "RHR/registries/BlockRegistry.h"
#include "RHR/stacking/Plane.h"

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

class CategoryHandler
{
public:
	static void Initialize(BlockRegistry* blockRegistry, Plane* toolbarPlane);
	static void Render(sf::RenderWindow* window, Plane* toolbarPlane);
	static void RenderPost(sf::RenderWindow* window);
	static void FrameUpdate(BlockRegistry* blockRegistry, Plane* toolbarPlane);

	static void ToggleMod(const uint16_t& modIdx, BlockRegistry* blockRegistry, Plane* toolbarPlane, const uint64_t& catIdx);
	static const uint32_t UpdateButtons();
	static void UpdateBlocks(BlockRegistry* blockRegistry, Plane* toolbarPlane, const uint64_t& catIdx);

	static void RegisterHeader(BlockRegistry* blockRegistry, Plane* primaryPlane);
private:
	static std::vector<Button*> m_modCategoryButtons;
	static std::vector<std::vector<Button*>> m_buttons;

	static std::vector<std::function<void()>*> m_modCategoryCallbacks;
	static std::vector<std::vector<std::function<void()>*>> m_buttonCallbacks;

	static std::vector<bool> m_modOpen;
	static std::vector<sf::Sprite> m_modIco;
	static sf::Texture m_textureOpen;
	static sf::Texture m_textureClose;

	static sf::RectangleShape m_background;
	static sf::RectangleShape m_backgroundOptions;
	static uint64_t m_toolbarStackCount;
	static uint64_t m_selectedCategory;

	static bool m_running;
	static bool m_fullBreak;

	static std::vector<Button*> m_editorButtons;
	static std::vector<Button*> m_runtimeButtons;
};