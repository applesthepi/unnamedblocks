#include "RHR/registries/ShaderRegistry.h"
#include "RHR/handlers/ContextHandler.h"
#include "Plane.h"

#include <Espresso/Global.h>
#include <Cappuccino/Logger.h>
#include <Cappuccino/Intrinsics.h>
#include <SFML/System/Vector2.hpp>
#include <cassert>
#include <exception>
#include <iostream>
#include <vector>

#ifdef ENABLE_AVX2
#ifdef WIN32
#include <immintrin.h>
#else
// TODO add Unix intrinsics
#endif
#endif

static sf::Sprite TEST_SPRITE;

Plane::Plane(bool toolbar, const Plane& plane)
	:m_fontTexture(Global::Font.getTexture(Global::BlockHeight * 2)),
	m_fontBoldTexture(Global::FontBold.getTexture(Global::BlockHeight * 2))
{
	Setup(toolbar);

	for (uint64_t i = 0; i < m_collections.size(); i++)
		delete m_collections[i];

	m_collections.clear();

	for (uint64_t i = 0; i < plane.m_collections.size(); i++)
		m_collections.push_back(new Collection(*plane.m_collections[i]));
}

Plane::Plane(bool toolbar)
	:m_fontTexture(Global::Font.getTexture(Global::BlockHeight * 2)),
	m_fontBoldTexture(Global::FontBold.getTexture(Global::BlockHeight * 2))
{
	Setup(toolbar);

	m_collections.reserve(5);
	m_collectionVanity.reserve(5);

	m_vertexArrays.reserve(5);

	// m_innerText displays the inner position of the plane

	m_innerText = sf::Text("0, 0", Global::Font, 12);
	m_innerText.setFillColor(MOD_BUTTON_TEXT_FG);

	// callback for the context menu

	m_contextCallback = [](uint8_t idx)
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
	for (uint64_t i = 0; i < m_collections.size(); i++)
		delete m_collections[i];
}

void Plane::AddCollection(Collection* collection, bool displayCollectionVanity)
{
	m_collections.push_back(collection);
	m_collectionVanity.push_back(displayCollectionVanity);

	//std::cout << "add col " << collection->getPosition().x << ", " << collection->getPosition().y << std::endl;

	CreateBuffer(m_collections.size() - 1, displayCollectionVanity);
}

const std::vector<Collection*>& Plane::GetCollections()
{
	return m_collections;
}

void Plane::DeleteCollection(uint64_t idx, bool dealloc)
{
	m_vertexArrays.erase(m_vertexArrays.begin() + idx);
	m_vertexBuffers.erase(m_vertexBuffers.begin() + idx);
	m_vertexBufferTransform.erase(m_vertexBufferTransform.begin() + idx);
	m_textureMapImage.erase(m_textureMapImage.begin() + idx);
	m_textureMapTexture.erase(m_textureMapTexture.begin() + idx);
	m_textureMapEnabled.erase(m_textureMapEnabled.begin() + idx);
	m_collectionVanity.erase(m_collectionVanity.begin() + idx);

	if (dealloc)
		delete m_collections[idx];

	m_collections.erase(m_collections.begin() + idx);
}

void Plane::TranslateInnerPosition(const sf::Vector2i& position)
{
	m_innerPosition += position;
}

void Plane::SetInnerPosition(sf::Vector2i position)
{
	m_innerPosition = position;
}

sf::Vector2i Plane::GetInnerPosition()
{
	return m_innerPosition;
}

void Plane::DeleteContents(bool dealloc)
{
	for (uint64_t i = 0; i < m_collections.size(); i++)
		DeleteCollection(0, dealloc);
}

void Plane::UpdateVAOPosition(uint64_t idx)
{
	/*
	uint64_t i = 0;

	if (m_collectionVanity[idx])
	{
		//const sf::Vector2f pos = m_collections[idx]->getPosition() + getPosition();
		const sf::Vector2f size = (sf::Vector2f)m_collections[idx]->getSize();

		m_vertexArrays[idx][i++].position = sf::Vector2f();
		m_vertexArrays[idx][i++].position = sf::Vector2f(size.x, COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = sf::Vector2f(0, COLLECTION_OUTLINE_WIDTH);

		m_vertexArrays[idx][i++].position = sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH, 0);
		m_vertexArrays[idx][i++].position = sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = sf::Vector2f();

		m_vertexArrays[idx][i++].position = sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH, 0);
		m_vertexArrays[idx][i++].position = sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH, 0);

		m_vertexArrays[idx][i++].position = sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH, 0);

		m_vertexArrays[idx][i++].position = sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = sf::Vector2f(COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH);

		m_vertexArrays[idx][i++].position = sf::Vector2f(COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = sf::Vector2f(COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH);

		m_vertexArrays[idx][i++].position = sf::Vector2f(0, size.y + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = sf::Vector2f(COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = sf::Vector2f(COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH);

		m_vertexArrays[idx][i++].position = sf::Vector2f(0, COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = sf::Vector2f(COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = sf::Vector2f(0, size.y + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH);

		m_vertexArrays[idx][i++].position = sf::Vector2f(COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH);

		m_vertexArrays[idx][i++].position = sf::Vector2f(COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = sf::Vector2f(COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH);
	}
	*/


	// TODO not finished; may not need
}

void Plane::UpdateVAO(uint64_t idx)
{
	UpdateBuffer(idx);
}

void Plane::frameUpdate(double deltaTime)
{
	// set innerText to a visual representation of the inner position coordinates

	if (!m_toolbar)
	{
		m_innerText.setPosition(getPosition().x + 5, getPosition().y + getSize().y - 18);
		char innerText[20];
		sprintf(innerText, "%d, %d", GetInnerPosition().x, GetInnerPosition().y); // TODO make sure "%d" has no trailing "0"s.
		m_innerText.setString(innerText);
	}

	// dragging position

	if (DraggingStack() || DraggingCollection())
		m_draggingCollection->setPosition(m_draggingBeginObject + sf::Vector2f(
			(Global::MousePosition.x - m_draggingBeginMouse.x) / CalculateZoom().x,
			(Global::MousePosition.y - m_draggingBeginMouse.y) / CalculateZoom().y));
	
	// dragging connections

	if (DraggingStack())
		DraggingStackUpdate();

	// transform applies offset to the vertex buffers

	for (uint64_t i = 0; i < m_vertexBufferTransform.size(); i++)
		m_vertexBufferTransform[i] = sf::Transform().translate(sf::Vector2f((GetInnerPosition().x) * -1, (GetInnerPosition().y) * -1)).translate((sf::Vector2f)getPosition()).translate(m_collections[i]->getPosition());
}

