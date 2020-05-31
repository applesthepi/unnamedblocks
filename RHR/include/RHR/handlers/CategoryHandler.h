#pragma once
#include "RHR/ui/ButtonText.h"
#include "RHR/ui/ButtonImage.h"
#include "RHR/ui/IRenderable.h"
#include "RHR/registries/BlockRegistry.h"
#include "RHR/stacking/Plane.h"

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

class CategoryHandler : public IRenderable
{
public:
	CategoryHandler();

	void ToggleMod(const uint16_t& modIdx, const uint64_t& catIdx);
	void UpdateBlocks(const uint64_t& catIdx);
	void RegisterHeader();
	const uint32_t UpdateButtons();
	const uint16_t& GetToolbarWidth();

	void frameUpdate(const double& deltaTime) override;

	static CategoryHandler& GetHandler();
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	static CategoryHandler m_handler;

	std::vector<ButtonText*> m_modCategoryButtons;

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