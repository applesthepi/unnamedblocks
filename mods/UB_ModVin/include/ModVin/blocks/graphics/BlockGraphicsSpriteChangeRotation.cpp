#include "BlockGraphicsSpriteChangeRotation.h"
#include "../system/BlockSystemCreateWindow.h"

#include <SFML/Graphics.hpp>

static void ExecuteRelease(ModBlockPass* pass)
{
	WindowHandler* handler = (WindowHandler*)pass->CustomGet(pass->GetReal(2));
	((sf::Sprite*)handler->GetRenderable(&pass->GetReal(0)))->rotate(pass->GetReal(1));
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockGraphicsSpriteChangeRotation::GetUnlocalizedName() const
{
	return "vin_graphics_sprite_change_rotation";
}

const char* BlockGraphicsSpriteChangeRotation::GetCategory() const
{
	return CATEGORY_GRAPHICS;
}

blockExecution BlockGraphicsSpriteChangeRotation::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockGraphicsSpriteChangeRotation::PullExecuteRelease() const
{
	return ExecuteRelease;
}

const std::vector<BlockArgumentInitializer> BlockGraphicsSpriteChangeRotation::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "change rotation of"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "sprite"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "by"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "in"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "window"));

	return args;
}
