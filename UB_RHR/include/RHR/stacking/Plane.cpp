#include "Plane.hpp"

#include "registries/ShaderRegistry.hpp"
#include "handlers/ContextHandler.hpp"

#include <Cappuccino/Utils.hpp>
#include <Cappuccino/Intrinsics.hpp>
#include <Espresso/InputHandler.hpp>

// TODO: remove include (used for testing)
#include <iostream>

#ifdef ENABLE_AVX2
#ifdef WIN32
#include <immintrin.h>
#else
// TODO add Unix intrinsics
#endif
#endif

//Plane::Plane(bool toolbar, const Plane& plane)
//{
//	Setup(toolbar);
//
//	for (uint64_t i = 0; i < m_Collections.size(); i++)
//		delete m_Collections[i];
//
//	m_Collections.clear();
//
//	for (uint64_t i = 0; i < plane.m_Collections.size(); i++)
//		m_Collections.push_back(new Collection(*plane.m_Collections[i]));
//}

Plane::Plane(bool toolbar)
	: m_Toolbar(toolbar)
	, m_Selected(false)
	, m_SelectedContext(false)
	, m_DraggingCollection(nullptr)
	, m_DraggingStack(nullptr)
	, m_DraggingUp(false)
	, m_DraggingSnap(false)
	, m_DraggingSnapCollection(0)
	, m_DraggingSnapStack(0)
{
	InputHandler::RegisterMouseCallback()

	//m_shader.loadFromFile("res/shaders/blocks.vert", "res/shaders/blocks.frag");

	//sf::Image imageCollectionOpen;
	//sf::Image imageCollectionClose;

	//imageCollectionOpen.loadFromFile("res/collection_tab_open.png");
	//imageCollectionClose.loadFromFile("res/collection_tab_closed.png");

	//imageCollectionOpen.flipVertically();
	//imageCollectionClose.flipVertically();

	//m_textureCollectionOpen.loadFromImage(imageCollectionOpen);
	//m_textureCollectionClosed.loadFromImage(imageCollectionClose);

	//m_draggingShape.setFillColor(MOD_BUTTON_TEXT_FG);

	//m_window = nullptr;

	m_Collections.reserve(5);
	m_CollectionVanity.reserve(5);

	//m_VertexArrays.reserve(5);

	// m_innerText displays the inner position of the plane

	//m_innerText = sf::Text("0, 0", Global::Font, 12);
	//m_innerText.setFillColor(MOD_BUTTON_TEXT_FG);

	// callback for the context menu

	m_ContextCallback = [](uint8_t idx)
	{
		if (idx == 0)
		{
			// stack duplicate

			//m_selectedCollection->AddStack(new Stack(*m_selectedStack));
		}
		else if (idx == 0)
		{
			// stack delete
		}
		else if (idx == 0)
		{
			// block duplicate
		}
		else if (idx == 0)
		{
			// block delete
		}
	};
}

Plane::~Plane()
{
	for (uint64_t i = 0; i < m_Collections.size(); i++)
		delete m_Collections[i];
}

void Plane::AddCollection(Collection* collection, bool displayCollectionVanity)
{
	m_Collections.push_back(collection);
	m_CollectionVanity.push_back(displayCollectionVanity);

	//CreateBuffer(m_Collections.size() - 1, displayCollectionVanity);
}

const std::vector<Collection*>& Plane::GetCollections()
{
	return m_Collections;
}

void Plane::DeleteCollection(uint64_t idx, bool dealloc)
{
	//m_VertexArrays.erase(m_vertexArrays.begin() + idx);
	//m_VertexBuffers.erase(m_vertexBuffers.begin() + idx);
	//m_VertexBufferTransform.erase(m_vertexBufferTransform.begin() + idx);
	//m_TextureMapImage.erase(m_textureMapImage.begin() + idx);
	//m_TextureMapTexture.erase(m_textureMapTexture.begin() + idx);
	//m_TextureMapEnabled.erase(m_textureMapEnabled.begin() + idx);
	m_CollectionVanity.erase(m_CollectionVanity.begin() + idx);

	if (dealloc)
		delete m_Collections[idx];

	m_Collections.erase(m_Collections.begin() + idx);
}

//void Plane::TranslateInnerPosition(const glm::vec<2, int32_t>& position)
//{
//	//m_innerPosition += position;
//	m_view.setCenter(m_view.getCenter() + (glm::vec<2, int32_t>)position);
//}
//
//void Plane::SetInnerPosition(glm::vec<2, int32_t> position)
//{
//	//m_innerPosition = position;
//	m_view.setCenter((glm::vec<2, int32_t>)position);
//}
//
//glm::vec<2, int32_t> Plane::GetInnerPosition()
//{
//	return glm::vec<2, int32_t>();
//}

void Plane::DeleteContents(bool dealloc)
{
	for (uint64_t i = 0; i < m_Collections.size(); i++)
		DeleteCollection(0, dealloc);
}

void Plane::UpdateVAOPosition(uint64_t idx)
{
	/*
	uint64_t i = 0;

	if (m_CollectionVanity[idx])
	{
		//const glm::vec<2, int32_t> pos = m_Collections[idx]->GetPosition() + GetPosition();
		const glm::vec<2, int32_t> size = (glm::vec<2, int32_t>)m_Collections[idx]->GetSize();

		m_vertexArrays[idx][i++].position = glm::vec<2, int32_t>();
		m_vertexArrays[idx][i++].position = glm::vec<2, int32_t>(size.x, COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = glm::vec<2, int32_t>(0, COLLECTION_OUTLINE_WIDTH);

		m_vertexArrays[idx][i++].position = glm::vec<2, int32_t>(size.x + COLLECTION_OUTLINE_WIDTH, 0);
		m_vertexArrays[idx][i++].position = glm::vec<2, int32_t>(size.x + COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = glm::vec<2, int32_t>();

		m_vertexArrays[idx][i++].position = glm::vec<2, int32_t>(size.x + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH, 0);
		m_vertexArrays[idx][i++].position = glm::vec<2, int32_t>(size.x + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = glm::vec<2, int32_t>(size.x + COLLECTION_OUTLINE_WIDTH, 0);

		m_vertexArrays[idx][i++].position = glm::vec<2, int32_t>(size.x + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = glm::vec<2, int32_t>(size.x + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = glm::vec<2, int32_t>(size.x + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH, 0);

		m_vertexArrays[idx][i++].position = glm::vec<2, int32_t>(size.x + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = glm::vec<2, int32_t>(COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = glm::vec<2, int32_t>(size.x + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH);

		m_vertexArrays[idx][i++].position = glm::vec<2, int32_t>(COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = glm::vec<2, int32_t>(COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = glm::vec<2, int32_t>(size.x + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH);

		m_vertexArrays[idx][i++].position = glm::vec<2, int32_t>(0, size.y + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = glm::vec<2, int32_t>(COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = glm::vec<2, int32_t>(COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH);

		m_vertexArrays[idx][i++].position = glm::vec<2, int32_t>(0, COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = glm::vec<2, int32_t>(COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = glm::vec<2, int32_t>(0, size.y + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH);

		m_vertexArrays[idx][i++].position = glm::vec<2, int32_t>(COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = glm::vec<2, int32_t>(size.x + COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = glm::vec<2, int32_t>(size.x + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH);

		m_vertexArrays[idx][i++].position = glm::vec<2, int32_t>(COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = glm::vec<2, int32_t>(size.x + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = glm::vec<2, int32_t>(COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH);
	}
	*/


	// TODO not finished; may not need
}

//void Plane::UpdateVAO(uint64_t idx)
//{
//	UpdateBuffer(idx);
//}

