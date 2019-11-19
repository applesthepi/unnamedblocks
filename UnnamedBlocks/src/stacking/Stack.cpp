#include "Stack.h"

void Stack::AddBlock(Block block)
{
	m_blocks.push_back(block);
}

void Stack::Render(sf::RenderWindow* window)
{
	for (unsigned int i = 0; i < m_blocks.size(); i++)
	{
		m_blocks[i].Render(window);
	}
}
