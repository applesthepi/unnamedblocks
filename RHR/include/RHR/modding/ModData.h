#pragma once
#include "registries/BlockRegistry.h"

#include <vector>
#include <string>

class ModData
{
public:
	ModData(BlockRegistry* registry);

	void RegisterBlock(const RegBlock block);
	void RegisterCatagory(const RegCatagory catatory);

	std::vector<RegBlock>* GetBlocks();
	std::vector<RegCatagory>* GetCatagories();
	
	BlockRegistry* Registry;
private:
	std::vector<RegBlock> m_blocks;
	std::vector<RegCatagory> m_catagories;
};