#pragma once
#include "config.h"

#include "registries/BlockRegistry.hpp"
#include "stacking/Plane.hpp"
#include "ui/RenderButton.hpp"
#include "ui/RenderFrame.hpp"

#include <Cappuccino/Utils.hpp>

static class CategoryHandler
{
public:
	static void Populate(std::shared_ptr<vui::RenderFrame>& renderFrame);
	static void Render();
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
	};

	static std::vector<ModGroup> m_ModGroups;
	static std::shared_ptr<vui::RenderFrame> m_RenderFrame;
};
