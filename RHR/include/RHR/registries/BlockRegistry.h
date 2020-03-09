#pragma once
#include "ArgumentEssentials.h"

#include <vector>
#include <string>
#include <SFML/Graphics/Color.hpp>
#include <functional>
#include <Espresso/catagory/ModCatagory.h>
#include <Espresso/block/ModBlock.h>

class BlockArgumentInitializer
{
public:
	BlockArgumentInitializer(const BlockArgumentType type, const BlockArgumentVariableModeRestriction restriction, const BlockArgumentVariableMode mode, const std::string defaultValue)
		:Type(type), Restriction(restriction), Mode(mode), DefaultValue(defaultValue) {}

	BlockArgumentType Type;
	BlockArgumentVariableModeRestriction Restriction;
	BlockArgumentVariableMode Mode;
	std::string DefaultValue;
};

class BlockArgumentCaller
{
public:
	BlockArgumentCaller(const BlockArgumentVariableMode mode, const std::string value)
		:Mode(mode), Value(value) {}

	BlockArgumentVariableMode Mode;
	std::string Value;
};

class BlockArgument
{
public:
	void SetupTEXT(const std::string value);
	void SetupREAL(const std::string value);
	void SetupBOOL(const std::string value);
	void SetupSTRING(const std::string value);

	BlockArgumentType Type;
	std::string Value;
};

class BlockRegistry
{
public:
	BlockRegistry();

	void RegisterCatagory(const ModCatagory& catagory);
	void RegisterBlock(const ModBlock& block);

	const ModBlock& GetBlock(const std::string& unlocalizedName);
	const ModCatagory& GetCatagory(const std::string& unlocalizedName);
private:
	std::vector<ModBlock> m_blocks;
	std::vector<ModCatagory> m_catagories;
};
