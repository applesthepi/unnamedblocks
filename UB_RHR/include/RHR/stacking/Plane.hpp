#pragma once
#include "config.h"

#include "stacking/Collection.hpp"
#include "ui/interfaces/IPositionable.hpp"
#include "ui/interfaces/ISizeable.hpp"
#include "ui/RenderRectangle.hpp"
#include "ui/RenderText.hpp"

#include <Cappuccino/Utils.hpp>
#include <Cappuccino/Intrinsics.hpp>
#include <Espresso/InputHandler.hpp>

#define SNAP_DISTANCE 30.0f
#define SNAP_GRAPHIC_HEIGHT 2.0f

//class RevertWindowView
//{
//public:
//	RevertWindowView(sf::RenderWindow* window)
//		:m_window(window) {}
//
//	~RevertWindowView()
//	{
//		m_window->setView(m_window->getDefaultView());
//	}
//private:
//	sf::RenderWindow* m_window;
//};

class Plane : public IPositionable<2, int32_t>, public ISizeable<int32_t>, public IRenderable, public IUpdatable
{
public:
	//Plane(bool toolbar, const Plane& plane);
	Plane(bool toolbar);

	~Plane();
	
	// Adds a collection to the plane. displayCollectionVanity should be true on PrimaryPlane and false on ToolbarPlane
	void AddCollection(std::shared_ptr<Collection>& collection, bool displayCollectionVanity);
	const std::vector<std::shared_ptr<Collection>>& GetCollections();

	void DeleteCollection(uint64_t idx);
	bool IsToolbar();

	//void TranslateInnerPosition(const glm::vec<2, int32_t>& position);
	//void SetInnerPosition(glm::vec<2, int32_t> position);
	
	//glm::vec<2, int32_t> GetInnerPosition();
	void DeleteContents();

	// TODO not finished; may not need
	//void UpdateVAOPosition(uint64_t idx);

	//void UpdateVAO(uint64_t idx);

	//void frameUpdate(double deltaTime) override;
	//void render(sf::RenderWindow& window);
	//void snapRender(sf::RenderWindow& window);
	//void postRender(sf::RenderWindow& window);
	void MouseButton(glm::vec<2, int32_t> position, float scroll, MouseOperation operation);

	//sf::View* GetView();
	//sf::Vector2f CalculateZoom();
	//sf::Vector2f GetCoordsFromPixel(sf::Vector2f pixel, sf::RenderWindow& window);

	static std::shared_ptr<Plane> PrimaryPlane;
	static std::shared_ptr<Plane> ToolbarPlane;
//protected:
	//virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	void FrameUpdate(double deltaTime) override;
	void OnRender() override;
	void OnUpdateBuffers() override;
	void OnReloadSwapChain() override;
	void PostPositionUpdate() override;
	void PostSizeUpdate() override;

	//void Setup(bool toolbar);
	//void UpdateCollectionVAO(std::vector<sf::Vertex>* vao, sf::Vector2u size);

	//////////////////////////////////////////////////////////
	// Rendering
	//////////////////////////////////////////////////////////

	// creates a buffer for the collection
	//void CreateBuffer(uint16_t collectionIdx, bool displayCollectionVanity);

	// updates the buffer for a collection
	//void UpdateBuffer(uint16_t bufferIdx);

	// parses an array into a nice vertex array that sfml can understand
	//void ParseIndices(std::vector<sf::Vertex>* vao, const float positions[], const uint8_t colors[], const float textureCoords[], const uint8_t indices[], uint8_t indexCount);
	//std::vector<std::vector<sf::Vertex>> m_vertexArrays;
	//std::vector<sf::VertexBuffer> m_vertexBuffers;
	//std::vector<sf::Transform> m_vertexBufferTransform;
	//std::vector<sf::Image> m_textureMapImage;
	//std::vector<sf::Texture> m_textureMapTexture;
	//std::vector<bool> m_textureMapEnabled;

	//sf::Shader m_shader;
	//sf::Texture m_textureCollectionOpen;
	//sf::Texture m_textureCollectionClosed;

	//////////////////////////////////////////////////////////
	// Selection & Context
	//////////////////////////////////////////////////////////

	void Select(uint64_t collection, uint64_t stack, uint64_t block, uint64_t argument);
	void SelectContext(uint64_t collection, uint64_t stack, uint64_t block);
	void UnSelect();

	bool m_Selected;
	bool m_SelectedContext;
	std::shared_ptr<Collection> m_SelectedCollection;
	std::shared_ptr<Stack> m_SelectedStack;
	std::shared_ptr<Block> m_SelectedBlock;
	std::shared_ptr<Argument> m_SelectedArgument;

	std::function<void(uint8_t)> m_ContextCallback;

	//////////////////////////////////////////////////////////
	// Dragging
	//////////////////////////////////////////////////////////

	void DragCollection(std::shared_ptr<Collection>& collection, bool up);
	void DragStack(std::shared_ptr<Collection>& collection, std::shared_ptr<Stack>& stack, bool up);
	void UnDrag(const glm::vec<2, int32_t>& position);
	void DraggingStackUpdate();

	bool DraggingCollection();
	bool DraggingStack();

	std::shared_ptr<Collection> m_DraggingCollection;
	std::shared_ptr<Stack> m_DraggingStack;

	glm::vec<2, int32_t> m_DraggingBeginObject;
	glm::vec<2, int32_t> m_DraggingBeginMouse;

	bool m_DraggingUp;

	//////////////////////////////////////////////////////////
	// Snapping
	//////////////////////////////////////////////////////////

	void SetSnap(uint64_t collection, uint64_t stackLoc, std::shared_ptr<Stack> stack);
	void ClearSnap();

	bool IsSnap();

	vui::RenderRectangle m_DraggingShape;

	uint64_t m_DraggingSnapCollection;
	uint64_t m_DraggingSnapStackLoc;
	std::shared_ptr<Stack> m_DraggingSnapStack;

	bool m_DraggingSnap;

	//////////////////////////////////////////////////////////
	// Other Data
	//////////////////////////////////////////////////////////

	std::vector<bool> m_CollectionVanity;
	std::vector<std::shared_ptr<Collection>> m_Collections;

	//glm::vec<2, int32_t> m_innerPosition;
	//sf::View m_view;
	//sf::RenderWindow* m_window;

	bool m_Toolbar;
	vui::RenderText m_InnerText;
	std::shared_ptr<vui::RenderRectangle> m_Background;

	//const sf::Texture& m_fontTexture;
	//sf::Image m_fontEditedImage;
	//sf::Texture m_fontEditedTexture;

	//const sf::Texture& m_fontBoldTexture;
	//sf::Image m_fontBoldEditedImage;
	//sf::Texture m_fontBoldEditedTexture;
};
