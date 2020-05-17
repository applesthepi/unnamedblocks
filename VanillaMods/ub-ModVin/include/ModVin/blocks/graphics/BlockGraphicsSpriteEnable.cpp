#include "BlockGraphicsSpriteEnable.h"
#include "../system/BlockSystemCreateWindow.h"

#include <SFML/Graphics.hpp>

static void ExecuteRelease(ModBlockPass* pass)
{
	WindowHandler* handler = (WindowHandler*)pass->CustomGet(*pass->GetReal(1));
	double* idx = pass->GetReal(0);

	handler->SetRenderableEnabled(*idx, true);
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockGraphicsSpriteEnable::GetUnlocalizedName() const
{
	return "vin_graphics_sprite_enable";
}

const char* BlockGraphicsSpriteEnable::GetCategory() const
{
	return CATEGORY_GRAPHICS;
}

blockExecution BlockGraphicsSpriteEnable::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockGraphicsSpriteEnable::PullExecuteRelease() const
{
	return ExecuteRelease;
}

const std::vector<BlockArgumentInitializer> BlockGraphicsSpriteEnable::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "sprite enable"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "sprite"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "in"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "window"));

	return args;
}
