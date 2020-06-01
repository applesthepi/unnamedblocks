#include "Block.h"
#include "RHR/registries/BlockRegistry.h"

#include <Espresso/Global.h>

Block::Block(const std::string& unlocalizedName)
	:m_modBlock(BlockRegistry::GetRegistry().GetBlock(unlocalizedName))
{
	m_modCategory = BlockRegistry::GetRegistry().GetCategory(m_modBlock->GetCategory());

	m_arguments.reserve(10);
}

Block::Block(const Block& block)
	:m_modBlock(block.m_modBlock)
{
	m_modCategory = block.m_modCategory;

	for (uint64_t i = 0; i < m_arguments.size(); i++)
		delete m_arguments[i];

	m_arguments.clear();

	for (uint64_t i = 0; i < block.m_arguments.size(); i++)
		m_arguments.push_back(new Argument(*block.m_arguments[i]));
}

Block::~Block()
{
	for (uint64_t i = 0; i < m_arguments.size(); i++)
		delete m_arguments[i];
}

void Block::AddArgument(Argument* argument)
{
	m_arguments.push_back(argument);
}

void Block::AddArguments(const std::vector<Argument*>& arguments)
{
	if (m_arguments.size() + arguments.size() >= m_arguments.capacity())
		m_arguments.reserve((uint64_t)std::ceil((float)(m_arguments.size() + arguments.size()) * 1.5f + 10.0f));

	for (uint64_t i = 0; i < arguments.size(); i++)
		m_arguments.push_back(arguments[i]);
}

const std::vector<Argument*>& Block::GetArguments()
{
	return m_arguments;
}

const uint32_t& Block::GetWidth()
{
	return m_width;
}

const ModBlock* Block::GetModBlock()
{
	return m_modBlock;
}

const ModCatagory* Block::GetModCategory()
{
	return m_modCategory;
}

void Block::UpdateWidth()
{
	m_width = Global::BlockBorder;

	for (uint64_t i = 0; i < m_arguments.size(); i++)
	{
		m_width += m_arguments[i]->GetWidth();
		m_width += Global::BlockBorder;
	}
}