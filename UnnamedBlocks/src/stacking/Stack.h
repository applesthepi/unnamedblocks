#pragma once

#include "Block.h"

#include <vector>
#include <functional>

class Stack
{
public:
	Stack(sf::Vector2i relitivePosition);
	~Stack();

	void ImportBlocks(std::vector<Block*>* blocks);
	void AddBlock(Block* block);
	void ReloadAllBlocks();
	void Render(sf::RenderTexture* render, sf::RenderWindow* window);
	void FrameUpdate(sf::RenderWindow* window);
	void SetupInPlane(sf::Vector2u* planePosition, sf::Vector2u* planeInnerPosition, void* planePtr, std::function<void(Stack* stack)>* functionAdd, std::function<void(Stack* stack)>* functionRemove, std::function<void(Stack* stack)>* functionMoveTop, std::function<void(Stack* stack)>* functionAddOver);

	unsigned int GetBlockCount();
	sf::Vector2i GetAbsolutePosition();
	sf::Vector2i GetRelitivePosition();
	sf::Vector2i GetSetPosition();
	sf::Vector2u GetPlanePosition();
	void SetRelitivePosition(sf::Vector2i position);
	unsigned int GetBlockWidth(unsigned int index);
	Block* GetBlock(unsigned int index);
	void CopyEverything(Stack* stack);

	bool MouseButton(bool down, sf::Vector2i position, sf::Mouse::Button button);
private:
	std::vector<Block*> m_blocks;
	sf::Vector2i m_setPosition;
	sf::Vector2i m_relitivePosition;
	sf::Vector2i m_absolutePosition;
	sf::Vector2u* m_planePosition;
	sf::Vector2u* m_planeInnerPosition;

	bool m_dragging = false;
	bool m_cutRendering;
	sf::Vector2i m_draggingMouseBegin;
	std::function<void(Stack* stack)>* m_functionAdd;
	std::function<void(Stack* stack)>* m_functionRemove;
	std::function<void(Stack* stack)>* m_functionMoveTop;
	std::function<void(Stack* stack)>* m_functionAddOver;
	std::function<void(unsigned int index, sf::Vector2i mousePosition)>* m_functionContext;
	std::function<void(unsigned int index)>* m_functionContextCallback;
	std::function<void(unsigned int index, sf::Vector2i mousePosition)>* m_functionSplit;
	unsigned int m_contextBlockIndex;
	void* m_planePtr;
};