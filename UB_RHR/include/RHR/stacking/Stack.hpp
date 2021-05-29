#pragma once
#include "config.h"

#include "Block.hpp"

#include <Cappuccino/Utils.hpp>

class Stack : public ITransformable
{
public:
	Stack();
	Stack(const Stack& stack);

	~Stack();

	void AddBlock(Block* block);
	void AddBlocks(const std::vector<Block*>& blocks);
	void InsertBlocks(const std::vector<Block*>& blocks, uint64_t idx);

	uint64_t GetWidestBlock();

	void RemoveBlock(uint64_t idx);

	const std::vector<Block*>& GetBlocks();
private:
	std::vector<Block*> m_blocks;
};