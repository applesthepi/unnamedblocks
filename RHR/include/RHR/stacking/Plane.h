#pragma once
#include "Collection.h"

#include "RHR/ui/IRenderable.h"
#include "RHR/ui/IMouseUpdatable.h"
#include "RHR/ui/ITransformable.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <functional>

class Plane : public ITransformable, public IRenderable, public IMouseUpdatable
{
public:
	Plane(const Plane& plane);
	Plane();

	~Plane();
	
	// Adds a collection to the plane. displayCollectionVanity should be true on PrimaryPlane and false on ToolbarPlane
	void AddCollection(Collection* collection, bool displayCollectionVanity);
	void AddCollections(const std::vector<Collection*>& collections);
	const std::vector<Collection*>& GetCollections();

	void DeleteCollection(uint64_t idx);

	void TranslateInnerPosition(const sf::Vector2i& position);
	void SetInnerPosition(sf::Vector2i position);
	const sf::Vector2i& GetInnerPosition();
	void DeleteContents();

	void frameUpdate(double deltaTime) override;
	bool mouseButton(bool down, const sf::Vector2i& position, const sf::Mouse::Button& button) override;

	static Plane* PrimaryPlane;
	static Plane* ToolbarPlane;
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	void Setup();

	// ===============================================================================================================
	// ================ Rendering
	// ===============================================================================================================

	void CreateBuffer(uint16_t collectionIdx, bool displayCollectionVanity);
	void UpdateBuffer(uint16_t bufferIdx);

	std::vector<std::vector<sf::Vertex>> m_collectionVertexArrays;
	std::vector<std::vector<sf::Vertex>> m_vertexArrays;
	std::vector<sf::VertexBuffer> m_vertexBuffers;
	std::vector<sf::Transform> m_vertexBufferTransform;
	std::vector<sf::Image> m_textureMapImage;
	std::vector<sf::Texture> m_textureMapTexture;
	std::vector<bool> m_textureMapEnabled;

	// ===============================================================================================================
	// ================ Selection & Context
	// ===============================================================================================================

	void Select(uint64_t collection, uint64_t stack, uint64_t block, uint64_t argument);
	void SelectContext(uint64_t collection, uint64_t stack, uint64_t block);
	void UnSelect();

	bool m_selected;
	bool m_selectedContext;
	Collection* m_selectedCollection;
	Stack* m_selectedStack;
	Block* m_selectedBlock;
	Argument* m_selectedArgument;

	std::function<void(uint8_t)> m_contextCallback;

	// ===============================================================================================================
	// ================ Dragging
	// ===============================================================================================================

	void DragCollection(uint64_t collection, bool up);
	void DragStack(uint64_t collection, uint64_t stack, bool up);
	void UnDrag();

	bool DraggingCollection();
	bool DraggingStack();

	Collection* m_draggingCollection;
	Stack* m_draggingStack;

	sf::Vector2f m_draggingBeginObject;
	sf::Vector2i m_draggingBeginMouse;

	bool m_draggingUp;

	// ===============================================================================================================
	// ================ Other Data
	// ===============================================================================================================

	std::vector<bool> m_collectionVanity;
	std::vector<Collection*> m_collections;

	sf::Vector2i m_innerPosition;
	sf::Text m_innerText;
};
