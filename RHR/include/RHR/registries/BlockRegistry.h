#pragma once
#include <vector>
#include <string>
#include <SFML/Graphics/Color.hpp>
#include <functional>
#include <Espresso/block/ModBlock.h>
#include <Espresso/catagory/ModCatagory.h>
#include <Cappuccino/ExecutionThread.h>

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
	BlockRegistry();

	void RegisterCatagory(ModCatagory* catagory);
	void RegisterBlock(ModBlock* block);
	void RegisterExeDebug(void(*fun)(ModBlockPass*));
	void RegisterExeRelease(void(*fun)(ModBlockPass*));

	const ModBlock* GetBlock(const std::string& unlocalizedName);
	const ModCatagory* GetCategory(const std::string& unlocalizedName);

	executionFunction GetExeDebug(const std::string& blockUnlocalizedName);
	executionFunction GetExeRelease(const std::string& blockUnlocalizedName);

	const std::vector<ModBlock*>& GetBlocks();
	const std::vector<ModCatagory*>& GetCategories();
private:
	std::vector<ModBlock*> m_blocks;
	std::vector<ModCatagory*> m_catagories;
	std::vector<executionFunction> m_exeDebug;
	std::vector<executionFunction> m_exeRelease;
};
