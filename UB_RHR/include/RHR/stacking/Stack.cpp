#include "Stack.hpp"

Stack::Stack()
{
	m_blocks.reserve(10);
}

Stack::Stack(const Stack& stack)
{
	for (uint64_t i = 0; i < m_blocks.size(); i++)
		delete m_blocks[i];

	m_blocks.clear();

	for (uint64_t i = 0; i < stack.m_blocks.size(); i++)
		m_blocks.push_back(new Block(*stack.m_blocks[i]));
}

Stack::~Stack()
{
	for (uint64_t i = 0; i < m_blocks.size(); i++)
		delete m_blocks[i];
}

void Stack::AddBlock(Block* block)
{
	m_blocks.push_back(block);
}

void Stack::AddBlocks(const std::vector<Block*>& blocks)
{
	if (m_blocks.size() + blocks.size() >= m_blocks.capacity())
		m_blocks.reserve((uint64_t)std::ceil((float)(m_blocks.size() + blocks.size()) * 1.5f + 10.0f));

	for (uint64_t i = 0; i < blocks.size(); i++)
		m_blocks.push_back(blocks[i]);
}

void Stack::InsertBlocks(const std::vector<Block*>& blocks, uint64_t idx)
{
	m_blocks.insert(m_blocks.begin() + idx, blocks.begin(), blocks.end());
}

uint64_t Stack::GetWidestBlock()
{
	uint64_t widest = 0;

	for (uint64_t i = 0; i < m_blocks.size(); i++)
	{
		if (m_blocks[i]->GetWidth() > widest)
			widest = m_blocks[i]->GetWidth();
	}

	return widest;
}

void Stack::RemoveBlock(uint64_t idx)
{
	m_blocks.erase(m_blocks.begin() + idx);
}

const std::vector<Block*>& Stack::GetBlocks()
{
	return m_blocks;
}