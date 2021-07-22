#include "BlockGraphicsSpriteSetPosition.h"
#include "../system/BlockSystemCreateWindow.h"

#include <SFML/Graphics.hpp>

static void ExecuteRelease(ModBlockPass* pass)
{
	WindowHandler* handler = (WindowHandler*)pass->CustomGet(pass->GetReal(3));
	((sf::Sprite*)handler->GetRenderable(&pass->GetReal(0)))->setPosition(pass->GetReal(1), pass->GetReal(2));
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockGraphicsSpriteSetPosition::GetUnlocalizedName() const
{
	return "vin_graphics_sprite_set_position";
}

const char* BlockGraphicsSpriteSetPosition::GetCategory() const
{
	return CATEGORY_GRAPHICS;
}

blockExecution BlockGraphicsSpriteSetPosition::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockGraphicsSpriteSetPosition::PullExecuteRelease() const
{
	return ExecuteRelease;
}

const std::vector<BlockArgumentInitializer> BlockGraphicsSpriteSetPosition::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "set position of"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "sprite"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "to"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "in"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "window"));

	return args;
}
