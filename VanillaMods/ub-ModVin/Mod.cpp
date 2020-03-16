#include "Mod.h"

#define CATEGORY_MAIN "test_category"

class SomeCategory : public ModCatagory
{
public:
	const sf::Color GetColor() const override
	{
		return sf::Color(255, 140, 0);
	}

	const std::string GetUnlocalizedName() const override
	{
		return CATEGORY_MAIN;
	}

	const std::string GetDisplayName() const override
	{
		return "Test Mod";
	}
};

class SomeBlock : public ModBlock
{
public:
	const char* GetUnlocalizedName() const override
	{
		return "test_print";
	}

	const char* GetPath() const override
	{
		return "TestMod/print.ubb";
	}

	const char* GetCategory() const override
	{
		return CATEGORY_MAIN;
	}

	const bool IsTopical() const override
	{
		return false;
	}

	const ModBlockFlags GetFlags() const override
	{
		return ModBlockFlags().CheckHeap().CheckStack();
	}

	const ModBlockActions RuntimePreInit() const override
	{
		ModBlockActions actions;
		actions.AddAction(ModBlockActionLogInfo("this should only run once; used for global containers"));

		return actions;
	}

	const ModBlockActions RuntimeInit() const override
	{
		ModBlockActions actions;
		actions.AddAction(ModBlockActionLogInfo("this should only run once for each stack; used for local containers"));

		return actions;
	}

	const ModBlockActions RuntimePostInit() const override
	{
		ModBlockActions actions;
		actions.AddAction(ModBlockActionLogInfo("this should run for every block that exists during post init; used for local containers"));

		return actions;
	}

	const std::vector<BlockArgumentInitializer> GetArguments() const override
	{
		std::vector<BlockArgumentInitializer> args;
		
		args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "example text"));
		args.push_back(BlockArgumentInitializer(BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "Hello, World!"));

		return args;
	}
};

UB_EXPORT void Initialization(ModData* data)
{
	data->RegisterCategory(new SomeCategory());
	data->RegisterBlock(new SomeBlock());
}