#pragma once
#include "Block.h"
#include "StatmentIf.h"

#include <vector>
#include <functional>
#include <SFML/Graphics.hpp>

enum DraggingType
{
	DOWN, UP
};

class Stack
{
public:
	Stack(sf::Vector2i relitivePosition, BlockRegistry* registry);
	~Stack();

	void ImportBlocks(std::vector<Block*>* blocks);
	void AddBlock(Block* block);
	void ReloadAllBlocks();
	void Render(sf::RenderTexture* render, sf::RenderWindow* window);
	void FrameUpdate(bool updateBlocks, bool forceUpdate = false);
	void SetupInPlane(sf::Vector2u* planePosition, sf::Vector2i* planeInnerPosition, void* planePtr, std::function<void(Stack* stack)>* functionAdd, std::function<void(Stack* stack)>* functionRemove, std::function<void(Stack* stack)>* functionMoveTop, std::function<void(Stack* stack)>* functionAddOver);

	void* GetPlanePointer();
	unsigned int GetBlockCount();
	sf::Vector2i GetAbsolutePosition();
	sf::Vector2i GetRelitivePosition();
	sf::Vector2i GetSetPosition();
	sf::Vector2u GetPlanePosition();
	void SetRelitivePosition(sf::Vector2i position);
	unsigned int GetBlockWidth(unsigned int index);
	Block* GetBlock(unsigned int index);
	void CopyEverything(Stack* stack, BlockRegistry* registry);
	void DragDown(sf::Vector2i mousePosition);
	void DragUp(sf::Vector2i mousePosition);
	std::function<void()>* GetFunctionUpdate();
	std::function<void()>* GetFunctionSelect();
	bool IsBounding(const sf::Vector2f& mousePos);

	bool MouseButton(bool down, sf::Vector2i position, sf::Mouse::Button button);
	void ReloadVanity();
	std::vector<StatmentIf> GetVanity();
	void ReRender();
	void IndexVariables();
private:
	void PreRender();
	std::vector<Block*> m_blocks;
	sf::Vector2i m_setPosition;
	sf::Vector2i m_relitivePosition;
	sf::Vector2i m_absolutePosition;
	sf::Vector2u* m_planePosition;
	sf::Vector2i* m_planeInnerPosition;

	sf::RenderTexture m_preTexture;
	sf::RectangleShape m_preShape;

	std::vector<StatmentIf> m_savedVanity;
	bool m_validHighlighting;
	int32_t m_highlightedShape;
	std::vector<uint32_t> m_ifShapeIdx;
	std::vector<sf::RectangleShape> m_ifShapes;
	std::vector<sf::RectangleShape> m_ifShapeHighlight;
	std::vector<sf::Vector2f> m_ifShapePositions;
	std::vector<sf::Vector2f> m_ifShapePositionHighlight;
	DraggingType m_draggingType;
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
	std::function<void()>* m_functionUpdatePreTexture;
	std::function<void()>* m_functionSelectStack;
	unsigned int m_contextBlockIndex;
	uint64_t m_highestWidth;
	void* m_planePtr;
	void* m_blockRegistry;
};
