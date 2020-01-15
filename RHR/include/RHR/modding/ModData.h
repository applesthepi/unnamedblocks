#pragma once
#include "registries/BlockRegistry.h"

#include <vector>
#include <string>

class ModData
{
public:
	void RegisterBlock(const RegBlock block);
	void RegisterCatagory(const RegCatagory catatory);

	const std::vector<RegBlock>* GetBlocks();
	const std::vector<RegCatagory>* GetCatagories();
private:
	std::vector<RegBlock> m_blocks;
	std::vector<RegCatagory> m_catagories;
};