void Plane::frameUpdate(double deltaTime)
{
	// set innerText to a visual representation of the inner position coordinates

	if (!m_Toolbar)
	{
		m_InnerText.SetPosition({ GetPosition().x + 5, GetPosition().y + GetSize().y - 18 });
		char innerText[20];
		memset(innerText, 0, 20);
		sprintf(innerText, "test");
		// TODO display m_view.center thingy instead of innerPosition
		//sprintf(innerText, "%d, %d", GetInnerPosition().x, GetInnerPosition().y); // TODO make sure "%d" has no trailing "0"s.
		m_InnerText.SetText(innerText);
	}

	// dragging position

	if (DraggingStack() || DraggingCollection())
	{
		m_DraggingCollection->SetPosition(m_DraggingBeginObject + glm::vec<2, int32_t>(
			(InputHandler::GetMousePosition().x - m_DraggingBeginMouse.x)/* / CalculateZoom().x*/,
			(InputHandler::GetMousePosition().y - m_DraggingBeginMouse.y)/* / CalculateZoom().y*/) - GetPosition());
	}
	
	// dragging connections

	if (DraggingStack())
		DraggingStackUpdate();

	// transform applies offset to the vertex buffers

	//for (uint64_t i = 0; i < m_vertexBufferTransform.size(); i++)
	//	m_vertexBufferTransform[i] = sf::Transform().translate((glm::vec<2, int32_t>)GetPosition()).translate(m_Collections[i]->GetPosition());

	//if (DraggingStack() || DraggingCollection())
	//{
	//	glm::vec<2, int32_t> zoom = Plane::PrimaryPlane->CalculateZoom();
	//	m_vertexBufferTransform.back().scale(zoom);
	//}
}

bool Plane::mouseButton(bool down, const glm::vec<2, int32_t>& position, const sf::Mouse::Button& button)
{
	//if (m_window == nullptr)
	//	return false;

	glm::vec<2, int32_t> mmpos = position;

	uint64_t collectionMax = m_Collections.size();

	if (collectionMax == 0)
		return false;

	if ((this == Plane::ToolbarPlane && Plane::PrimaryPlane->DraggingStack()) ||
		(this == Plane::PrimaryPlane && Plane::ToolbarPlane->DraggingStack()))
		return false;

	if (DraggingStack() || DraggingCollection())
		collectionMax--;

	if (DraggingStack())
	{
		if (!down && !m_DraggingUp)
		{
			// dragging and mouse released (used when dragging a stack)
			UnDrag(position);
		}
		else if (down && m_DraggingUp)
		{
			// dragging and mouse pressed (used when duplicating stack)
			UnDrag(position);
		}
	}

	for (int64_t i = collectionMax - 1; i >= 0; i--)
	{
		if (!m_Collections[i]->GetEnabled())
			continue;

		// size
		
		glm::vec<2, int32_t> collectionSize = glm::vec<2, int32_t>(
			m_Collections[i]->GetSize().x,
			m_Collections[i]->GetSize().y
		);
		
		// position

		glm::vec<2, int32_t> collectionPosition = glm::vec<2, int32_t>(
			(m_Collections[i]->GetPosition().x) + GetPosition().x,// - GetInnerPosition().x,
			(m_Collections[i]->GetPosition().y) + GetPosition().y// - GetInnerPosition().y
		);

		if (mmpos.x > collectionPosition.x && mmpos.x < collectionPosition.x + collectionSize.x &&
			mmpos.y > collectionPosition.y && mmpos.y < collectionPosition.y + collectionSize.y)
		{
			//for (int64_t a = 0; a < m_Collections[i]->GetStacks().size(); a++)
			//for (int64_t a = m_Collections[i]->GetStacks().size() - 1; a >= 0; a--)
			for (int64_t a = m_Collections[i]->GetStacks().size() - 1; a >= 0; a--)
			{
				// size

				glm::vec<2, int32_t> stackSize;

				for (uint64_t b = 0; b < m_Collections[i]->GetStacks()[a]->GetBlocks().size(); b++)
				{
					if (m_Collections[i]->GetStacks()[a]->GetBlocks()[b]->GetWidth() > stackSize.x)
						stackSize.x = m_Collections[i]->GetStacks()[a]->GetBlocks()[b]->GetWidth();
				}

				stackSize.y = m_Collections[i]->GetStacks()[a]->GetBlocks().size() * Block::Height;
				stackSize = glm::vec<2, int32_t>(stackSize.x, stackSize.y);

				// position

				glm::vec<2, int32_t> stackPosition = glm::vec<2, int32_t>(
					m_Collections[i]->GetStacks()[a]->GetPosition().x,
					m_Collections[i]->GetStacks()[a]->GetPosition().y);

				stackPosition += collectionPosition;

				if (mmpos.x > stackPosition.x && mmpos.x < stackPosition.x + stackSize.x &&
					mmpos.y > stackPosition.y && mmpos.y < stackPosition.y + stackSize.y)
				{
					for (uint64_t b = 0; b < m_Collections[i]->GetStacks()[a]->GetBlocks().size(); b++)
					{
						// size

						glm::vec<2, int32_t> blockSize;

						blockSize.x = m_Collections[i]->GetStacks()[a]->GetBlocks()[b]->GetWidth();
						blockSize.y = Block::Height;
						blockSize = glm::vec<2, int32_t>(blockSize.x, blockSize.y);

						// position

						glm::vec<2, int32_t> blockPosition = glm::vec<2, int32_t>(stackPosition.x, Block::Height * b + stackPosition.y);

						if (mmpos.x >= blockPosition.x && mmpos.x < blockPosition.x + blockSize.x &&
							mmpos.y >= blockPosition.y && mmpos.y < blockPosition.y + blockSize.y)
						{
							if (down && !DraggingStack())
							{
								// not dragging and mouse down

								if (button == sf::Mouse::Button::Left)
								{
									// remove context menu

									//ContextHandler::Disable();
									UnSelect();

									Collection* activeCollection = new Collection();
									Stack* activeStack = m_Collections[i]->GetStacks()[a];

									if (b > 0)
									{
										// create stack that is left behind (no the one picked up)

										Stack* leftStack = new Stack();
										leftStack->SetPosition(activeStack->GetPosition());
										
										for (uint64_t c = 0; c < b; c++)
											leftStack->AddBlock(m_Collections[i]->GetStacks()[a]->GetBlocks()[c]);

										for (uint64_t c = 0; c < b; c++)
											activeStack->RemoveBlock(0);

										m_Collections[i]->AddStack(leftStack);
									}
									activeCollection->SetPosition((m_Collections[i]->GetPosition()) + glm::vec<2, int32_t>(0, b * Block::Height) + ((glm::vec<2, int32_t>)activeStack->GetPosition()));
									activeCollection->SetSize(glm::vec<2, int32_t>(activeStack->GetWidestBlock(), activeStack->GetBlocks().size() * Block::Height));

									activeStack->SetPosition({ 0, 0 });

									// register stack and collection

									m_Collections[i]->RemoveStack(a);
									activeCollection->AddStack(activeStack);
									AddCollection(activeCollection, false);

									// update buffer that was taken from

									if (m_Collections[i]->GetStacks().size() > 0)
									{
										//UpdateBuffer(i);

									}
									else
										DeleteCollection(i, true);

									// drag the current stack (excludes the stack that was left behind)

									DragStack(activeCollection, activeStack, false);
								}
								else if (button == sf::Mouse::Button::Right)
								{
									Logger::Debug("opening context menu");

									// startup context menu on block

									//ContextHandler::Disable();
									//ContextHandler::Enable(mmpos + glm::vec<2, int32_t>(5, 5), &m_contextCallback);
									SelectContext(i, a, b);
								}
							}

							// if block was bounded

							return true;
						}
					}

					// if stack was bounded

					break;
				}
			}

			// if collection was bounded

			break;
		}
	}

	return false;
}

