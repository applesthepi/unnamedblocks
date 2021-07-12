#include "Stack.hpp"

Stack::Stack()
{
	m_blocks.reserve(10);
}

//Stack::Stack(const Stack& stack)
//{
//	for (uint64_t i = 0; i < m_blocks.size(); i++)
//		delete m_blocks[i];
//
//	m_blocks.clear();
//
//	for (uint64_t i = 0; i < stack.m_blocks.size(); i++)
//		m_blocks.push_back(new Block(*stack.m_blocks[i]));
//}

Stack::~Stack()
{
	//for (uint64_t i = 0; i < m_blocks.size(); i++)
	//	delete m_blocks[i];
}

void Stack::AddBlock(std::shared_ptr<Block> block)
{
	block->SetPosition({ 0, Block::Height * m_blocks.size() });
	block->SetSuperOffset(m_Position + m_SuperOffset);
	m_blocks.push_back(block);
}

void Stack::AddBlocks(const std::vector<std::shared_ptr<Block>>& blocks)
{
	if (m_blocks.size() + blocks.size() >= m_blocks.capacity())
		m_blocks.reserve((uint64_t)std::ceil((float)(m_blocks.size() + blocks.size()) * 1.5f + 10.0f));

	for (uint64_t i = 0; i < blocks.size(); i++)
	{
		blocks[i]->SetPosition({ 0, Block::Height * (m_blocks.size() + i) });
		blocks[i]->SetSuperOffset(m_Position + m_SuperOffset);
		m_blocks.push_back(blocks[i]);
	}
}

void Stack::InsertBlocks(const std::vector<std::shared_ptr<Block>>& blocks, uint64_t idx)
{
	for (uint64_t i = 0; i < blocks.size(); i++)
	{
		blocks[i]->SetPosition({ 0, Block::Height * (m_blocks.size() + i) });
		blocks[i]->SetSuperOffset(m_Position + m_SuperOffset);
		m_blocks.push_back(blocks[i]);
	}

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

	for (size_t i = idx; i < m_blocks.size(); i++)
	{
		m_blocks[i]->SetPosition({ 0, Block::Height * i });
		m_blocks[i]->SetSuperOffset(m_Position + m_SuperOffset);
	}
}

const std::vector<std::shared_ptr<Block>>& Stack::GetBlocks()
{
	return m_blocks;
}

void Stack::FrameUpdate(double deltaTime)
{
	for (auto& block : m_blocks)
		block->FrameUpdate(deltaTime);
}

void Stack::OnRender()
{
	for (auto& block : m_blocks)
		block->Render();
}

void Stack::OnUpdateBuffers()
{
	for (auto& block : m_blocks)
		block->UpdateBuffers();
}

void Stack::OnReloadSwapChain()
{
	for (auto& block : m_blocks)
		block->ReloadSwapChain();
}

void Stack::PostPositionUpdate()
{
	for (auto& block : m_blocks)
		block->SetSuperOffset(m_Position + m_SuperOffset);
}
