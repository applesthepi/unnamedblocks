#pragma once
#include "config.h"

#include "Block.hpp"
#include "ui/interfaces/IPositionable.hpp"
#include "ui/interfaces/IRenderable.hpp"
#include "ui/interfaces/IUpdatable.hpp"
#include "ui/interfaces/ISizeable.hpp"

#include <Cappuccino/Utils.hpp>

class Stack : public IPositionable<2, int32_t>, public ISizeable<int32_t>, public IRenderable, public IUpdatable
{
public:
	Stack();
	~Stack();

	void AddBlock(std::shared_ptr<Block> block);
	void AddBlocks(const std::vector<std::shared_ptr<Block>>& blocks);
	void InsertBlocks(const std::vector<std::shared_ptr<Block>>& blocks, uint64_t idx);

	uint64_t GetWidestBlock();

	void RemoveBlock(uint64_t idx);
	void UpdateSize();
	const std::vector<std::shared_ptr<Block>>& GetBlocks();
	void FrameUpdate(double deltaTime) override;
private:
	void OnRender() override;
	void OnUpdateBuffers() override;
	void OnReloadSwapChain() override;
	void PostPositionUpdate() override;

	std::vector<std::shared_ptr<Block>> m_blocks;
};