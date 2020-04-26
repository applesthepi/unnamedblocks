#pragma once
#include "ModVin/ModCategories.h"
#include <Espresso/Espresso.h>

class BlockMain : public ModBlock
{
public:
	const char* GetUnlocalizedName() const override;
	const char* GetCategory() const override;
	const ModBlockFlags GetFlags() const override;

	const ModBlockActions RuntimePreInit() const override;
	const ModBlockActions RuntimeInit() const override;
	const ModBlockActions RuntimePostInit() const override;

	const std::vector<BlockArgumentInitializer> GetArguments() const override;
};