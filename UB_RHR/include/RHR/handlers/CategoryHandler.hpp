#pragma once
#include "config.h"

#include "registries/BlockRegistry.hpp"
#include "stacking/Plane.hpp"

#include <Cappuccino/Utils.hpp>
/*
class CategoryHandler : public IRenderable
{
public:
	CategoryHandler();

	void ToggleMod(uint16_t modIdx, uint64_t catIdx);
	void UpdateBlocks(uint64_t catIdx);
	void RegisterHeader();
	uint32_t UpdateButtons();
	uint16_t GetToolbarWidth();

	void frameUpdate(double deltaTime) override;

	static void CreateHandler();
	static CategoryHandler& GetHandler();

	void PostRender(sf::RenderWindow* window);
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	static CategoryHandler* m_handler;

	std::vector<ButtonText*> m_modCategoryButtons;
	std::vector<std::vector<ButtonText*>> m_buttons;

	std::vector<std::function<void()>*> m_modCategoryCallbacks;
	std::vector<std::vector<std::function<void()>*>> m_buttonCallbacks;

	std::vector<bool> m_modOpen;
	std::vector<sf::Sprite> m_modIco;
	sf::Texture m_textureOpen;
	sf::Texture m_textureClose;

	sf::RectangleShape m_background;
	sf::RectangleShape m_backgroundOptions;
	uint64_t m_toolbarStackCount;
	uint64_t m_selectedCategory;

	bool m_running;
	bool m_fullBreak;
	bool m_needsUpdate;

	std::vector<ButtonImage*> m_editorButtons;
	std::vector<ButtonImage*> m_runtimeButtons;

	uint16_t m_toolbarWidth;
};
*/