Plane* Plane::PrimaryPlane;
Plane* Plane::ToolbarPlane;

//void Plane::render(sf::RenderWindow& window)
//{
	//m_background.setFillColor(MOD_BACKGROUND_LOW);
	//m_background.SetPosition(GetPosition());
	//m_background.SetSize((glm::vec<2, int32_t>)GetSize());

	//window.setView(window.getDefaultView());
	//window.draw(m_background);

	//uint16_t collectionMax = m_Collections.size();

	//if (collectionMax == 0)
	//	return;

	//if (DraggingStack() || DraggingCollection())
	//	collectionMax--;

	//// render each batch collection

	//window.setView(m_view);

	//for (uint16_t i = 0; i < collectionMax; i++)
	//{
	//	if (!m_Collections[i]->GetEnabled())
	//		continue;

	//	// setup render

	//	sf::RenderStates states;
	//	states.transform = m_vertexBufferTransform[i];
	//	//states.transform.scale(glm::vec<2, int32_t>(Global::BlockZoom, Global::BlockZoom), GetPosition());//, /*glm::vec<2, int32_t>(GetSize().x / 2.0f, GetSize().y / 2.0f) - */-(m_Collections[i]->GetPosition() * Global::BlockZoom));
	//	//sf::Texture re;
	//	//re.loadFromFile("res/grade.png");
	//	//if (m_textureMapEnabled[i])
	//	if (CalculateZoom().x < 0.95f)
	//		m_shader.setUniform("texture", /*TEST_TEXTURE*//*m_textureMapTexture[i]*//*re*/m_fontBoldEditedTexture);
	//	else
	//		m_shader.setUniform("texture", /*TEST_TEXTURE*//*m_textureMapTexture[i]*//*re*/m_fontEditedTexture);

	//	states.shader = &m_shader;

	//	// render VBO
	//	window.draw(m_vertexBuffers[i], states);
	//}

	//window.setView(window.getDefaultView());

	//// render inner position coordinates

	////window.draw(m_innerText);
	//m_window = &window;
//}

//void Plane::snapRender(sf::RenderWindow& window)
//{
	// snap graphic

	//if (IsSnap())
	//{
	//	RevertWindowView rwv(&window);
	//	window.setView(m_view);

	//	m_draggingShape.SetPosition(
	//		Plane::PrimaryPlane->GetPosition() +
	//		Plane::PrimaryPlane->GetCollections()[m_draggingSnapCollection]->GetPosition() +
	//		m_draggingSnapStack->GetPosition() +
	//		glm::vec<2, int32_t>(0, m_draggingSnapStackLoc * Block::Height)// -
	//		//(glm::vec<2, int32_t>)Plane::PrimaryPlane->GetInnerPosition()
	//	);

	//	uint64_t refIdx = 0;

	//	if (m_draggingSnapStackLoc > 0)
	//		refIdx = m_draggingSnapStackLoc - 1;

	//	m_draggingShape.SetSize(glm::vec<2, int32_t>(m_draggingSnapStack->GetBlocks()[refIdx]->GetWidth(), SNAP_GRAPHIC_HEIGHT));

	//	window.draw(m_draggingShape);
	//}
//}

//void Plane::postRender(sf::RenderWindow& window)
//{
	//if (DraggingCollection())
	//{

	//}
	//else if (DraggingStack())
	//{
	//	sf::RenderStates states;
	//	states.transform = m_vertexBufferTransform.back();
	//	//states.transform.translate((glm::vec<2, int32_t>)GetInnerPosition());

	//	if (m_textureMapEnabled.back())
	//		m_shader.setUniform("texture", m_textureMapTexture.back());

	//	states.shader = &m_shader;
	//	
	//	window.setView(window.getDefaultView());
	//	window.draw(m_vertexBuffers.back(), states);
	//	window.setView(window.getDefaultView());
	//}
//}

//void Plane::Setup(bool toolbar)
//{
//	m_toolbar = toolbar;
//	m_Selected = false;
//	m_SelectedContext = false;
//
//	m_DraggingCollection = nullptr;
//	m_DraggingStack = nullptr;
//	m_DraggingUp = false;
//
//	//m_shader.loadFromFile("res/shaders/blocks.vert", "res/shaders/blocks.frag");
//
//	//sf::Image imageCollectionOpen;
//	//sf::Image imageCollectionClose;
//
//	//imageCollectionOpen.loadFromFile("res/collection_tab_open.png");
//	//imageCollectionClose.loadFromFile("res/collection_tab_closed.png");
//
//	//imageCollectionOpen.flipVertically();
//	//imageCollectionClose.flipVertically();
//
//	//m_textureCollectionOpen.loadFromImage(imageCollectionOpen);
//	//m_textureCollectionClosed.loadFromImage(imageCollectionClose);
//
//	//m_draggingShape.setFillColor(MOD_BUTTON_TEXT_FG);
//
//	m_DraggingSnap = false;
//	m_DraggingSnapCollection = 0;
//	m_DraggingSnapStackLoc = 0;
//	m_DraggingSnapStack = nullptr;
//
//	m_window = nullptr;
//}

