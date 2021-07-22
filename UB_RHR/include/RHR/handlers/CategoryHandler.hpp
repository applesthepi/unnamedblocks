#pragma once
#include "config.h"

#include "registries/BlockRegistry.hpp"
#include "stacking/Plane.hpp"
#include "stacking/Collection.hpp"
#include "stacking/Stack.hpp"
#include "stacking/Block.hpp"
#include "ui/RenderButton.hpp"
#include "ui/RenderFrame.hpp"

#include <Cappuccino/Utils.hpp>

static class CatagoryHandler
{
public:
	struct ActiveCatagory
	{
		uint16_t ActiveModGroup;
		uint16_t ActiveModGroupCatagory;
	};

	static void Populate(std::shared_ptr<vui::RenderFrame>& renderFrame);
	static void Render();
	static void SelectCatagory(ActiveCatagory activeCatagory);
private:
	struct ModGroup
	{
		ModGroup(const Color& primaryColor, const Color& secondaryColor)
			: ModButton(std::make_shared<vui::RenderButton>(primaryColor, secondaryColor))
		{
			ModButton->SetWeak(ModButton);
		}

		std::shared_ptr<vui::RenderButton> ModButton;
		std::vector<std::shared_ptr<vui::RenderButton>> ModCategories;
		std::vector<std::vector<std::shared_ptr<Collection>>> ModCategoryCollections;
		std::vector<std::vector<std::shared_ptr<Stack>>> ModCategoryStacks;
		std::vector<std::vector<std::shared_ptr<Block>>> ModCategoryBlocks;
	};

	static std::vector<ModGroup> m_ModGroups;
	static std::shared_ptr<vui::RenderFrame> m_RenderFrame;
	static ActiveCatagory m_ActiveCatagory;
};
