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

struct BlockInfo
{
	ModBlock* BlockModBlock;
	std::string BlockModUnlocalizedName;
};

struct CatagoryInfo
{
	ModCatagory* CatagoryModCatagory;
	std::string CatagoryModUnlocalizedName;
};

class BlockRegistry
{
public:
	void RegisterBlock(ModBlock* block, const std::string& modUnlocalizedName);
	void RegisterCatagory(ModCatagory* catagory, const std::string& modUnlocalizedName);

	const BlockInfo* GetBlock(const std::string& unlocalizedName);
	const CatagoryInfo* GetCategory(const std::string& unlocalizedName);

	const std::vector<BlockInfo>& GetBlocks();
	const std::vector<CatagoryInfo>& GetCategories();

	//const std::string& GetBlockMod(const ModBlock* modBlock);

	static void CreateBlockRegistry();
	static BlockRegistry& GetRegistry();
private:
	static BlockRegistry* m_Registry;

	std::vector<BlockInfo> m_Blocks;
	std::vector<CatagoryInfo> m_Catagories;
};
