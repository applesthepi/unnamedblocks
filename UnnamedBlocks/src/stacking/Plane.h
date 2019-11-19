#pragma once

#include "Stack.h"

#include <vector>

class Plane
{
public:
	void AddStack(Stack stack);
	void Render(sf::RenderWindow* window);
private:
	std::vector<Stack> m_stacks;
};