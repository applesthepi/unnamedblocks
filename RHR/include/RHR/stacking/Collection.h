#pragma once
#include "Stack.h"
#include "RHR/ui/IRenderable.h"
#include "RHR/ui/IMouseUpdatable.h"

class Collection
{
public:
	Collection();
	~Collection();

	void AddStack(Stack* stack);
	void AddStacks(const std::vector<Stack*>& stacks);

	const std::vector<Stack*>& GetStacks();
private:
	std::vector<Stack*> m_stacks;
};