#pragma once
#include "Stack.h"
#include "RHR/ui/ITransformable.h"

#define COLLECTION_COLOR_OUTLINE_R 61
#define COLLECTION_COLOR_OUTLINE_G 61
#define COLLECTION_COLOR_OUTLINE_B 81
#define COLLECTION_COLOR_OUTLINE sf::Color(COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B)

#define COLLECTION_COLOR_FILL_R 23
#define COLLECTION_COLOR_FILL_G 23
#define COLLECTION_COLOR_FILL_B 28
#define COLLECTION_COLOR_FILL sf::Color(COLLECTION_COLOR_FILL_R, COLLECTION_COLOR_FILL_G, COLLECTION_COLOR_FILL_B)

#define COLLECTION_OUTLINE_WIDTH 4
#define COLLECTION_TAB_PART 30
#define COLLECTION_TAB_PART_COUNT 3

#define COLLECTION_EMPTY_SPACE 50

class Collection : public ITransformable
{
public:
	Collection();
	Collection(const Collection& collection);

	~Collection();

	void AddStack(Stack* stack);
	void AddStacks(const std::vector<Stack*>& stacks);

	void RemoveStack(uint64_t idx);
	void RemoveAll(bool dealloc);

	const std::vector<Stack*>& GetStacks();

	void SetEnabled(bool enabled);
	bool GetEnabled();
private:
	std::vector<Stack*> m_stacks;
	bool m_enabled;
};