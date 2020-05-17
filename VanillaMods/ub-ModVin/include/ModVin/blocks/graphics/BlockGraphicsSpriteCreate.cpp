#include "BlockGraphicsSpriteCreate.h"
#include "../system/BlockSystemCreateWindow.h"

#include <SFML/Graphics.hpp>

static void ExecuteRelease(ModBlockPass* pass)
{
	WindowHandler* handler = (WindowHandler*)pass->CustomGet(*pass->GetReal(1));
	sf::Sprite* sprite = new sf::Sprite();
	
	handler->AddRenderable(sprite, pass->GetReal(0));
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockGraphicsSpriteCreate::GetUnlocalizedName() const
{
	return "vin_graphics_sprite_create";
}

const char* BlockGraphicsSpriteCreate::GetCategory() const
{
	return CATEGORY_GRAPHICS;
}

blockExecution BlockGraphicsSpriteCreate::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockGraphicsSpriteCreate::PullExecuteRelease() const
{
	return ExecuteRelease;
}

const std::vector<BlockArgumentInitializer> BlockGraphicsSpriteCreate::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "sprite create"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "sprite"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "in"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "window"));

	return args;
}