//void Plane::UpdateCollectionVAO(std::vector<sf::Vertex>* vao, glm::vec<2, int32_t> size)
//{
//	// ====================================================================================================================================================
//	// =============== add vao; see "dev/collection_geometry.png"
//	// ====================================================================================================================================================
//
//	float positions[] = {
//		// 0
//		0.0f, 0.0f,
//		(COLLECTION_TAB_PART * COLLECTION_TAB_PART_COUNT) + COLLECTION_OUTLINE_WIDTH, 0.0f,
//		(COLLECTION_TAB_PART * COLLECTION_TAB_PART_COUNT) + (COLLECTION_OUTLINE_WIDTH * 2), 0.0f,
//		(COLLECTION_TAB_PART * COLLECTION_TAB_PART_COUNT) + (COLLECTION_OUTLINE_WIDTH * 2), COLLECTION_TAB_PART,
//		static_cast<float>(size.x + COLLECTION_OUTLINE_WIDTH), COLLECTION_TAB_PART,
//		// 5
//		static_cast<float>(size.x + (COLLECTION_OUTLINE_WIDTH * 2)), COLLECTION_TAB_PART,
//		static_cast<float>(size.x + (COLLECTION_OUTLINE_WIDTH * 2)), static_cast<float>(COLLECTION_TAB_PART + size.y + COLLECTION_OUTLINE_WIDTH),
//		static_cast<float>(size.x + (COLLECTION_OUTLINE_WIDTH * 2)), static_cast<float>(COLLECTION_TAB_PART + size.y + (COLLECTION_OUTLINE_WIDTH * 2)),
//		COLLECTION_OUTLINE_WIDTH, static_cast<float>(COLLECTION_TAB_PART + size.y + (COLLECTION_OUTLINE_WIDTH * 2)),
//		0.0f, static_cast<float>(COLLECTION_TAB_PART + size.y + (COLLECTION_OUTLINE_WIDTH * 2)),
//		// 10
//		0.0f, COLLECTION_OUTLINE_WIDTH,
//		COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH,
//		COLLECTION_OUTLINE_WIDTH + (COLLECTION_TAB_PART * COLLECTION_TAB_PART_COUNT), COLLECTION_OUTLINE_WIDTH,
//		COLLECTION_OUTLINE_WIDTH + (COLLECTION_TAB_PART * COLLECTION_TAB_PART_COUNT), COLLECTION_TAB_PART,
//		COLLECTION_OUTLINE_WIDTH + (COLLECTION_TAB_PART * COLLECTION_TAB_PART_COUNT), COLLECTION_TAB_PART + COLLECTION_OUTLINE_WIDTH,
//		// 15
//		static_cast<float>(size.x + COLLECTION_OUTLINE_WIDTH), COLLECTION_TAB_PART + COLLECTION_OUTLINE_WIDTH,
//		static_cast<float>(size.x + COLLECTION_OUTLINE_WIDTH), static_cast<float>(COLLECTION_TAB_PART + COLLECTION_OUTLINE_WIDTH + size.y),
//		COLLECTION_OUTLINE_WIDTH, static_cast<float>(COLLECTION_TAB_PART + COLLECTION_OUTLINE_WIDTH + size.y),
//		COLLECTION_OUTLINE_WIDTH, COLLECTION_TAB_PART + COLLECTION_OUTLINE_WIDTH,
//		// 19 (tab texture)
//		COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH,
//		COLLECTION_OUTLINE_WIDTH + (COLLECTION_TAB_PART * COLLECTION_TAB_PART_COUNT), COLLECTION_OUTLINE_WIDTH,
//		COLLECTION_OUTLINE_WIDTH + (COLLECTION_TAB_PART * COLLECTION_TAB_PART_COUNT), COLLECTION_TAB_PART + COLLECTION_OUTLINE_WIDTH,
//		COLLECTION_OUTLINE_WIDTH, COLLECTION_TAB_PART + COLLECTION_OUTLINE_WIDTH,
//		// 23 (background)
//		COLLECTION_OUTLINE_WIDTH, COLLECTION_TAB_PART + COLLECTION_OUTLINE_WIDTH,
//		static_cast<float>(size.x + COLLECTION_OUTLINE_WIDTH), COLLECTION_TAB_PART + COLLECTION_OUTLINE_WIDTH,
//		static_cast<float>(size.x + COLLECTION_OUTLINE_WIDTH), static_cast<float>(COLLECTION_TAB_PART + COLLECTION_OUTLINE_WIDTH + size.y),
//		COLLECTION_OUTLINE_WIDTH, static_cast<float>(COLLECTION_TAB_PART + COLLECTION_OUTLINE_WIDTH + size.y),
//		// 27 (tab background)
//		COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH,
//		COLLECTION_OUTLINE_WIDTH + (COLLECTION_TAB_PART * COLLECTION_TAB_PART_COUNT), COLLECTION_OUTLINE_WIDTH,
//		COLLECTION_OUTLINE_WIDTH + (COLLECTION_TAB_PART * COLLECTION_TAB_PART_COUNT), COLLECTION_TAB_PART + COLLECTION_OUTLINE_WIDTH,
//		COLLECTION_OUTLINE_WIDTH, COLLECTION_TAB_PART + COLLECTION_OUTLINE_WIDTH,
//	};
//
//	const uint8_t colors[] = {
//		// 0
//		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
//		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
//		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
//		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
//		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
//		// 5
//		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
//		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
//		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
//		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
//		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
//		// 10
//		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
//		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
//		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
//		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
//		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
//		// 15
//		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
//		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
//		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
//		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
//		// 19 (tab texture)
//		255, 255, 255,
//		255, 255, 255,
//		255, 255, 255,
//		255, 255, 255,
//		// 23 (background)
//		COLLECTION_COLOR_FILL_R, COLLECTION_COLOR_FILL_G, COLLECTION_COLOR_FILL_B,
//		COLLECTION_COLOR_FILL_R, COLLECTION_COLOR_FILL_G, COLLECTION_COLOR_FILL_B,
//		COLLECTION_COLOR_FILL_R, COLLECTION_COLOR_FILL_G, COLLECTION_COLOR_FILL_B,
//		COLLECTION_COLOR_FILL_R, COLLECTION_COLOR_FILL_G, COLLECTION_COLOR_FILL_B,
//		// 27 (tab background)
//		COLLECTION_COLOR_FILL_R, COLLECTION_COLOR_FILL_G, COLLECTION_COLOR_FILL_B,
//		COLLECTION_COLOR_FILL_R, COLLECTION_COLOR_FILL_G, COLLECTION_COLOR_FILL_B,
//		COLLECTION_COLOR_FILL_R, COLLECTION_COLOR_FILL_G, COLLECTION_COLOR_FILL_B,
//		COLLECTION_COLOR_FILL_R, COLLECTION_COLOR_FILL_G, COLLECTION_COLOR_FILL_B,
//	};
//
//	const float textureCoords[] = {
//		// 0
//		0, 0,
//		0, 0,
//		0, 0,
//		0, 0,
//		0, 0,
//		// 5
//		0, 0,
//		0, 0,
//		0, 0,
//		0, 0,
//		0, 0,
//		// 10
//		0, 0,
//		0, 0,
//		0, 0,
//		0, 0,
//		0, 0,
//		// 15
//		0, 0,
//		0, 0,
//		0, 0,
//		0, 0,
//		// 19 (tab texture)
//		1, 1,
//		91, 1,
//		91, 31,
//		1, 31,
//		// 23 (background)
//		0, 0,
//		0, 0,
//		0, 0,
//		0, 0,
//		// 27 (tab background)
//		0, 0,
//		0, 0,
//		0, 0,
//		0, 0,
//	};
//
//	const uint8_t indices[] = {
//		// 0
//		0, 12, 10,
//		1, 12, 0,
//		2, 3, 1,
//		13, 12, 3,
//		14, 13, 15,
//		// 5
//		4, 15, 13,
//		5, 6, 4,
//		6, 16, 15,
//		7, 17, 16,
//		8, 17, 7,
//		// 10
//		9, 10, 8,
//		10, 11, 8,
//		11, 14, 18,
//		11, 12, 14,
//		18, 16, 17,
//		// 15
//		18, 15, 16,
//		// 16 (background)
//		23, 25, 26,
//		23, 24, 25,
//		// 18 (tab background)
//		27, 29, 30,
//		27, 28, 29,
//		// 20 (tab texture)
//		19, 21, 22,
//		19, 20, 21,
//	};
//
//#ifdef ENABLE_AVX2
//	{
//		__m256 _positions, _op, _result;
//
//		_op = _mm256_set_ps(
//			COLLECTION_OUTLINE_WIDTH,
//			COLLECTION_OUTLINE_WIDTH + COLLECTION_TAB_PART,
//			COLLECTION_OUTLINE_WIDTH,
//			COLLECTION_OUTLINE_WIDTH + COLLECTION_TAB_PART,
//			COLLECTION_OUTLINE_WIDTH,
//			COLLECTION_OUTLINE_WIDTH + COLLECTION_TAB_PART,
//			COLLECTION_OUTLINE_WIDTH,
//			COLLECTION_OUTLINE_WIDTH + COLLECTION_TAB_PART);
//
//		for (uint8_t i = 0; i < 7; i++)
//		{
//			_positions = _mm256_set_ps(
//				positions[i * 8 + 0], positions[i * 8 + 1], positions[i * 8 + 2], positions[i * 8 + 3],
//				positions[i * 8 + 4], positions[i * 8 + 5], positions[i * 8 + 6], positions[i * 8 + 7]);
//
//			_result = _mm256_sub_ps(_positions, _op);
//
//#ifdef WIN32
//			positions[i * 8 + 0] = _result.m256_f32[7];
//			positions[i * 8 + 1] = _result.m256_f32[6];
//			positions[i * 8 + 2] = _result.m256_f32[5];
//			positions[i * 8 + 3] = _result.m256_f32[4];
//
//			positions[i * 8 + 4] = _result.m256_f32[3];
//			positions[i * 8 + 5] = _result.m256_f32[2];
//			positions[i * 8 + 6] = _result.m256_f32[1];
//			positions[i * 8 + 7] = _result.m256_f32[0];
//#else
//			positions[i * 8 + 0] = _result[7];
//			positions[i * 8 + 1] = _result[6];
//			positions[i * 8 + 2] = _result[5];
//			positions[i * 8 + 3] = _result[4];
//
//			positions[i * 8 + 4] = _result[3];
//			positions[i * 8 + 5] = _result[2];
//			positions[i * 8 + 6] = _result[1];
//			positions[i * 8 + 7] = _result[0];
//#endif
//		}
//
//		_positions = _mm256_set_ps(
//			positions[56], positions[57], positions[58], positions[59],
//			positions[60], positions[61], COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH + COLLECTION_TAB_PART);
//
//		_result = _mm256_sub_ps(_positions, _op);
//
//#ifdef WIN32
//		positions[56] = _result.m256_f32[0];
//		positions[57] = _result.m256_f32[1];
//		positions[58] = _result.m256_f32[2];
//		positions[59] = _result.m256_f32[3];
//
//		positions[60] = _result.m256_f32[4];
//		positions[61] = _result.m256_f32[5];
//#else
//		positions[56] = _result[0];
//		positions[57] = _result[1];
//		positions[58] = _result[2];
//		positions[59] = _result[3];
//
//		positions[60] = _result[4];
//		positions[61] = _result[5];
//#endif
//	}
//#else
//	{
//		for (uint8_t i = 0; i < 31 * 2; i++)
//		{
//			if (i % 2 == 0)
//				positions[i] -= COLLECTION_OUTLINE_WIDTH;
//			else
//				positions[i] -= COLLECTION_OUTLINE_WIDTH + COLLECTION_TAB_PART;
//		}
//	}
//#endif
//
//	vao->clear();
//	vao->reserve(22 * 3);
//
//	ParseIndices(vao, positions, colors, textureCoords, indices, 22);
//}

