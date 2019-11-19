#include "Plane.h"

void Plane::AddStack(Stack stack)
{
	m_stacks.push_back(stack);
}

void Plane::Render(sf::RenderWindow* window)
{
	for (unsigned int i = 0; i < m_stacks.size(); i++)
	{
		m_stacks[i].Render(window);
	}
}
