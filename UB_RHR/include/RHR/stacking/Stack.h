#pragma once
#include "Block.h"
#include "RHR/ui/ITransformable.h"

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