//void Plane::CreateBuffer(uint16_t collectionIdx, bool displayCollectionVanity)
//{
//	// Create VAO
//
//	m_vertexArrays.push_back(std::vector<sf::Vertex>());
//	m_vertexArrays.back().reserve(200);
//
//	// Create VBO
//	
//	// "Triangles" because we are batch rendering and can not use the other optimized connected primitive types
//	// "Dynamic" because "Occasionally changing data", the buffer will only be changed during a user induced event
//	m_vertexBuffers.push_back(sf::VertexBuffer(sf::PrimitiveType::Triangles, sf::VertexBuffer::Usage::Dynamic));
//
//	m_vertexBufferTransform.push_back(sf::Transform());
//	m_textureMapImage.push_back(sf::Image());
//	m_textureMapTexture.push_back(sf::Texture());
//	m_textureMapEnabled.push_back(false);
//
//	UpdateBuffer(collectionIdx);
//}

//void Plane::UpdateBuffer(uint16_t bufferIdx)
//{
//	// update font image
//
//	m_fontEditedImage = m_fontTexture.copyToImage();
//	m_fontEditedImage.setPixel(0, 0, sf::Color::White);
//	m_fontEditedTexture.loadFromImage(m_fontEditedImage);
//
//	m_fontBoldEditedImage = m_fontBoldTexture.copyToImage();
//	m_fontBoldEditedImage.setPixel(0, 0, sf::Color::White);
//	m_fontBoldEditedTexture.loadFromImage(m_fontBoldEditedImage);
//
//	//TEST_SPRITE.setTexture(m_fontBoldEditedTexture, true);
//
//	// clean and update collection VAO
//	
//	m_vertexArrays[bufferIdx].clear();
//
//	if (m_CollectionVanity[bufferIdx])
//		UpdateCollectionVAO(&(m_vertexArrays[bufferIdx]), m_Collections[bufferIdx]->GetSize());
//
//	// pull vertex buffers from children
//
//	std::vector<sf::VertexArray> va;
//	std::vector<std::vector<bool>*> vaChar;
//	//std::vector<const sf::Image*> vaTextures;
//	std::vector<std::vector<sf::Vertex>> vaBox;
//	std::vector<uint16_t> vaSize;
//
//	for (uint64_t a = 0; a < m_Collections[bufferIdx]->GetStacks().size(); a++)
//	{
//		for (uint64_t b = 0; b < m_Collections[bufferIdx]->GetStacks()[a]->GetBlocks().size(); b++)
//		{
//			vaBox.push_back(std::vector<sf::Vertex>());
//			uint16_t calcSize = 0;
//
//			// add block vertices
//
//			float blockWidth = static_cast<float>(Block::Padding);
//
//			glm::vec<2, int32_t> blockPos = m_Collections[bufferIdx]->GetStacks()[a]->GetPosition() + glm::vec<2, int32_t>(0, b * Block::Height);
//			uint32_t blockShellWidth = m_Collections[bufferIdx]->GetStacks()[a]->GetBlocks()[b]->GetWidth();
//			const sf::Color blockColor = m_Collections[bufferIdx]->GetStacks()[a]->GetBlocks()[b]->GetModCategory()->GetColor();
//
//			vaBox.back().push_back(sf::Vertex(blockPos + glm::vec<2, int32_t>(0, 0), blockColor, glm::vec<2, int32_t>(0, 0)));
//			vaBox.back().push_back(sf::Vertex(blockPos + glm::vec<2, int32_t>(blockShellWidth, 0), blockColor, glm::vec<2, int32_t>(0, 0)));
//			vaBox.back().push_back(sf::Vertex(blockPos + glm::vec<2, int32_t>(blockShellWidth, Block::Height), blockColor, glm::vec<2, int32_t>(0, 0)));
//
//			vaBox.back().push_back(sf::Vertex(blockPos + glm::vec<2, int32_t>(0, 0), blockColor, glm::vec<2, int32_t>(0, 0)));
//			vaBox.back().push_back(sf::Vertex(blockPos + glm::vec<2, int32_t>(blockShellWidth, Block::Height), blockColor, glm::vec<2, int32_t>(0, 0)));
//			vaBox.back().push_back(sf::Vertex(blockPos + glm::vec<2, int32_t>(0, Block::Height), blockColor, glm::vec<2, int32_t>(0, 0)));
//
//			// get argument information
//			
//			for (uint64_t c = 0; c < m_Collections[bufferIdx]->GetStacks()[a]->GetBlocks()[b]->GetArguments().size(); c++)
//			{
//				Argument* arg = m_Collections[bufferIdx]->GetStacks()[a]->GetBlocks()[b]->GetArguments()[c];
//
//				calcSize++;
//
//				va.push_back(*arg->GetVAO().GetVertexArray());
//				vaChar.push_back(arg->GetVAO().GetChar());
//
//				for (uint64_t d = 0; d < va.back().getVertexCount(); d++)
//				{
//					if (vaChar.back()->at(d))
//					{
//						Global::FontBold.getGlyph(va.back()[d].texCoords.x, Block::Height * 2, false);
//						sf::Glyph gl = Global::Font.getGlyph(va.back()[d].texCoords.x, Block::Height * 2, false);
//						
//						// make char texture coordinate match the charsheet
//
//						va.back()[d + 0].texCoords = glm::vec<2, int32_t>(gl.textureRect.left, gl.textureRect.top);
//						va.back()[d + 1].texCoords = glm::vec<2, int32_t>(gl.textureRect.left + gl.textureRect.width, gl.textureRect.top);
//						va.back()[d + 2].texCoords = glm::vec<2, int32_t>(gl.textureRect.left + gl.textureRect.width, gl.textureRect.top + gl.textureRect.height);
//						va.back()[d + 3].texCoords = glm::vec<2, int32_t>(gl.textureRect.left, gl.textureRect.top);
//						va.back()[d + 4].texCoords = glm::vec<2, int32_t>(gl.textureRect.left + gl.textureRect.width, gl.textureRect.top + gl.textureRect.height);
//						va.back()[d + 5].texCoords = glm::vec<2, int32_t>(gl.textureRect.left, gl.textureRect.top + gl.textureRect.height);
//
//						va.back()[d].position += glm::vec<2, int32_t>(
//							static_cast<float>(blockWidth) + blockPos.x,
//							blockPos.y + ((float)Block::Padding / 2.0f)
//						);
//					}
//					else
//					{
//						// make VAO absolute
//						va.back()[d].position += glm::vec<2, int32_t>(
//							static_cast<float>(blockWidth) + blockPos.x,
//							blockPos.y + ((float)Block::Padding / 2.0f)
//						);
//					}
//				}
//
//				blockWidth += static_cast<float>(arg->GetWidth());
//				blockWidth += static_cast<float>(Block::Padding);
//			}
//
//			vaSize.push_back(calcSize);
//
//			// for every block
//		}
//
//		// for every stack
//	}
//
//	if (m_CollectionVanity[bufferIdx])
//	{
//		// TODO replace with intrinsic functions
//
//		for (uint8_t i = 0; i < 6; i++)
//			m_vertexArrays[bufferIdx][20 * 3 + i].texCoords.y += m_fontEditedImage.GetSize().y;
//
//		for (uint8_t a = 0; a < 22 * 3; a++)
//		{
//			m_vertexArrays[bufferIdx][a].texCoords = glm::vec<2, int32_t>(
//				m_vertexArrays[bufferIdx][a].texCoords.x / static_cast<float>(m_fontEditedImage.GetSize().x),
//				m_vertexArrays[bufferIdx][a].texCoords.y / static_cast<float>(m_fontEditedImage.GetSize().y)
//			);
//		}
//	}
//
//	// normalize
//
//	for (uint64_t a = 0; a < va.size(); a++)
//	{
//		for (uint64_t b = 0; b < va[a].getVertexCount(); b++)
//		{
//			va[a][b].texCoords = glm::vec<2, int32_t>(
//				va[a][b].texCoords.x / static_cast<float>(m_fontEditedImage.GetSize().x),
//				va[a][b].texCoords.y / static_cast<float>(m_fontEditedImage.GetSize().y)
//			);
//		}
//	}
//
//	// normalize
//
//	for (uint64_t a = 0; a < vaBox.size(); a++)
//	{
//		for (uint8_t b = 0; b < 6; b++)
//		{
//			vaBox[a][b].texCoords = glm::vec<2, int32_t>(
//				vaBox[a][b].texCoords.x / static_cast<float>(m_fontEditedImage.GetSize().x),
//				vaBox[a][b].texCoords.y / static_cast<float>(m_fontEditedImage.GetSize().y)
//			);
//			//m_vertexArrays[bufferIdx].push_back(vaBox[a][b]);
//		}
//	}
//
//	// add vertices
//
//	uint32_t fullSize = 0;
//
//	for (uint64_t i = 0; i < vaSize.size(); i++)
//	{
//		// add block vertices to VAO
//
//		for (uint8_t a = 0; a < 6; a++)
//			m_vertexArrays[bufferIdx].push_back(vaBox[i][a]);
//
//		// add block argument vertices to VAO
//
//		for (uint16_t a = 0; a < vaSize[i]; a++)
//		{
//			for (uint16_t b = 0; b < va[fullSize].getVertexCount(); b++)
//				m_vertexArrays[bufferIdx].push_back(va[fullSize][b]);
//
//			fullSize++;
//		}
//	}
//
//	// update collection VBO
//
//	m_vertexBuffers[bufferIdx].create(m_vertexArrays[bufferIdx].size());
//	m_vertexBuffers[bufferIdx].update(&(m_vertexArrays[bufferIdx][0]), m_vertexArrays[bufferIdx].size(), 0);
//}

