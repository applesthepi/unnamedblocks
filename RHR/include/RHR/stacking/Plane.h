#pragma once
#include "Collection.h"

#include "RHR/ui/IRenderable.h"
#include "RHR/ui/IMouseUpdatable.h"
#include "RHR/ui/ITransformable.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/System/Vector2.hpp>
#include <functional>

#define SNAP_DISTANCE 30.0f
#define SNAP_GRAPHIC_HEIGHT 2.0f

class RevertWindowView
{
public:
	RevertWindowView(sf::RenderWindow* window)
		:m_window(window) {}

	~RevertWindowView()
	{
		m_window->setView(m_window->getDefaultView());
	}
private:
	sf::RenderWindow* m_window;
};

class Plane : public ITransformable, public IRenderable, public IMouseUpdatable
{
public:
	Plane(bool toolbar, const Plane& plane);
	Plane(bool toolbar);

	~Plane();
	
	// Adds a collection to the plane. displayCollectionVanity should be true on PrimaryPlane and false on ToolbarPlane
	void AddCollection(Collection* collection, bool displayCollectionVanity);
	const std::vector<Collection*>& GetCollections();

	void DeleteCollection(uint64_t idx, bool dealloc = true);

	void TranslateInnerPosition(const sf::Vector2i& position);
	void SetInnerPosition(sf::Vector2i position);
	
	sf::Vector2i GetInnerPosition();
	void DeleteContents(bool dealloc = true);

	// TODO not finished; may not need
	void UpdateVAOPosition(uint64_t idx);

	void UpdateVAO(uint64_t idx);

	void frameUpdate(double deltaTime) override;
	void render(sf::RenderWindow& window);
	void snapRender(sf::RenderWindow& window);
	void postRender(sf::RenderWindow& window);
	bool mouseButton(bool down, const sf::Vector2i& position, const sf::Mouse::Button& button) override;

	sf::View* GetView();
	sf::Vector2f CalculateZoom();

	static Plane* PrimaryPlane;
	static Plane* ToolbarPlane;
protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	void Setup(bool toolbar);
	void UpdateCollectionVAO(std::vector<sf::Vertex>* vao, sf::Vector2u size);

	//////////////////////////////////////////////////////////
	// Rendering
	//////////////////////////////////////////////////////////

	// creates a buffer for the collection
	void CreateBuffer(uint16_t collectionIdx, bool displayCollectionVanity);

	// updates the buffer for a collection
	void UpdateBuffer(uint16_t bufferIdx);

	// parses an array into a nice vertex array that sfml can understand
	void ParseIndices(std::vector<sf::Vertex>* vao, const float positions[], const uint8_t colors[], const float textureCoords[], const uint8_t indices[], uint8_t indexCount);
	std::vector<std::vector<sf::Vertex>> m_vertexArrays;
	std::vector<sf::VertexBuffer> m_vertexBuffers;
	std::vector<sf::Transform> m_vertexBufferTransform;
	std::vector<sf::Image> m_textureMapImage;
	std::vector<sf::Texture> m_textureMapTexture;
	std::vector<bool> m_textureMapEnabled;

	sf::Shader m_shader;
	sf::Texture m_textureCollectionOpen;
	sf::Texture m_textureCollectionClosed;

	//////////////////////////////////////////////////////////
	// Selection & Context
	//////////////////////////////////////////////////////////

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

	//////////////////////////////////////////////////////////
	// Dragging
	//////////////////////////////////////////////////////////

	void DragCollection(Collection* collection, bool up);
	void DragStack(Collection* collection, Stack* stack, bool up);
	void UnDrag(const sf::Vector2i& position);
	void DraggingStackUpdate();

	bool DraggingCollection();
	bool DraggingStack();

	Collection* m_draggingCollection;
	Stack* m_draggingStack;

	sf::Vector2f m_draggingBeginObject;
	sf::Vector2i m_draggingBeginMouse;

	bool m_draggingUp;

	//////////////////////////////////////////////////////////
	// Snapping
	//////////////////////////////////////////////////////////

	void SetSnap(uint64_t collection, uint64_t stackLoc, Stack* stack);
	void ClearSnap();

	bool IsSnap();

	sf::RectangleShape m_draggingShape;

	uint64_t m_draggingSnapCollection;
	uint64_t m_draggingSnapStackLoc;
	Stack* m_draggingSnapStack;

	bool m_draggingSnap;

	//////////////////////////////////////////////////////////
	// Other Data
	//////////////////////////////////////////////////////////

	std::vector<bool> m_collectionVanity;
	std::vector<Collection*> m_collections;

	sf::Vector2i m_innerPosition;
	sf::Text m_innerText;
	bool m_toolbar;
	sf::View m_view;
	sf::RenderWindow* m_window;
	sf::RectangleShape m_background;

	const sf::Texture& m_fontTexture;
	sf::Image m_fontEditedImage;
	sf::Texture m_fontEditedTexture;

	const sf::Texture& m_fontBoldTexture;
	sf::Image m_fontBoldEditedImage;
	sf::Texture m_fontBoldEditedTexture;
};
