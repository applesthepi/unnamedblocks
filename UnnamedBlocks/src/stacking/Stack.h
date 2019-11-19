#pragma once

#include "Block.h"

#include <vector>

class Stack
{
public:
	void AddBlock(Block block);
	void Render(sf::RenderWindow* window);
private:
	std::vector<Block> m_blocks;
};