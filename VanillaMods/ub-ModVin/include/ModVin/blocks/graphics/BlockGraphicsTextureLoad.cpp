#include "BlockGraphicsTextureLoad.h"

#include <SFML/Graphics.hpp>

static void ExecuteRelease(ModBlockPass* pass)
{
	sf::Texture* texture = new sf::Texture();
	texture->loadFromFile(pass->GetString(0));

	pass->GetReal(1) = pass->CustomPut(texture);
}

static void ExecuteDebug(ModBlockPass* pass)
{
	sf::Texture* texture = new sf::Texture();
	if (!texture->loadFromFile(pass->GetString(0)))
	{
		pass->LogError("failed to load texture from path \"" + pass->GetString(0) + "\"; loading test texture instead", LoggerFatality::BREAK);
		
		texture->loadFromFile("res/test.png");
		pass->GetReal(1) = pass->CustomPut(texture);
		return;
	}

	pass->GetReal(1) = pass->CustomPut(texture);
}

const char* BlockGraphicsTextureLoad::GetUnlocalizedName() const
{
	return "vin_graphics_texture_load";
}

const char* BlockGraphicsTextureLoad::GetCategory() const
{
	return CATEGORY_GRAPHICS;
}

blockExecution BlockGraphicsTextureLoad::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockGraphicsTextureLoad::PullExecuteRelease() const
{
	return ExecuteRelease;
}

const std::vector<BlockArgumentInitializer> BlockGraphicsTextureLoad::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "texture load"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "res/"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "texture"));

	return args;
}