//void Plane::ParseIndices(std::vector<sf::Vertex>* vao, const float positions[], const uint8_t colors[], const float textureCoords[], const uint8_t indices[], uint8_t indexCount)
//{
//	for (uint8_t i = 0; i < indexCount; i++)
//	{
//		uint8_t idx0 = indices[i * 3];
//
//		vao->push_back(sf::Vertex(glm::vec<2, int32_t>(positions[idx0 * 2], positions[idx0 * 2 + 1]),
//			sf::Color(colors[idx0 * 3], colors[idx0 * 3 + 1], colors[idx0 * 3 + 2]),
//			glm::vec<2, int32_t>(textureCoords[idx0 * 2], textureCoords[idx0 * 2 + 1])));
//
//		uint8_t idx1 = indices[i * 3 + 1];
//
//		vao->push_back(sf::Vertex(glm::vec<2, int32_t>(positions[idx1 * 2], positions[idx1 * 2 + 1]),
//			sf::Color(colors[idx1 * 3], colors[idx1 * 3 + 1], colors[idx1 * 3 + 2]),
//			glm::vec<2, int32_t>(textureCoords[idx1 * 2], textureCoords[idx1 * 2 + 1])));
//
//		uint8_t idx2 = indices[i * 3 + 2];
//
//		vao->push_back(sf::Vertex(glm::vec<2, int32_t>(positions[idx2 * 2], positions[idx2 * 2 + 1]),
//			sf::Color(colors[idx2 * 3], colors[idx2 * 3 + 1], colors[idx2 * 3 + 2]),
//			glm::vec<2, int32_t>(textureCoords[idx2 * 2], textureCoords[idx2 * 2 + 1])));
//	}
//}

void Plane::Select(uint64_t collection, uint64_t stack, uint64_t block, uint64_t argument)
{
	m_Selected = true;

	m_SelectedCollection = m_Collections[collection];
	m_SelectedStack = m_Collections[collection]->GetStacks()[stack];
	m_SelectedBlock = m_Collections[collection]->GetStacks()[stack]->GetBlocks()[block];
	m_SelectedArgument = m_Collections[collection]->GetStacks()[stack]->GetBlocks()[block]->GetArguments()[argument];
}

void Plane::SelectContext(uint64_t collection, uint64_t stack, uint64_t block)
{
	m_Selected = true;

	m_SelectedCollection = m_Collections[collection];
	m_SelectedStack = m_Collections[collection]->GetStacks()[stack];
	m_SelectedBlock = m_Collections[collection]->GetStacks()[stack]->GetBlocks()[block];
	m_SelectedArgument = nullptr;
}

void Plane::UnSelect()
{
	m_Selected = false;

	m_SelectedCollection = nullptr;
	m_SelectedStack = nullptr;
	m_SelectedBlock = nullptr;
	m_SelectedArgument = nullptr;
}

void Plane::DragCollection(Collection* collection, bool up)
{
	if (m_DraggingStack != nullptr || m_DraggingCollection != nullptr)
		return;

	m_DraggingUp = up;

	m_DraggingCollection = collection;
	m_DraggingStack = nullptr;

	m_DraggingBeginMouse = InputHandler::GetMousePosition();
	m_DraggingBeginObject = m_DraggingCollection->GetPosition();
}

