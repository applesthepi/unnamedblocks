#include "Collection.hpp"

Collection::Collection()
{
	m_stacks.reserve(5);
	m_enabled = true;
}

Collection::Collection(const Collection& collection)
{
	for (uint64_t i = 0; i < m_stacks.size(); i++)
		delete m_stacks[i];

	m_stacks.clear();

	for (uint64_t i = 0; i < collection.m_stacks.size(); i++)
		m_stacks.push_back(new Stack(*collection.m_stacks[i]));
}

Collection::~Collection()
{
	for (uint64_t i = 0; i < m_stacks.size(); i++)
		delete m_stacks[i];
}

void Collection::AddStack(Stack* stack)
{
	m_stacks.push_back(stack);

	if (stack->getPosition().x + stack->GetWidestBlock() > getSize().x)
		m_size.x = stack->getPosition().x + stack->GetWidestBlock() + COLLECTION_EMPTY_SPACE;

	if (stack->getPosition().y + (stack->GetBlocks().size() * Global::BlockHeight) > getSize().y)
		m_size.y = stack->getPosition().y + (stack->GetBlocks().size() * Global::BlockHeight) + COLLECTION_EMPTY_SPACE;
}

void Collection::AddStacks(const std::vector<Stack*>& stacks)
{
	if (m_stacks.size() + stacks.size() >= m_stacks.capacity())
		m_stacks.reserve((uint64_t)std::ceil((float)(m_stacks.size() + stacks.size()) * 1.5f + 10.0f));

	for (uint64_t i = 0; i < stacks.size(); i++)
	{
		m_stacks.push_back(stacks[i]);

		if (stacks[i]->getPosition().x + stacks[i]->GetWidestBlock() > getSize().x)
			m_size.x = stacks[i]->getPosition().x + stacks[i]->GetWidestBlock() + COLLECTION_EMPTY_SPACE;

		if (stacks[i]->getPosition().y + (stacks[i]->GetBlocks().size() * Global::BlockHeight) > getSize().y)
			m_size.y = stacks[i]->getPosition().y + (stacks[i]->GetBlocks().size() * Global::BlockHeight) + COLLECTION_EMPTY_SPACE;
	}
}

void Collection::RemoveStack(uint64_t idx)
{
	m_stacks.erase(m_stacks.begin() + idx);
}

void Collection::RemoveAll(bool dealloc)
{
	if (dealloc)
	{
		for (uint64_t i = 0; i < m_stacks.size(); i++)
			delete m_stacks[i];
	}

	m_stacks.clear();
}

const std::vector<Stack*>& Collection::GetStacks()
{
	return m_stacks;
}

void Collection::SetEnabled(bool enabled)
{
	m_enabled = enabled;
}

bool Collection::GetEnabled()
{
	return m_enabled;
}
