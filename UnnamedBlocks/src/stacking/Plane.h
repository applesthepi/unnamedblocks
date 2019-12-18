#pragma once

#include "Stack.h"

#include <vector>
#include <functional>

class Plane
{
public:
	Plane(sf::Vector2u position, sf::Vector2u size, bool toolbar = false);
	~Plane();

	void AddStack(Stack* stack);
	void Render(sf::RenderWindow* window);
	void RenderConnection(sf::RenderWindow* window);
	void FrameUpdate(sf::RenderWindow* window);
	void DeleteAllBlocks();
	void CopyEverything(Plane* plane);
	unsigned int GetStackCount();
	sf::Vector2u GetSize();
	sf::Vector2u GetPosition();
	sf::Vector2i GetInnerPosition();
	void SetInnerPosition(sf::Vector2i position);
	sf::Vector2u GetAbsolutePosition();
	void SetPosition(sf::Vector2u position);
	void SetSize(sf::Vector2u size);
	Stack* GetStack(unsigned int index);
	bool IsToolbar();
	const std::vector<Stack*>* GetAllStacks();

	void MouseButton(bool down, sf::Vector2i position, sf::Mouse::Button button);

	static std::vector<Plane*>* Planes;
private:
	std::vector<Stack*> m_stacks;
	sf::Vector2u* m_position;
	sf::Vector2i* m_innerPosition;
	sf::Vector2u* m_absolutePosition;
	sf::Vector2u m_size;
	sf::Vector2i m_draggingMouseStart;
	sf::Vector2i m_draggingStart;
	std::function<void(Stack* stack)>* m_functionAdd;
	std::function<void(Stack* stack)>* m_functionRemove;
	std::function<void(Stack* stack)>* m_functionMoveTop;
	std::function<void(Stack* stack)>* m_functionAddOver;

	sf::RectangleShape m_background;
	sf::RectangleShape m_draggingConnection;
	bool m_useDraggingConnection;
	bool m_toolbar;
	bool m_dragging;

	sf::Text m_innerPositionText;
};