void Plane::DragStack(Collection* collection, Stack* stack, bool up)
{
	if (m_DraggingStack != nullptr || m_DraggingCollection != nullptr)
		return;

	m_DraggingUp = up;

	m_DraggingCollection = collection;
	m_DraggingStack = stack;
	//collection->SetPosition(collection->GetPosition() - (glm::vec<2, int32_t>)GetInnerPosition());
	//collection->SetPosition(
	//	(glm::vec<2, int32_t>)m_window->mapPixelToCoords((glm::vec<2, int32_t>)collection->GetPosition(), m_view)// get coords
	//);
	//collection->SetPosition(collection->GetPosition().x * CalculateZoom().x, collection->GetPosition().y * CalculateZoom().y);
	m_DraggingBeginMouse = InputHandler::GetMousePosition();
	m_DraggingBeginObject = collection->GetPosition();
	//m_DraggingBeginObject = (glm::vec<2, int32_t>)m_window->mapCoordsToPixel(collection->GetPosition()/*(collection->GetPosition() + stack->GetPosition()) * CalculateZoom().x*/ 
	//	//- (glm::vec<2, int32_t>)((glm::vec<2, int32_t>(1.0f, 1.0f) - Plane::PrimaryPlane->CalculateZoom()) * Plane::PrimaryPlane->GetPosition())
	//	+ GetPosition()
	//	, m_view);
	//std::cout << "mapped: " << m_window->mapCoordsToPixel(collection->GetPosition(), m_view).x << std::endl;
	std::cout << "col pos: " << collection->GetPosition().x << std::endl;

	// + ((glm::vec<2, int32_t>(1.0f, 1.0f) - CalculateZoom()) * Plane::PrimaryPlane->GetPosition())
	
	//m_draggingCollection->SetPosition(
	//	m_window->mapPixelToCoords((glm::vec<2, int32_t>)(((glm::vec<2, int32_t>)position - (glm::vec<2, int32_t>)Plane::PrimaryPlane->GetPosition()))
	//		- (glm::vec<2, int32_t>)(((glm::vec<2, int32_t>)m_draggingBeginMouse - m_draggingBeginObject))
	//		+ (glm::vec<2, int32_t>)((glm::vec<2, int32_t>(1.0f, 1.0f) - CalculateZoom()) * Plane::PrimaryPlane->GetPosition())
	//		, *Plane::PrimaryPlane->GetView())
	//	- glm::vec<2, int32_t>(COLLECTION_EMPTY_SPACE, COLLECTION_EMPTY_SPACE)
	//);
	

	std::cout << "bObject " << m_DraggingBeginObject.x << ", " << m_DraggingBeginObject.y << std::endl;
	std::cout << "bMouse " << m_DraggingBeginMouse.x << ", " << m_DraggingBeginMouse.y << std::endl;

	//m_draggingBeginMouse += (glm::vec<2, int32_t>)(((glm::vec<2, int32_t>)m_draggingBeginMouse - m_draggingBeginObject) * (CalculateZoom() - glm::vec<2, int32_t>(1.0f, 1.0f)));// add offset because of the blocks scales down

	//std::cout << "center " << m_view.getCenter().x << std::endl;
}

void Plane::UnDrag(const glm::vec<2, int32_t>& position)
{
	if (DraggingStack())
	{
		glm::vec<2, int32_t> pixelPosition = m_DraggingCollection->GetPosition() + GetPosition();

		if (!(pixelPosition.x >= Plane::PrimaryPlane->GetPosition().x && pixelPosition.x < Plane::PrimaryPlane->GetSize().x + Plane::PrimaryPlane->GetPosition().x &&
			pixelPosition.y >= Plane::PrimaryPlane->GetPosition().y && pixelPosition.y < Plane::PrimaryPlane->GetSize().y + Plane::PrimaryPlane->GetPosition().y))
			return;

		if (IsSnap())
		{
			m_DraggingSnapStack->InsertBlocks(m_DraggingStack->GetBlocks(), m_DraggingSnapStackLoc);
			m_DraggingCollection->RemoveAll(false);

			DeleteCollection(m_Collections.size() - 1, true);
			//Plane::PrimaryPlane->UpdateBuffer(m_DraggingSnapCollection);
		}
		else
		{
			if (pixelPosition.x > Plane::ToolbarPlane->GetPosition().x && pixelPosition.x < Plane::ToolbarPlane->GetPosition().x + Plane::ToolbarPlane->GetSize().x &&
				pixelPosition.y > Plane::ToolbarPlane->GetPosition().y && pixelPosition.y < Plane::ToolbarPlane->GetPosition().y + Plane::ToolbarPlane->GetSize().y)
			{
				DeleteCollection(m_Collections.size() - 1, true);
			}
			else if (pixelPosition.x > Plane::PrimaryPlane->GetPosition().x && pixelPosition.x < Plane::PrimaryPlane->GetPosition().x + Plane::PrimaryPlane->GetSize().x &&
					 pixelPosition.y > Plane::PrimaryPlane->GetPosition().y && pixelPosition.y < Plane::PrimaryPlane->GetPosition().y + Plane::PrimaryPlane->GetSize().y)
			{
				bool found = false;

				for (uint64_t i = 0; i < Plane::PrimaryPlane->GetCollections().size(); i++)
				{
					if (Plane::PrimaryPlane->GetCollections()[i] == m_DraggingCollection)
						continue;

					//glm::vec<2, int32_t> colPos = (glm::vec<2, int32_t>)m_window->mapCoordsToPixel(
					//	Plane::PrimaryPlane->GetCollections()[i]->GetPosition(),
					//	*Plane::PrimaryPlane->GetView()) + Plane::PrimaryPlane->GetPosition();

					glm::vec<2, int32_t> colPos = Plane::PrimaryPlane->GetCollections()[i]->GetPosition() + Plane::PrimaryPlane->GetPosition();

					glm::vec<2, int32_t> colSize = glm::vec<2, int32_t>(
						Plane::PrimaryPlane->GetCollections()[i]->GetSize().x,
						Plane::PrimaryPlane->GetCollections()[i]->GetSize().y
					);

					//colSize = colSize * CalculateZoom();

					if (m_DraggingStack->GetPosition().x > colPos.x && m_DraggingStack->GetPosition().x < colPos.x + colSize.x &&
						m_DraggingStack->GetPosition().y > colPos.y && m_DraggingStack->GetPosition().y < colPos.y + colSize.y)
					{
						puts("dropping inside collection");
						m_DraggingStack->SetPosition((m_DraggingCollection->GetPosition() + GetPosition()) - colPos);
						Plane::PrimaryPlane->GetCollections()[i]->AddStack(m_DraggingStack);

						//Plane::PrimaryPlane->UpdateBuffer(i);

						m_DraggingCollection->RemoveAll(false);
						DeleteCollection(m_Collections.size() - 1, true);

						found = true;
						break;
					}
				}

				if (!found)
				{
					m_DraggingCollection->SetPosition((glm::vec<2, int32_t>)(((glm::vec<2, int32_t>)position - (glm::vec<2, int32_t>)Plane::PrimaryPlane->GetPosition()))
							  - (glm::vec<2, int32_t>)(((glm::vec<2, int32_t>)m_DraggingBeginMouse - m_DraggingBeginObject))
							  + (glm::vec<2, int32_t>)(glm::vec<2, int32_t>(1.0f, 1.0f) - /*Plane::PrimaryPlane->CalculateZoom() **/ Plane::PrimaryPlane->GetPosition())
						 - glm::vec<2, int32_t>(COLLECTION_EMPTY_SPACE, COLLECTION_EMPTY_SPACE)
					);

					//std::cout << Plane::PrimaryPlane->CalculateZoom().x << " || " << Plane::PrimaryPlane->CalculateZoom().y << std::endl;

					m_DraggingCollection->SetSize((glm::vec<2, int32_t>)m_DraggingCollection->GetSize() + glm::vec<2, int32_t>(COLLECTION_EMPTY_SPACE * 2, COLLECTION_EMPTY_SPACE * 2));
					m_DraggingStack->SetPosition({ COLLECTION_EMPTY_SPACE, COLLECTION_EMPTY_SPACE });

					DeleteCollection(m_Collections.size() - 1, false);
					Plane::PrimaryPlane->AddCollection(m_DraggingCollection, true);
				}
			}
		}
	}

	m_DraggingUp = false;
	m_DraggingCollection = nullptr;
	m_DraggingStack = nullptr;

	ClearSnap();
}