bool Plane::mouseButton(bool down, const sf::Vector2i& position, const sf::Mouse::Button& button)
{
	if (m_window == nullptr)
		return false;

	sf::Vector2f mmpos = m_window->mapPixelToCoords(position, m_view);

	uint64_t collectionMax = m_collections.size();

	if (collectionMax == 0)
		return false;

	if ((this == Plane::ToolbarPlane && Plane::PrimaryPlane->DraggingStack()) ||
		(this == Plane::PrimaryPlane && Plane::ToolbarPlane->DraggingStack()))
		return false;

	if (DraggingStack() || DraggingCollection())
		collectionMax--;

	if (DraggingStack())
	{
		if (!down && !m_draggingUp)
		{
			// dragging and mouse released (used when dragging a stack)
			UnDrag(position);
		}
		else if (down && m_draggingUp)
		{
			// dragging and mouse pressed (used when duplicating stack)
			UnDrag(position);
		}
	}

	for (int64_t i = collectionMax - 1; i >= 0; i--)
	{
		if (!m_collections[i]->GetEnabled())
			continue;

		// size
		
		sf::Vector2u collectionSize = sf::Vector2u(
			m_collections[i]->getSize().x,
			m_collections[i]->getSize().y
		);
		
		// position

		sf::Vector2f collectionPosition = sf::Vector2f(
			(m_collections[i]->getPosition().x) + getPosition().x - GetInnerPosition().x,
			(m_collections[i]->getPosition().y) + getPosition().y - GetInnerPosition().y
		);

		if (mmpos.x > collectionPosition.x && mmpos.x < collectionPosition.x + collectionSize.x &&
			mmpos.y > collectionPosition.y && mmpos.y < collectionPosition.y + collectionSize.y)
		{
			//for (int64_t a = 0; a < m_collections[i]->GetStacks().size(); a++)
			//for (int64_t a = m_collections[i]->GetStacks().size() - 1; a >= 0; a--)
			for (int64_t a = m_collections[i]->GetStacks().size() - 1; a >= 0; a--)
			{
				// size

				sf::Vector2u stackSize;

				for (uint64_t b = 0; b < m_collections[i]->GetStacks()[a]->GetBlocks().size(); b++)
				{
					if (m_collections[i]->GetStacks()[a]->GetBlocks()[b]->GetWidth() > stackSize.x)
						stackSize.x = m_collections[i]->GetStacks()[a]->GetBlocks()[b]->GetWidth();
				}

				stackSize.y = m_collections[i]->GetStacks()[a]->GetBlocks().size() * Global::BlockHeight;
				stackSize = sf::Vector2u(stackSize.x, stackSize.y);

				// position

				sf::Vector2f stackPosition = sf::Vector2f(
					m_collections[i]->GetStacks()[a]->getPosition().x,
					m_collections[i]->GetStacks()[a]->getPosition().y);

				stackPosition += collectionPosition;

				if (mmpos.x > stackPosition.x && mmpos.x < stackPosition.x + stackSize.x &&
					mmpos.y > stackPosition.y && mmpos.y < stackPosition.y + stackSize.y)
				{
					for (uint64_t b = 0; b < m_collections[i]->GetStacks()[a]->GetBlocks().size(); b++)
					{
						// size

						sf::Vector2u blockSize;

						blockSize.x = m_collections[i]->GetStacks()[a]->GetBlocks()[b]->GetWidth();
						blockSize.y = Global::BlockHeight;
						blockSize = sf::Vector2u(blockSize.x, blockSize.y);

						// position

						sf::Vector2f blockPosition = sf::Vector2f(stackPosition.x, Global::BlockHeight * b + stackPosition.y);

						if (mmpos.x >= blockPosition.x && mmpos.x < blockPosition.x + blockSize.x &&
							mmpos.y >= blockPosition.y && mmpos.y < blockPosition.y + blockSize.y)
						{
							if (down && !DraggingStack())
							{
								// not dragging and mouse down

								if (button == sf::Mouse::Button::Left)
								{
									// remove context menu

									ContextHandler::Disable();
									UnSelect();

									Collection* activeCollection = new Collection();
									Stack* activeStack = m_collections[i]->GetStacks()[a];

									if (b > 0)
									{
										// create stack that is left behind (no the one picked up)

										Stack* leftStack = new Stack();
										leftStack->setPosition(activeStack->getPosition());
										
										for (uint64_t c = 0; c < b; c++)
											leftStack->AddBlock(m_collections[i]->GetStacks()[a]->GetBlocks()[c]);

										for (uint64_t c = 0; c < b; c++)
											activeStack->RemoveBlock(0);

										m_collections[i]->AddStack(leftStack);
									}

									activeCollection->setPosition((m_collections[i]->getPosition()) + sf::Vector2f(0, b * Global::BlockHeight) + ((sf::Vector2f)activeStack->getPosition()));
									activeCollection->setSize(sf::Vector2u(activeStack->GetWidestBlock(), activeStack->GetBlocks().size()* Global::BlockHeight));

									activeStack->setPosition(0, 0);

									// register stack and collection

									m_collections[i]->RemoveStack(a);
									activeCollection->AddStack(activeStack);
									AddCollection(activeCollection, false);

									// update buffer that was taken from

									if (m_collections[i]->GetStacks().size() > 0)
										UpdateBuffer(i);
									else
										DeleteCollection(i, true);

									// drag the current stack (excludes the stack that was left behind)

									DragStack(activeCollection, activeStack, false);
								}
								else if (button == sf::Mouse::Button::Right)
								{
									Logger::Debug("opening context menu");

									// startup context menu on block

									ContextHandler::Disable();
									ContextHandler::Enable(mmpos + sf::Vector2f(5, 5), &m_contextCallback);
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

void Plane::render(sf::RenderWindow& window)
{
	m_background.setFillColor(MOD_BACKGROUND_LOW);
	m_background.setPosition(getPosition());
	m_background.setSize((sf::Vector2f)getSize());

	window.setView(window.getDefaultView());
	window.draw(m_background);

	uint16_t collectionMax = m_collections.size();

	if (collectionMax == 0)
		return;

	if (DraggingStack() || DraggingCollection())
		collectionMax--;

	// render each batch collection

	window.setView(m_view);

	for (uint16_t i = 0; i < collectionMax; i++)
	{
		if (!m_collections[i]->GetEnabled())
			continue;

		// setup render

		sf::RenderStates states;
		states.transform = m_vertexBufferTransform[i];
		//states.transform.scale(sf::Vector2f(Global::BlockZoom, Global::BlockZoom), getPosition());//, /*sf::Vector2f(getSize().x / 2.0f, getSize().y / 2.0f) - */-(m_collections[i]->getPosition() * Global::BlockZoom));
		//sf::Texture re;
		//re.loadFromFile("res/grade.png");
		//if (m_textureMapEnabled[i])
		if (CalculateZoom().x < 0.95f)
			m_shader.setUniform("texture", /*TEST_TEXTURE*//*m_textureMapTexture[i]*//*re*/m_fontBoldEditedTexture);
		else
			m_shader.setUniform("texture", /*TEST_TEXTURE*//*m_textureMapTexture[i]*//*re*/m_fontEditedTexture);

		states.shader = &m_shader;

		// render VBO
		window.draw(m_vertexBuffers[i], states);
	}

	window.setView(window.getDefaultView());

	// render inner position coordinates

	//window.draw(m_innerText);
	m_window = &window;
}

void Plane::snapRender(sf::RenderWindow& window)
{
	// snap graphic

	if (IsSnap())
	{
		RevertWindowView rwv(&window);
		window.setView(m_view);

		m_draggingShape.setPosition(
			Plane::PrimaryPlane->getPosition() +
			Plane::PrimaryPlane->GetCollections()[m_draggingSnapCollection]->getPosition() +
			m_draggingSnapStack->getPosition() +
			sf::Vector2f(0, m_draggingSnapStackLoc * Global::BlockHeight) -
			(sf::Vector2f)Plane::PrimaryPlane->GetInnerPosition());

		uint64_t refIdx = 0;

		if (m_draggingSnapStackLoc > 0)
			refIdx = m_draggingSnapStackLoc - 1;

		m_draggingShape.setSize(sf::Vector2f(m_draggingSnapStack->GetBlocks()[refIdx]->GetWidth(), SNAP_GRAPHIC_HEIGHT));

		window.draw(m_draggingShape);
	}
}

void Plane::postRender(sf::RenderWindow& window)
{
	if (DraggingCollection())
	{

	}
	else if (DraggingStack())
	{
		sf::RenderStates states;
		states.transform = m_vertexBufferTransform[m_vertexBufferTransform.size() - 1];

		if (m_textureMapEnabled[m_vertexBufferTransform.size() - 1])
			m_shader.setUniform("texture", m_textureMapTexture[m_vertexBufferTransform.size() - 1]);

		states.shader = &m_shader;

		window.setView(m_view);
		window.draw(m_vertexBuffers.back(), states);
		window.setView(window.getDefaultView());
	}

#if 0
	window.draw(TEST_SPRITE);
#endif
}

void Plane::Setup(bool toolbar)
{
	m_toolbar = toolbar;
	m_selected = false;
	m_selectedContext = false;

	m_draggingCollection = nullptr;
	m_draggingStack = nullptr;
	m_draggingUp = false;

	m_shader.loadFromFile("res/shaders/blocks.vert", "res/shaders/blocks.frag");

	sf::Image imageCollectionOpen;
	sf::Image imageCollectionClose;

	imageCollectionOpen.loadFromFile("res/collection_tab_open.png");
	imageCollectionClose.loadFromFile("res/collection_tab_closed.png");

	imageCollectionOpen.flipVertically();
	imageCollectionClose.flipVertically();

	m_textureCollectionOpen.loadFromImage(imageCollectionOpen);
	m_textureCollectionClosed.loadFromImage(imageCollectionClose);

	m_draggingShape.setFillColor(MOD_BUTTON_TEXT_FG);

	m_draggingSnap = false;
	m_draggingSnapCollection = 0;
	m_draggingSnapStackLoc = 0;
	m_draggingSnapStack = nullptr;

	m_window = nullptr;
}

void Plane::UpdateCollectionVAO(std::vector<sf::Vertex>* vao, sf::Vector2u size)
{
	// ====================================================================================================================================================
	// =============== add vao; see "dev/collection_geometry.png"
	// ====================================================================================================================================================

	float positions[] = {
		// 0
		0.0f, 0.0f,
		(COLLECTION_TAB_PART * COLLECTION_TAB_PART_COUNT) + COLLECTION_OUTLINE_WIDTH, 0.0f,
		(COLLECTION_TAB_PART * COLLECTION_TAB_PART_COUNT) + (COLLECTION_OUTLINE_WIDTH * 2), 0.0f,
		(COLLECTION_TAB_PART * COLLECTION_TAB_PART_COUNT) + (COLLECTION_OUTLINE_WIDTH * 2), COLLECTION_TAB_PART,
		static_cast<float>(size.x + COLLECTION_OUTLINE_WIDTH), COLLECTION_TAB_PART,
		// 5
		static_cast<float>(size.x + (COLLECTION_OUTLINE_WIDTH * 2)), COLLECTION_TAB_PART,
		static_cast<float>(size.x + (COLLECTION_OUTLINE_WIDTH * 2)), static_cast<float>(COLLECTION_TAB_PART + size.y + COLLECTION_OUTLINE_WIDTH),
		static_cast<float>(size.x + (COLLECTION_OUTLINE_WIDTH * 2)), static_cast<float>(COLLECTION_TAB_PART + size.y + (COLLECTION_OUTLINE_WIDTH * 2)),
		COLLECTION_OUTLINE_WIDTH, static_cast<float>(COLLECTION_TAB_PART + size.y + (COLLECTION_OUTLINE_WIDTH * 2)),
		0.0f, static_cast<float>(COLLECTION_TAB_PART + size.y + (COLLECTION_OUTLINE_WIDTH * 2)),
		// 10
		0.0f, COLLECTION_OUTLINE_WIDTH,
		COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH,
		COLLECTION_OUTLINE_WIDTH + (COLLECTION_TAB_PART * COLLECTION_TAB_PART_COUNT), COLLECTION_OUTLINE_WIDTH,
		COLLECTION_OUTLINE_WIDTH + (COLLECTION_TAB_PART * COLLECTION_TAB_PART_COUNT), COLLECTION_TAB_PART,
		COLLECTION_OUTLINE_WIDTH + (COLLECTION_TAB_PART * COLLECTION_TAB_PART_COUNT), COLLECTION_TAB_PART + COLLECTION_OUTLINE_WIDTH,
		// 15
		static_cast<float>(size.x + COLLECTION_OUTLINE_WIDTH), COLLECTION_TAB_PART + COLLECTION_OUTLINE_WIDTH,
		static_cast<float>(size.x + COLLECTION_OUTLINE_WIDTH), static_cast<float>(COLLECTION_TAB_PART + COLLECTION_OUTLINE_WIDTH + size.y),
		COLLECTION_OUTLINE_WIDTH, static_cast<float>(COLLECTION_TAB_PART + COLLECTION_OUTLINE_WIDTH + size.y),
		COLLECTION_OUTLINE_WIDTH, COLLECTION_TAB_PART + COLLECTION_OUTLINE_WIDTH,
		// 19 (tab texture)
		COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH,
		COLLECTION_OUTLINE_WIDTH + (COLLECTION_TAB_PART * COLLECTION_TAB_PART_COUNT), COLLECTION_OUTLINE_WIDTH,
		COLLECTION_OUTLINE_WIDTH + (COLLECTION_TAB_PART * COLLECTION_TAB_PART_COUNT), COLLECTION_TAB_PART + COLLECTION_OUTLINE_WIDTH,
		COLLECTION_OUTLINE_WIDTH, COLLECTION_TAB_PART + COLLECTION_OUTLINE_WIDTH,
		// 23 (background)
		COLLECTION_OUTLINE_WIDTH, COLLECTION_TAB_PART + COLLECTION_OUTLINE_WIDTH,
		static_cast<float>(size.x + COLLECTION_OUTLINE_WIDTH), COLLECTION_TAB_PART + COLLECTION_OUTLINE_WIDTH,
		static_cast<float>(size.x + COLLECTION_OUTLINE_WIDTH), static_cast<float>(COLLECTION_TAB_PART + COLLECTION_OUTLINE_WIDTH + size.y),
		COLLECTION_OUTLINE_WIDTH, static_cast<float>(COLLECTION_TAB_PART + COLLECTION_OUTLINE_WIDTH + size.y),
		// 27 (tab background)
		COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH,
		COLLECTION_OUTLINE_WIDTH + (COLLECTION_TAB_PART * COLLECTION_TAB_PART_COUNT), COLLECTION_OUTLINE_WIDTH,
		COLLECTION_OUTLINE_WIDTH + (COLLECTION_TAB_PART * COLLECTION_TAB_PART_COUNT), COLLECTION_TAB_PART + COLLECTION_OUTLINE_WIDTH,
		COLLECTION_OUTLINE_WIDTH, COLLECTION_TAB_PART + COLLECTION_OUTLINE_WIDTH,
	};

	const uint8_t colors[] = {
		// 0
		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
		// 5
		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
		// 10
		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
		// 15
		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
		COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B,
		// 19 (tab texture)
		255, 255, 255,
		255, 255, 255,
		255, 255, 255,
		255, 255, 255,
		// 23 (background)
		COLLECTION_COLOR_FILL_R, COLLECTION_COLOR_FILL_G, COLLECTION_COLOR_FILL_B,
		COLLECTION_COLOR_FILL_R, COLLECTION_COLOR_FILL_G, COLLECTION_COLOR_FILL_B,
		COLLECTION_COLOR_FILL_R, COLLECTION_COLOR_FILL_G, COLLECTION_COLOR_FILL_B,
		COLLECTION_COLOR_FILL_R, COLLECTION_COLOR_FILL_G, COLLECTION_COLOR_FILL_B,
		// 27 (tab background)
		COLLECTION_COLOR_FILL_R, COLLECTION_COLOR_FILL_G, COLLECTION_COLOR_FILL_B,
		COLLECTION_COLOR_FILL_R, COLLECTION_COLOR_FILL_G, COLLECTION_COLOR_FILL_B,
		COLLECTION_COLOR_FILL_R, COLLECTION_COLOR_FILL_G, COLLECTION_COLOR_FILL_B,
		COLLECTION_COLOR_FILL_R, COLLECTION_COLOR_FILL_G, COLLECTION_COLOR_FILL_B,
	};

	const float textureCoords[] = {
		// 0
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		// 5
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		// 10
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		// 15
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		// 19 (tab texture)
		1, 1,
		91, 1,
		91, 31,
		1, 31,
		// 23 (background)
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		// 27 (tab background)
		0, 0,
		0, 0,
		0, 0,
		0, 0,
	};

	const uint8_t indices[] = {
		// 0
		0, 12, 10,
		1, 12, 0,
		2, 3, 1,
		13, 12, 3,
		14, 13, 15,
		// 5
		4, 15, 13,
		5, 6, 4,
		6, 16, 15,
		7, 17, 16,
		8, 17, 7,
		// 10
		9, 10, 8,
		10, 11, 8,
		11, 14, 18,
		11, 12, 14,
		18, 16, 17,
		// 15
		18, 15, 16,
		// 16 (background)
		23, 25, 26,
		23, 24, 25,
		// 18 (tab background)
		27, 29, 30,
		27, 28, 29,
		// 20 (tab texture)
		19, 21, 22,
		19, 20, 21,
	};

#ifdef ENABLE_AVX2
	{
		__m256 _positions, _op, _result;

		_op = _mm256_set_ps(
			COLLECTION_OUTLINE_WIDTH,
			COLLECTION_OUTLINE_WIDTH + COLLECTION_TAB_PART,
			COLLECTION_OUTLINE_WIDTH,
			COLLECTION_OUTLINE_WIDTH + COLLECTION_TAB_PART,
			COLLECTION_OUTLINE_WIDTH,
			COLLECTION_OUTLINE_WIDTH + COLLECTION_TAB_PART,
			COLLECTION_OUTLINE_WIDTH,
			COLLECTION_OUTLINE_WIDTH + COLLECTION_TAB_PART);

		for (uint8_t i = 0; i < 7; i++)
		{
			_positions = _mm256_set_ps(
				positions[i * 8 + 0], positions[i * 8 + 1], positions[i * 8 + 2], positions[i * 8 + 3],
				positions[i * 8 + 4], positions[i * 8 + 5], positions[i * 8 + 6], positions[i * 8 + 7]);

			_result = _mm256_sub_ps(_positions, _op);

#ifdef WIN32
			positions[i * 8 + 0] = _result.m256_f32[7];
			positions[i * 8 + 1] = _result.m256_f32[6];
			positions[i * 8 + 2] = _result.m256_f32[5];
			positions[i * 8 + 3] = _result.m256_f32[4];

			positions[i * 8 + 4] = _result.m256_f32[3];
			positions[i * 8 + 5] = _result.m256_f32[2];
			positions[i * 8 + 6] = _result.m256_f32[1];
			positions[i * 8 + 7] = _result.m256_f32[0];
#else
			positions[i * 8 + 0] = _result[7];
			positions[i * 8 + 1] = _result[6];
			positions[i * 8 + 2] = _result[5];
			positions[i * 8 + 3] = _result[4];

			positions[i * 8 + 4] = _result[3];
			positions[i * 8 + 5] = _result[2];
			positions[i * 8 + 6] = _result[1];
			positions[i * 8 + 7] = _result[0];
#endif
		}

		_positions = _mm256_set_ps(
			positions[56], positions[57], positions[58], positions[59],
			positions[60], positions[61], COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH + COLLECTION_TAB_PART);

		_result = _mm256_sub_ps(_positions, _op);

#ifdef WIN32
		positions[56] = _result.m256_f32[0];
		positions[57] = _result.m256_f32[1];
		positions[58] = _result.m256_f32[2];
		positions[59] = _result.m256_f32[3];

		positions[60] = _result.m256_f32[4];
		positions[61] = _result.m256_f32[5];
#else
		positions[56] = _result[0];
		positions[57] = _result[1];
		positions[58] = _result[2];
		positions[59] = _result[3];

		positions[60] = _result[4];
		positions[61] = _result[5];
#endif
	}
#else
	{
		for (uint8_t i = 0; i < 31 * 2; i++)
		{
			if (i % 2 == 0)
				positions[i] -= COLLECTION_OUTLINE_WIDTH;
			else
				positions[i] -= COLLECTION_OUTLINE_WIDTH + COLLECTION_TAB_PART;
		}
	}
#endif

	vao->clear();
	vao->reserve(22 * 3);

	ParseIndices(vao, positions, colors, textureCoords, indices, 22);
}

void Plane::CreateBuffer(uint16_t collectionIdx, bool displayCollectionVanity)
{
	// Create VAO

	m_vertexArrays.push_back(std::vector<sf::Vertex>());
	m_vertexArrays.back().reserve(200);

	// Create VBO
	
	// "Triangles" because we are batch rendering and can not use the other optimized connected primitive types
	// "Dynamic" because "Occasionally changing data", the buffer will only be changed during a user induced event
	m_vertexBuffers.push_back(sf::VertexBuffer(sf::PrimitiveType::Triangles, sf::VertexBuffer::Usage::Dynamic));

	m_vertexBufferTransform.push_back(sf::Transform());
	m_textureMapImage.push_back(sf::Image());
	m_textureMapTexture.push_back(sf::Texture());
	m_textureMapEnabled.push_back(false);

	UpdateBuffer(collectionIdx);
}

void Plane::UpdateBuffer(uint16_t bufferIdx)
{
	// update font image

	m_fontEditedImage = m_fontTexture.copyToImage();
	m_fontEditedImage.setPixel(0, 0, sf::Color::White);
	m_fontEditedTexture.loadFromImage(m_fontEditedImage);

	m_fontBoldEditedImage = m_fontBoldTexture.copyToImage();
	m_fontBoldEditedImage.setPixel(0, 0, sf::Color::White);
	m_fontBoldEditedTexture.loadFromImage(m_fontBoldEditedImage);

	//TEST_SPRITE.setTexture(m_fontBoldEditedTexture, true);

	// clean and update collection VAO
	
	m_vertexArrays[bufferIdx].clear();

	if (m_collectionVanity[bufferIdx])
		UpdateCollectionVAO(&(m_vertexArrays[bufferIdx]), m_collections[bufferIdx]->getSize());

	// pull vertex buffers from children

	std::vector<sf::VertexArray> va;
	std::vector<std::vector<bool>*> vaChar;
	//std::vector<const sf::Image*> vaTextures;
	std::vector<std::vector<sf::Vertex>> vaBox;
	std::vector<uint16_t> vaSize;

	for (uint64_t a = 0; a < m_collections[bufferIdx]->GetStacks().size(); a++)
	{
		for (uint64_t b = 0; b < m_collections[bufferIdx]->GetStacks()[a]->GetBlocks().size(); b++)
		{
			vaBox.push_back(std::vector<sf::Vertex>());
			uint16_t calcSize = 0;

			// add block vertices

			float blockWidth = static_cast<float>(Global::BlockBorder);

			sf::Vector2f blockPos = m_collections[bufferIdx]->GetStacks()[a]->getPosition() + sf::Vector2f(0, b * Global::BlockHeight);
			uint32_t blockShellWidth = m_collections[bufferIdx]->GetStacks()[a]->GetBlocks()[b]->GetWidth();
			const sf::Color blockColor = m_collections[bufferIdx]->GetStacks()[a]->GetBlocks()[b]->GetModCategory()->GetColor();

			vaBox.back().push_back(sf::Vertex(blockPos + sf::Vector2f(0, 0), blockColor, sf::Vector2f(0, 0)));
			vaBox.back().push_back(sf::Vertex(blockPos + sf::Vector2f(blockShellWidth, 0), blockColor, sf::Vector2f(0, 0)));
			vaBox.back().push_back(sf::Vertex(blockPos + sf::Vector2f(blockShellWidth, Global::BlockHeight), blockColor, sf::Vector2f(0, 0)));

			vaBox.back().push_back(sf::Vertex(blockPos + sf::Vector2f(0, 0), blockColor, sf::Vector2f(0, 0)));
			vaBox.back().push_back(sf::Vertex(blockPos + sf::Vector2f(blockShellWidth, Global::BlockHeight), blockColor, sf::Vector2f(0, 0)));
			vaBox.back().push_back(sf::Vertex(blockPos + sf::Vector2f(0, Global::BlockHeight), blockColor, sf::Vector2f(0, 0)));

			// get argument information
			
			for (uint64_t c = 0; c < m_collections[bufferIdx]->GetStacks()[a]->GetBlocks()[b]->GetArguments().size(); c++)
			{
				Argument* arg = m_collections[bufferIdx]->GetStacks()[a]->GetBlocks()[b]->GetArguments()[c];

				calcSize++;

				va.push_back(*arg->GetVAO().GetVertexArray());
				vaChar.push_back(arg->GetVAO().GetChar());

				for (uint64_t d = 0; d < va.back().getVertexCount(); d++)
				{
					if (vaChar.back()->at(d))
					{
						Global::FontBold.getGlyph(va.back()[d].texCoords.x, Global::BlockHeight * 2, false);
						sf::Glyph gl = Global::Font.getGlyph(va.back()[d].texCoords.x, Global::BlockHeight * 2, false);
						
						// make char texture coordinate match the charsheet

						va.back()[d + 0].texCoords = sf::Vector2f(gl.textureRect.left, gl.textureRect.top);
						va.back()[d + 1].texCoords = sf::Vector2f(gl.textureRect.left + gl.textureRect.width, gl.textureRect.top);
						va.back()[d + 2].texCoords = sf::Vector2f(gl.textureRect.left + gl.textureRect.width, gl.textureRect.top + gl.textureRect.height);
						va.back()[d + 3].texCoords = sf::Vector2f(gl.textureRect.left, gl.textureRect.top);
						va.back()[d + 4].texCoords = sf::Vector2f(gl.textureRect.left + gl.textureRect.width, gl.textureRect.top + gl.textureRect.height);
						va.back()[d + 5].texCoords = sf::Vector2f(gl.textureRect.left, gl.textureRect.top + gl.textureRect.height);

						va.back()[d].position += sf::Vector2f(
							static_cast<float>(blockWidth) + blockPos.x,
							blockPos.y + ((float)Global::BlockBorder / 2.0f)
						);
					}
					else
					{
						// make VAO absolute
						va.back()[d].position += sf::Vector2f(
							static_cast<float>(blockWidth) + blockPos.x,
							blockPos.y + ((float)Global::BlockBorder / 2.0f)
						);
					}
				}

				blockWidth += static_cast<float>(arg->GetWidth());
				blockWidth += static_cast<float>(Global::BlockBorder);
			}

			vaSize.push_back(calcSize);

			// for every block
		}

		// for every stack
	}

	if (m_collectionVanity[bufferIdx])
	{
		// TODO replace with intrinsic functions

		for (uint8_t i = 0; i < 6; i++)
			m_vertexArrays[bufferIdx][20 * 3 + i].texCoords.y += m_fontEditedImage.getSize().y;

		for (uint8_t a = 0; a < 22 * 3; a++)
		{
			m_vertexArrays[bufferIdx][a].texCoords = sf::Vector2f(
				m_vertexArrays[bufferIdx][a].texCoords.x / static_cast<float>(m_fontEditedImage.getSize().x),
				m_vertexArrays[bufferIdx][a].texCoords.y / static_cast<float>(m_fontEditedImage.getSize().y)
			);
		}
	}

	// normalize

	for (uint64_t a = 0; a < va.size(); a++)
	{
		for (uint64_t b = 0; b < va[a].getVertexCount(); b++)
		{
			va[a][b].texCoords = sf::Vector2f(
				va[a][b].texCoords.x / static_cast<float>(m_fontEditedImage.getSize().x),
				va[a][b].texCoords.y / static_cast<float>(m_fontEditedImage.getSize().y)
			);
		}
	}

	// normalize

	for (uint64_t a = 0; a < vaBox.size(); a++)
	{
		for (uint8_t b = 0; b < 6; b++)
		{
			vaBox[a][b].texCoords = sf::Vector2f(
				vaBox[a][b].texCoords.x / static_cast<float>(m_fontEditedImage.getSize().x),
				vaBox[a][b].texCoords.y / static_cast<float>(m_fontEditedImage.getSize().y)
			);
			//m_vertexArrays[bufferIdx].push_back(vaBox[a][b]);
		}
	}

	// add vertices

	uint32_t fullSize = 0;

	for (uint64_t i = 0; i < vaSize.size(); i++)
	{
		// add block vertices to VAO

		for (uint8_t a = 0; a < 6; a++)
			m_vertexArrays[bufferIdx].push_back(vaBox[i][a]);

		// add block argument vertices to VAO

		for (uint16_t a = 0; a < vaSize[i]; a++)
		{
			for (uint16_t b = 0; b < va[fullSize].getVertexCount(); b++)
				m_vertexArrays[bufferIdx].push_back(va[fullSize][b]);

			fullSize++;
		}
	}

	// update collection VBO

	m_vertexBuffers[bufferIdx].create(m_vertexArrays[bufferIdx].size());
	m_vertexBuffers[bufferIdx].update(&(m_vertexArrays[bufferIdx][0]), m_vertexArrays[bufferIdx].size(), 0);
}

void Plane::ParseIndices(std::vector<sf::Vertex>* vao, const float positions[], const uint8_t colors[], const float textureCoords[], const uint8_t indices[], uint8_t indexCount)
{
	for (uint8_t i = 0; i < indexCount; i++)
	{
		uint8_t idx0 = indices[i * 3];

		vao->push_back(sf::Vertex(sf::Vector2f(positions[idx0 * 2], positions[idx0 * 2 + 1]),
			sf::Color(colors[idx0 * 3], colors[idx0 * 3 + 1], colors[idx0 * 3 + 2]),
			sf::Vector2f(textureCoords[idx0 * 2], textureCoords[idx0 * 2 + 1])));

		uint8_t idx1 = indices[i * 3 + 1];

		vao->push_back(sf::Vertex(sf::Vector2f(positions[idx1 * 2], positions[idx1 * 2 + 1]),
			sf::Color(colors[idx1 * 3], colors[idx1 * 3 + 1], colors[idx1 * 3 + 2]),
			sf::Vector2f(textureCoords[idx1 * 2], textureCoords[idx1 * 2 + 1])));

		uint8_t idx2 = indices[i * 3 + 2];

		vao->push_back(sf::Vertex(sf::Vector2f(positions[idx2 * 2], positions[idx2 * 2 + 1]),
			sf::Color(colors[idx2 * 3], colors[idx2 * 3 + 1], colors[idx2 * 3 + 2]),
			sf::Vector2f(textureCoords[idx2 * 2], textureCoords[idx2 * 2 + 1])));
	}
}

void Plane::Select(uint64_t collection, uint64_t stack, uint64_t block, uint64_t argument)
{
	m_selected = true;

	m_selectedCollection = m_collections[collection];
	m_selectedStack = m_collections[collection]->GetStacks()[stack];
	m_selectedBlock = m_collections[collection]->GetStacks()[stack]->GetBlocks()[block];
	m_selectedArgument = m_collections[collection]->GetStacks()[stack]->GetBlocks()[block]->GetArguments()[argument];
}

void Plane::SelectContext(uint64_t collection, uint64_t stack, uint64_t block)
{
	m_selected = true;

	m_selectedCollection = m_collections[collection];
	m_selectedStack = m_collections[collection]->GetStacks()[stack];
	m_selectedBlock = m_collections[collection]->GetStacks()[stack]->GetBlocks()[block];
	m_selectedArgument = nullptr;
}

void Plane::UnSelect()
{
	m_selected = false;

	m_selectedCollection = nullptr;
	m_selectedStack = nullptr;
	m_selectedBlock = nullptr;
	m_selectedArgument = nullptr;
}

void Plane::DragCollection(Collection* collection, bool up)
{
	if (m_draggingStack != nullptr || m_draggingCollection != nullptr)
		return;

	m_draggingUp = up;

	m_draggingCollection = collection;
	m_draggingStack = nullptr;

	m_draggingBeginMouse = Global::MousePosition;
	m_draggingBeginObject = m_draggingCollection->getPosition();
}

void Plane::DragStack(Collection* collection, Stack* stack, bool up)
{
	if (m_draggingStack != nullptr || m_draggingCollection != nullptr)
		return;

	m_draggingUp = up;

	m_draggingCollection = collection;
	m_draggingStack = stack;

	m_draggingBeginMouse = Global::MousePosition;
	m_draggingBeginObject = collection->getPosition();
}

void Plane::UnDrag(const sf::Vector2i& position)
{
	if (DraggingStack())
	{
		if (IsSnap())
		{
			m_draggingSnapStack->InsertBlocks(m_draggingStack->GetBlocks(), m_draggingSnapStackLoc);
			m_draggingCollection->RemoveAll(false);

			DeleteCollection(m_collections.size() - 1, true);
			Plane::PrimaryPlane->UpdateBuffer(m_draggingSnapCollection);
		}
		else
		{
			sf::Vector2f mmpos = m_window->mapPixelToCoords(position, m_view);

			if (position.x > Plane::ToolbarPlane->getPosition().x && position.x < Plane::ToolbarPlane->getPosition().x + Plane::ToolbarPlane->getSize().x &&
				position.y > Plane::ToolbarPlane->getPosition().y && position.y < Plane::ToolbarPlane->getPosition().y + Plane::ToolbarPlane->getSize().y)
			{
				DeleteCollection(m_collections.size() - 1, true);
			}
			else if (position.x > Plane::PrimaryPlane->getPosition().x && position.x < Plane::PrimaryPlane->getPosition().x + Plane::PrimaryPlane->getSize().x &&
				position.y > Plane::PrimaryPlane->getPosition().y && position.y < Plane::PrimaryPlane->getPosition().y + Plane::PrimaryPlane->getSize().y)
			{
				bool found = false;

				for (uint64_t i = 0; i < Plane::PrimaryPlane->GetCollections().size(); i++)
				{
					if (Plane::PrimaryPlane->GetCollections()[i] == m_draggingCollection)
						continue;

					sf::Vector2f colPos = sf::Vector2f(
						Plane::PrimaryPlane->GetCollections()[i]->getPosition().x,
						Plane::PrimaryPlane->GetCollections()[i]->getPosition().y
					);

					colPos += getPosition();

					sf::Vector2f colSize = sf::Vector2f(
						Plane::PrimaryPlane->GetCollections()[i]->getSize().x,
						Plane::PrimaryPlane->GetCollections()[i]->getSize().y
					);

					if (mmpos.x > colPos.x && mmpos.x < colPos.x + colSize.x &&
						mmpos.y > colPos.y && mmpos.y < colPos.y + colSize.y)
					{
						m_draggingStack->setPosition((m_draggingCollection->getPosition() + getPosition()) - colPos);
						Plane::PrimaryPlane->GetCollections()[i]->AddStack(m_draggingStack);

						Plane::PrimaryPlane->UpdateBuffer(i);

						m_draggingCollection->RemoveAll(false);
						DeleteCollection(m_collections.size() - 1, true);

						found = true;
						break;
					}
				}

				if (!found)
				{
					m_draggingCollection->setPosition(sf::Vector2f(
						m_draggingCollection->getPosition().x - Plane::PrimaryPlane->getPosition().x + getPosition().x,
						m_draggingCollection->getPosition().y - Plane::PrimaryPlane->getPosition().y + getPosition().y)
						- sf::Vector2f(COLLECTION_EMPTY_SPACE, COLLECTION_EMPTY_SPACE));

					m_draggingCollection->setSize((sf::Vector2u)m_draggingCollection->getSize() + sf::Vector2u(COLLECTION_EMPTY_SPACE * 2, COLLECTION_EMPTY_SPACE * 2));

					m_draggingStack->setPosition(COLLECTION_EMPTY_SPACE, COLLECTION_EMPTY_SPACE);

					DeleteCollection(m_collections.size() - 1, false);
					Plane::PrimaryPlane->AddCollection(m_draggingCollection, true);
				}
			}
		}
	}

	m_draggingUp = false;
	m_draggingCollection = nullptr;
	m_draggingStack = nullptr;

	ClearSnap();
}

void Plane::DraggingStackUpdate()
{
	ClearSnap();

	Plane* usePlane = Plane::PrimaryPlane;

	sf::Vector2f position = (sf::Vector2f)m_window->mapCoordsToPixel(m_draggingCollection->getPosition() + m_draggingStack->getPosition());
	position += getPosition();

	if (position.x >= Plane::ToolbarPlane->getPosition().x && position.x < Plane::ToolbarPlane->getSize().x + Plane::ToolbarPlane->getPosition().x &&
		position.y >= Plane::ToolbarPlane->getPosition().y && position.y < Plane::ToolbarPlane->getSize().y + Plane::ToolbarPlane->getPosition().y)
		return;

	const std::vector<Collection*>& useCollections = usePlane->GetCollections();

	uint64_t collectionMax = useCollections.size();

	if (collectionMax == 0)
		return;

	if (usePlane == this && (DraggingStack() || DraggingCollection()))
		collectionMax--;

	for (uint64_t i = 0; i < collectionMax; i++)
	{
		// size

		sf::Vector2u collectionSize = useCollections[i]->getSize();
		collectionSize += sf::Vector2u(SNAP_DISTANCE * 2, SNAP_DISTANCE * 2);

		// position

		sf::Vector2f collectionPosition = useCollections[i]->getPosition();
		collectionPosition += usePlane->getPosition();
		collectionPosition -= sf::Vector2f(SNAP_DISTANCE, SNAP_DISTANCE);

		if (position.x > collectionPosition.x && position.x < collectionPosition.x + collectionSize.x &&
			position.y > collectionPosition.y && position.y < collectionPosition.y + collectionSize.y)
		{
			for (int64_t a = useCollections[i]->GetStacks().size() - 1; a >= 0; a--)
			{
				// size

				sf::Vector2u stackSize;

				for (uint64_t b = 0; b < useCollections[i]->GetStacks()[a]->GetBlocks().size(); b++)
				{
					if (useCollections[i]->GetStacks()[a]->GetBlocks()[b]->GetWidth() > stackSize.x)
						stackSize.x = useCollections[i]->GetStacks()[a]->GetBlocks()[b]->GetWidth();
				}

				stackSize.y = useCollections[i]->GetStacks()[a]->GetBlocks().size() * Global::BlockHeight;
				stackSize += sf::Vector2u(SNAP_DISTANCE * 2, SNAP_DISTANCE * 2);

				// position

				sf::Vector2f stackPosition = useCollections[i]->GetStacks()[a]->getPosition();
				stackPosition += collectionPosition;

				if (position.x > stackPosition.x && position.x < stackPosition.x + stackSize.x &&
					position.y > stackPosition.y && position.y < stackPosition.y + stackSize.y)
				{
					for (uint64_t b = 0; b < useCollections[i]->GetStacks()[a]->GetBlocks().size() + 1; b++)
					{
						uint64_t refIdx = 0;

						if (b > 0)
							refIdx = b - 1;

						// size

						sf::Vector2u boundingSize(
							useCollections[i]->GetStacks()[a]->GetBlocks()[refIdx]->GetWidth(),
							Global::BlockHeight);

						boundingSize += sf::Vector2u(SNAP_DISTANCE * 2, 0);

						// position

						sf::Vector2f boundingPos = stackPosition;
						boundingPos += sf::Vector2f(0, SNAP_DISTANCE);
						boundingPos.y += b * Global::BlockHeight;
						boundingPos -= sf::Vector2f(0, (float)Global::BlockHeight / 2.0f);

						if (position.x >= boundingPos.x && position.x < boundingPos.x + boundingSize.x &&
							position.y >= boundingPos.y && position.y < boundingPos.y + boundingSize.y)
						{
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
	return m_draggingCollection != nullptr && m_draggingStack == nullptr;
}

bool Plane::DraggingStack()
{
	return m_draggingCollection != nullptr && m_draggingStack != nullptr;
}

void Plane::SetSnap(uint64_t collection, uint64_t stackLoc, Stack* stack)
{
	m_draggingSnap = true;

	m_draggingSnapCollection = collection;
	m_draggingSnapStackLoc = stackLoc;
	m_draggingSnapStack = stack;
}

void Plane::ClearSnap()
{
	m_draggingSnap = false;

	m_draggingSnapCollection = 0;
	m_draggingSnapStackLoc = 0;
	m_draggingSnapStack = nullptr;
}

bool Plane::IsSnap()
{
	return m_draggingSnap;
}

sf::View* Plane::GetView()
{
	return &m_view;
}

sf::Vector2f Plane::CalculateZoom()
{
	return sf::Vector2f(
		Global::WindowSize.x / m_view.getSize().x,
		Global::WindowSize.y / m_view.getSize().y
	);
}

void Plane::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	throw std::logic_error("use Plane::render instead");
}
