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
	void FrameUpdate(bool overrideBounding = false);
	void DeleteAllBlocks();
	void CopyEverything(Plane* plane, BlockRegistry* registry);
	unsigned int GetStackCount() const;
	sf::Vector2u GetSize() const;
	sf::Vector2u GetPosition() const;
	sf::Vector2i GetInnerPosition() const;
	void SetInnerPosition(sf::Vector2i position);
	sf::Vector2u GetAbsolutePosition() const;
	void SetPosition(sf::Vector2u position);
	void SetSize(sf::Vector2u size);
	Stack* GetStack(unsigned int index) const;
	bool IsToolbar() const;
	const std::vector<Stack*>* GetAllStacks() const;
	void ReloadVanity();

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
