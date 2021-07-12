#pragma once
#include "config.h"

#include "stacking/args/Argument.hpp"
#include "ui/interfaces/IRenderable.hpp"
#include "ui/interfaces/IUpdatable.hpp"
#include "ui/interfaces/IPositionable.hpp"
#include "ui/interfaces/ISizeable.hpp"
#include "ui/RenderRectangle.hpp"

#include <Cappuccino/Utils.hpp>
#include <Cappuccino/block/ModBlock.hpp>
#include <Espresso/catagory/ModCatagory.hpp>

class Block : public IRenderable, public IUpdatable, public IPositionable<2, int32_t>, public ISizeable<int32_t>
{
public:
	Block(const std::string& unlocalizedName);
	//Block(const Block& block);

	~Block();

	// would not recommend calling these

	void AddArgument(std::shared_ptr<Argument> argument);
	void AddArguments(const std::vector<std::shared_ptr<Argument>>& arguments);

	const std::vector<std::shared_ptr<Argument>>& GetArguments();
	uint32_t GetWidth();

	const ModBlock* GetModBlock();
	const ModCatagory* GetModCategory();
	void FrameUpdate(double deltaTime) override;

	static int16_t Padding;
	static int16_t Height;
	static int16_t HeightContent;
private:
	void OnRender() override;
	void OnUpdateBuffers() override;
	void OnReloadSwapChain() override;
	void PostPositionUpdate() override;

	void UpdateArguments();
	void UpdateWidth();

	uint32_t m_width;

	std::vector<std::shared_ptr<Argument>> m_arguments;
	const ModBlock* m_modBlock;
	const ModCatagory* m_modCategory;
	std::shared_ptr<vui::RenderRectangle> m_Background;
};