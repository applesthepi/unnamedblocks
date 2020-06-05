#pragma once
#include "Stack.h"
#include "RHR/ui/ITransformable.h"

#define COLLECTION_COLOR_OUTLINE sf::Color(61, 61, 81, 255)
#define COLLECTION_COLOR_FILL sf::Color(23, 23, 28, 255)
#define COLLECTION_OUTLINE_WIDTH 4

class Collection : public ITransformable
{
public:
	Collection();
	Collection(const Collection& collection);

	~Collection();

	void AddStack(Stack* stack);
	void AddStacks(const std::vector<Stack*>& stacks);

	void RemoveStack(uint64_t idx);

	const std::vector<Stack*>& GetStacks();
private:
	std::vector<Stack*> m_stacks;
};