void Plane::DraggingStackUpdate()
{
	ClearSnap();

	glm::vec<2, int32_t> pixelPosition = m_DraggingCollection->GetPosition() + GetPosition();

	if (!(pixelPosition.x >= Plane::PrimaryPlane->GetPosition().x && pixelPosition.x < Plane::PrimaryPlane->GetSize().x + Plane::PrimaryPlane->GetPosition().x &&
		pixelPosition.y >= Plane::PrimaryPlane->GetPosition().y && pixelPosition.y < Plane::PrimaryPlane->GetSize().y + Plane::PrimaryPlane->GetPosition().y))
		return;

	Plane* usePlane = Plane::PrimaryPlane;
	const std::vector<Collection*>& useCollections = usePlane->GetCollections();

	uint64_t collectionMax = useCollections.size();

	if (collectionMax == 0)
		return;

	if (usePlane == this && (DraggingStack() || DraggingCollection()))
		collectionMax--;

	for (uint64_t i = 0; i < collectionMax; i++)
	{
		// size

		// TODO: consolidate these
		/*glm::vec<2, int32_t> collectionSize = glm::vec<2, int32_t>(
			((SNAP_DISTANCE * 2.0f * CalculateZoom().x) + useCollections[i]->GetSize().x),
			((SNAP_DISTANCE * 2.0f * CalculateZoom().y) + useCollections[i]->GetSize().y)
		);*/

		glm::vec<2, int32_t> collectionSize = glm::vec<2, int32_t>(
			((SNAP_DISTANCE * 2.0f) + useCollections[i]->GetSize().x),
			((SNAP_DISTANCE * 2.0f) + useCollections[i]->GetSize().y)
			);

		// position

		glm::vec<2, int32_t> collectionPosition = useCollections[i]->GetPosition();

		collectionPosition += usePlane->GetPosition();
		collectionPosition -= glm::vec<2, int32_t>(SNAP_DISTANCE/* * CalculateZoom().x*/, SNAP_DISTANCE/* * CalculateZoom().y*/);

		if (pixelPosition.x > collectionPosition.x && pixelPosition.x < collectionPosition.x + collectionSize.x &&
			pixelPosition.y > collectionPosition.y && pixelPosition.y < collectionPosition.y + collectionSize.y)
		{
			puts("collection");

			for (int64_t a = useCollections[i]->GetStacks().size() - 1; a >= 0; a--)
			{
				// size

				glm::vec<2, int32_t> stackSize;

				for (uint64_t b = 0; b < useCollections[i]->GetStacks()[a]->GetBlocks().size(); b++)
				{
					if (useCollections[i]->GetStacks()[a]->GetBlocks()[b]->GetWidth() > stackSize.x)
						stackSize.x = useCollections[i]->GetStacks()[a]->GetBlocks()[b]->GetWidth();// *CalculateZoom().x;
				}

				stackSize.y = useCollections[i]->GetStacks()[a]->GetBlocks().size() * Block::Height;// *CalculateZoom().y;
				stackSize += glm::vec<2, int32_t>(SNAP_DISTANCE * 2.0f/* * CalculateZoom().x*/, SNAP_DISTANCE * 2.0f/* * CalculateZoom().y*/);

				// position

				glm::vec<2, int32_t> stackPosition = useCollections[i]->GetPosition() + useCollections[i]->GetStacks()[a]->GetPosition();
				stackPosition += usePlane->GetPosition();
				stackPosition -= glm::vec<2, int32_t>(SNAP_DISTANCE/* * CalculateZoom().x*/, SNAP_DISTANCE/* * CalculateZoom().y*/);

				//stackPosition += collectionPosition;

				if (pixelPosition.x > stackPosition.x && pixelPosition.x < stackPosition.x + stackSize.x &&
					pixelPosition.y > stackPosition.y && pixelPosition.y < stackPosition.y + stackSize.y)
				{
					puts("stack");

					for (uint64_t b = 0; b < useCollections[i]->GetStacks()[a]->GetBlocks().size() + 1; b++)
					{
						uint64_t refIdx = 0;

						if (b > 0)
							refIdx = b - 1;

						// size

						glm::vec<2, int32_t> boundingSize(
							useCollections[i]->GetStacks()[a]->GetBlocks()[refIdx]->GetWidth()/* * CalculateZoom().x*/,
							Block::Height/* * CalculateZoom().y*/);

						boundingSize += glm::vec<2, int32_t>(SNAP_DISTANCE * 2, 0);

						// position

						glm::vec<2, int32_t> boundingPos = stackPosition;
						boundingPos += glm::vec<2, int32_t>(0, SNAP_DISTANCE/* * CalculateZoom().y*/);
						boundingPos.y += b * Block::Height/* * CalculateZoom().y*/;
						boundingPos -= glm::vec<2, int32_t>(0, (float)Block::Height / 2.0f/* * CalculateZoom().y*/);

						if (pixelPosition.x >= boundingPos.x && pixelPosition.x < boundingPos.x + boundingSize.x &&
							pixelPosition.y >= boundingPos.y && pixelPosition.y < boundingPos.y + boundingSize.y)
						{
							puts("snapping");
							SetSnap(i, b, useCollections[i]->GetStacks()[a]);

							// if block was bounded

							return;
						}
					}

					// if stack was bounded

					break;
				}
			}

			// if collection was bounded

			break;
		}
	}
}

bool Plane::DraggingCollection()
{
	return m_DraggingCollection != nullptr && m_DraggingStack == nullptr;
}

bool Plane::DraggingStack()
{
	return m_DraggingCollection != nullptr && m_DraggingStack != nullptr;
}

void Plane::SetSnap(uint64_t collection, uint64_t stackLoc, Stack* stack)
{
	m_DraggingSnap = true;

	m_DraggingSnapCollection = collection;
	m_DraggingSnapStackLoc = stackLoc;
	m_DraggingSnapStack = stack;
}

void Plane::ClearSnap()
{
	m_DraggingSnap = false;

	m_DraggingSnapCollection = 0;
	m_DraggingSnapStackLoc = 0;
	m_DraggingSnapStack = nullptr;
}

bool Plane::IsSnap()
{
	return m_DraggingSnap;
}

//sf::View* Plane::GetView()
//{
//	return &m_view;
//}

//glm::vec<2, int32_t> Plane::CalculateZoom()
//{
//	return glm::vec<2, int32_t>(
//		static_cast<float>(Global::WindowSize.x) / m_view.GetSize().x,
//		static_cast<float>(Global::WindowSize.y) / m_view.GetSize().y
//	);
//}

//glm::vec<2, int32_t> Plane::GetCoordsFromPixel(glm::vec<2, int32_t> pixel, sf::RenderWindow& window)
//{
//	return window.mapPixelToCoords((glm::vec<2, int32_t>)pixel, m_view);
//}

//void Plane::draw(sf::RenderTarget& target, sf::RenderStates states) const
//{
//	throw std::logic_error("use Plane::render instead");
//}
