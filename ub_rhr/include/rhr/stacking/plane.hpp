#pragma once
#include "config.h"

#include "rhr/stacking/collection.hpp"
#include "rhr/rendering/interfaces/i_positionable.hpp"
#include "rhr/rendering/interfaces/i_updateable.hpp"
#include "rhr/rendering/interfaces/i_renderable.hpp"
#include "rhr/rendering/interfaces/i_sizeable.hpp"
#include "rhr/rendering/objects/rectangle.hpp"
#include "rhr/rendering/objects/text.hpp"

#include <cappuccino/utils.hpp>
#include <cappuccino/intrinsics.hpp>
#include <espresso/input_handler.hpp>

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

namespace rhr::stack
{
///
class plane : public rhr::render::interfaces::i_positionable<2, i32>, public rhr::render::interfaces::i_sizeable<2, i32>, public rhr::render::interfaces::i_renderable, public rhr::render::interfaces::i_updateable
{
public:
	///
	plane(bool toolbar);

	///
	~plane();

	/// Adds a collection to the plane. displayCollectionVanity should be true on PrimaryPlane and false on ToolbarPlane
	void AddCollection(std::shared_ptr<rhr::stack::collection>& collection, bool displayCollectionVanity);
	const std::vector<std::shared_ptr<rhr::stack::collection>>& GetCollections();

	void DeleteCollection(u64 idx);
	bool IsToolbar();

	//void TranslateInnerPosition(const glm::vec<2, i32>& position);
	//void SetInnerPosition(glm::vec<2, i32> position);

	//glm::vec<2, i32> GetInnerPosition();
	void DeleteContents();

	// TODO not finished; may not need
	//void UpdateVAOPosition(u64 idx);

	//void UpdateVAO(u64 idx);

	//void frameUpdate(f64 deltaTime) override;
	//void render(sf::RenderWindow& window);
	//void snapRender(sf::RenderWindow& window);
	//void postRender(sf::RenderWindow& window);
	void MouseButton(glm::vec<2, i32> position, f32 scroll, MouseOperation operation);

	//sf::View* GetView();
	//sf::Vector2f CalculateZoom();
	//sf::Vector2f GetCoordsFromPixel(sf::Vector2f pixel, sf::RenderWindow& window);

	static std::shared_ptr<Plane> PrimaryPlane;
	static std::shared_ptr<Plane> ToolbarPlane;
	//protected:
		//virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void frame_update(f64 deltaTime) override;
private:
	void on_render() override;
	void on_update_buffers() override;
	void on_reload_swap_chain() override;
	void post_position_update() override;
	void post_size_update() override;

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
	//void ParseIndices(std::vector<sf::Vertex>* vao, const f32 positions[], const uint8_t colors[], const f32 textureCoords[], const uint8_t indices[], uint8_t indexCount);
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

	void select(u64 collection, u64 stack, u64 block, u64 argument);
	void SelectContext(u64 collection, u64 stack, u64 block);
	void UnSelect();

	bool m_Selected;
	bool m_SelectedContext;
	std::shared_ptr<rhr::stack::collection> m_SelectedCollection;
	std::shared_ptr<rhr::stack::stack> m_SelectedStack;
	std::shared_ptr<rhr::stack::block> m_SelectedBlock;
	std::shared_ptr<rhr::stack::argument::argument> m_SelectedArgument;

	std::function<void(uint8_t)> m_ContextCallback;

	//////////////////////////////////////////////////////////
	// Dragging
	//////////////////////////////////////////////////////////

	void DragCollection(std::shared_ptr<rhr::stack::collection>& collection, bool up);
	void DragStack(std::shared_ptr<rhr::stack::collection>& collection, std::shared_ptr<rhr::stack::stack>& stack, bool up);
	void UnDrag(const glm::vec<2, i32>& position);
	void DraggingStackUpdate();

	bool DraggingCollection();
	bool DraggingStack();

	std::shared_ptr<rhr::stack::collection> m_DraggingCollection;
	std::shared_ptr<rhr::stack::stack> m_DraggingStack;

	glm::vec<2, i32> m_DraggingBeginObject;
	glm::vec<2, i32> m_DraggingBeginMouse;

	bool m_DraggingUp;

	//////////////////////////////////////////////////////////
	// Snapping
	//////////////////////////////////////////////////////////

	void SetSnap(std::shared_ptr<rhr::stack::collection> collection, u64 stackLoc, std::shared_ptr<rhr::stack::stack> stack);
	void ClearSnap();

	bool IsSnap();

	vui::RenderRectangle m_DraggingShape;

	std::shared_ptr<rhr::stack::collection> m_DraggingSnapCollection;
	u64 m_DraggingSnapStackLoc;
	std::shared_ptr<rhr::stack::stack> m_DraggingSnapStack;

	bool m_DraggingSnap;

	//////////////////////////////////////////////////////////
	// Other Data
	//////////////////////////////////////////////////////////

	std::vector<bool> m_CollectionVanity;
	std::vector<std::shared_ptr<rhr::stack::collection>> m_Collections;

	//glm::vec<2, i32> m_innerPosition;
	//sf::View m_view;
	//sf::RenderWindow* m_window;

	bool m_Toolbar;
	vui::RenderText m_InnerText;
	std::shared_ptr<vui::RenderRectangle> m_background;

	//const sf::Texture& m_fontTexture;
	//sf::Image m_fontEditedImage;
	//sf::Texture m_fontEditedTexture;

	//const sf::Texture& m_fontBoldTexture;
	//sf::Image m_fontBoldEditedImage;
	//sf::Texture m_fontBoldEditedTexture;
};
}
