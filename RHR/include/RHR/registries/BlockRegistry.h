#pragma once
#include "handlers/runtime/VariableHandler.h"
#include "ArgumentEssentials.h"

#include <vector>
#include <string>
#include <SFML/Graphics/Color.hpp>
#include <functional>

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

class RegBlock
{
public:
	std::function<bool(const std::vector<BlockArgumentCaller>&, const uint64_t&)>* Execute;
	std::vector<BlockArgument> Args;
	std::string UnlocalizedName;
	std::string Catagory;

	//carry
	std::vector<BlockArgumentInitializer>* BlockInit;
	std::function<bool(const std::vector<std::string>&)>* BlockExecute;
	std::function<bool(const std::vector<std::string>&, const uint64_t&)>* BlockExecuteIdx;
};

class RegCatagory
{
public:
	RegCatagory(const std::string unlocalizedName, const std::string displayName, const sf::Color color)
		:UnlocalizedName(unlocalizedName), DisplayName(displayName), Color(color) {}

	std::string UnlocalizedName;
	std::string DisplayName;
	sf::Color Color;
};

class BlockRegistry
{
public:
	BlockRegistry();

	void RegisterCatagory(RegCatagory* catagory);
	void RegisterBlock(RegBlock* block, VariableHandler* variables);

	const RegBlock* GetBlock(std::string unlocalizedName);
	const RegCatagory* GetCatagory(std::string unlocalizedName);

	std::vector<RegBlock>* GetBlocks();
	std::vector<RegCatagory>* GetCatagories();

	RegBlock* CreateBlock(const std::string unlocalizedName, const std::string catagory, std::function<bool(const std::vector<std::string>&)>* execute, const std::vector<BlockArgumentInitializer> blockInit);
	RegBlock* CreateBlock(const std::string unlocalizedName, const std::string catagory, std::function<bool(const std::vector<std::string>&, const uint64_t&)>* execute, const std::vector<BlockArgumentInitializer> blockInit);

	void FinalizeBlock(RegBlock* block, VariableHandler* variables);
private:
	std::vector<RegBlock>* m_blocks;
	std::vector<RegCatagory>* m_catagories;
};