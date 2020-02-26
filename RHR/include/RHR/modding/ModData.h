#pragma once
#include "handlers/runtime/ByteHandler.h"
#include "handlers/runtime/ObjectHandler.h"
#include "handlers/runtime/RuntimeHandler.h"
#include "handlers/runtime/ThreadHandler.h"
#include "handlers/runtime/VariableHandler.h"
#include "registries/BlockRegistry.h"

#include <vector>
#include <string>

class ModData
{
public:
	ModData();

	void RegisterBlock(const RegBlock block);
	void RegisterCatagory(const RegCatagory catatory);

	std::vector<RegBlock>* GetBlocks();
	std::vector<RegCatagory>* GetCatagories();
	
	BlockRegistry* Registry;
	RuntimeHandler* Runtime;
	ThreadHandler* Thread;
	VariableHandler* Variable;
	ByteHandler* Byte;
	ObjectHandler* Object;
private:
	std::vector<RegBlock> m_blocks;
	std::vector<RegCatagory> m_catagories;
};
