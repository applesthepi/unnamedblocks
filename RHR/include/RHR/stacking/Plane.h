#pragma once
#include "Collection.h"

#include "RHR/ui/IRenderable.h"
#include "RHR/ui/IMouseUpdatable.h"
#include "RHR/ui/ITransformable.h"

#define VA_DATA_OFFSET 24

class Plane : public ITransformable, public IRenderable, public IMouseUpdatable
{
public:
	Plane();
	~Plane();

	void AddCollection(Collection* collection);
	void AddCollections(const std::vector<Collection*>& collections);

	const std::vector<Collection*>& GetCollections();

	void frameUpdate(const double& deltaTime) override;
	const bool mouseButton(const bool& down, const sf::Vector2i& position, const sf::Mouse::Button& button) override;
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	void CreateBuffer(const uint16_t& collectionIdx);
	void UpdateBuffer(const uint16_t& bufferIdx);

	std::vector<Collection*> m_collections;

	sf::Vector2i m_innerPosition;

	std::vector<std::vector<sf::Vertex>> m_vertexArrays;
	std::vector<sf::VertexBuffer> m_vertexBuffers;
	sf::Transform m_vertexArrayTransform;
	sf::Text m_innerText;





	/*
	Plane(sf::Vector2u position, sf::Vector2u size, bool toolbar = false);
	~Plane();

	void AddStack(Stack* stack);
	void Render(sf::RenderWindow* window);
	void RenderConnection(sf::RenderWindow* window);
	void FrameUpdate(BlockRegistry* blockRegistry, bool overrideBounding = false);
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
	void AddInnerPosition(const sf::Vector2i& change);

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
	sf::RenderTexture m_renderBuffer;
	sf::Sprite m_renderSprite;
	*/
};
