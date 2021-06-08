#pragma once
#include "config.h"

#include <Cappuccino/Utils.hpp>
#include <Cappuccino/block/ModBlock.hpp>
#include <Espresso/catagory/ModCatagory.hpp>
#include <Cappuccino/ExecutionThread.hpp>

class BlockArgumentCaller
{
public:
	BlockArgumentCaller(const BlockArgumentVariableMode mode, const std::string value)
		:Mode(mode), Value(value) {}

	BlockArgumentVariableMode Mode;
	std::string Value;
};

class BlockRegistry
{
public:
	void RegisterBlock(ModBlock* block, const std::string& mod);
	void RegisterCatagory(ModCatagory* catagory);

	const ModBlock* GetBlock(const std::string& unlocalizedName);
	const ModCatagory* GetCategory(const std::string& unlocalizedName);

	const std::vector<ModBlock*>& GetBlocks();
	const std::vector<ModCatagory*>& GetCategories();

	const std::string& GetBlockMod(const ModBlock* modBlock);

	static BlockRegistry& GetRegistry();
private:
	static BlockRegistry m_registry;

	std::vector<std::string> m_blockMods;
	std::vector<ModBlock*> m_blocks;
	std::vector<ModCatagory*> m_catagories;
};
