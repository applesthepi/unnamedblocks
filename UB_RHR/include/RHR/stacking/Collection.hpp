#pragma once
#include "config.h"

#include "stacking/Stack.hpp"
#include "ui/interfaces/IPositionable.hpp"
#include "ui/interfaces/ISizeable.hpp"
#include "ui/interfaces/IRenderable.hpp"
#include "ui/interfaces/IUpdatable.hpp"
#include "ui/interfaces/IEnableable.hpp"

#include <Cappuccino/Utils.hpp>

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

class Collection : public IPositionable<2, int32_t>, public ISizeable<int32_t>, public IRenderable, public IUpdatable, public IEnableable
{
public:
	Collection();
	//Collection(const Collection& collection);

	~Collection();

	void AddStack(std::shared_ptr<Stack> stack);
	void AddStacks(const std::vector<std::shared_ptr<Stack>>& stacks);

	void RemoveStack(uint64_t idx);
	void RemoveAll();

	const std::vector<std::shared_ptr<Stack>>& GetStacks();
	void FrameUpdate(double deltaTime) override;
private:
	void OnRender() override;
	void OnUpdateBuffers() override;
	void OnReloadSwapChain() override;
	void PostPositionUpdate() override;

	std::vector<std::shared_ptr<Stack>> m_stacks;
};