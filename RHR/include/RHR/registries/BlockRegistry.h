#pragma once
#include <vector>
#include <string>
#include <SFML/Graphics/Color.hpp>
#include <functional>
#include <Espresso/block/ModBlock.h>
#include <Espresso/catagory/ModCatagory.h>

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

	void RegisterCatagory(const ModCatagory& catagory);
	void RegisterBlock(const ModBlock& block);

	const ModBlock& GetBlock(const std::string& unlocalizedName);
	const ModCatagory& GetCatagory(const std::string& unlocalizedName);
private:
	std::vector<ModBlock> m_blocks;
	std::vector<ModCatagory> m_catagories;
};
