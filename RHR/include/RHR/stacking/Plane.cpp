#include "RHR/registries/ShaderRegistry.h"
#include "RHR/handlers/ContextHandler.h"
#include "Plane.h"

#include <Espresso/Global.h>
#include <Cappuccino/Logger.h>
#include <SFML/System/Vector2.hpp>
#include <cassert>
#include <exception>
#include <iostream>
#include <vector>

Plane::Plane(bool toolbar, const Plane& plane)
{
	Setup(toolbar);

	for (uint64_t i = 0; i < m_collections.size(); i++)
		delete m_collections[i];

	m_collections.clear();

	for (uint64_t i = 0; i < plane.m_collections.size(); i++)
		m_collections.push_back(new Collection(*plane.m_collections[i]));
}

Plane::Plane(bool toolbar)
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

void Plane::AddCollections(const std::vector<Collection*>& collections)
{
	if (m_collections.size() + collections.size() >= m_collections.capacity())
	{
		size_t ncap = (uint64_t)std::ceil((float)(m_collections.size() + collections.size()) * 1.5f + 10.0f);

		m_collections.reserve(ncap);
		m_collectionVanity.reserve(ncap);
	}

	for (uint64_t i = 0; i < collections.size(); i++)
	{
		m_collections.push_back(collections[i]);
		m_collectionVanity.push_back(true);

		CreateBuffer(m_collections.size() - 1, true);
	}
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

const sf::Vector2i& Plane::GetInnerPosition()
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

	m_innerText.setPosition(getPosition().x + 5, getPosition().y + getSize().y - 18);
	char innerText[20];
	sprintf(innerText, "%d, %d", m_innerPosition.x, m_innerPosition.y); // TODO make sure "%d" has no trailing "0"s.
	m_innerText.setString(innerText);

	// dragging stack

	if (DraggingStack())
	{
		m_draggingCollection->setPosition(m_draggingBeginObject + (sf::Vector2f)(Global::MousePosition - m_draggingBeginMouse));
		//std::cout << "col pos: " << m_draggingCollection->getPosition().x << ", " << m_draggingCollection->getPosition().y << std::endl;
	}

	// dragging collection

	if (DraggingCollection())
		m_draggingCollection->setPosition(m_draggingBeginObject + (sf::Vector2f)(Global::MousePosition - m_draggingBeginMouse));

	// update dragging buffer

	//if (DraggingCollection() || DraggingStack())
	//	UpdateBuffer(m_collections.size() - 1);

	// transform applies offset to the vertex buffers

	for (uint64_t i = 0; i < m_vertexBufferTransform.size(); i++)
		m_vertexBufferTransform[i] = sf::Transform().translate((sf::Vector2f)m_innerPosition).translate((sf::Vector2f)getPosition()).translate(m_collections[i]->getPosition());
}

bool Plane::mouseButton(bool down, const sf::Vector2i& position, const sf::Mouse::Button& button)
{
	uint64_t collectionMax = m_collections.size();

	if (DraggingStack() || DraggingCollection())
		collectionMax--;

	if (DraggingStack() || DraggingCollection())
	{
		if (!down && DraggingStack() && !m_draggingUp)
		{
			Logger::Debug("dragging of stack released");

			// dragging and mouse released (used when dragging a stack)

			UnDrag(position);
		}
		else if (down && DraggingStack() && m_draggingUp)
		{
			Logger::Debug("dragging of stack released");

			// dragging and mouse pressed (used when duplicating stack)

			UnDrag(position);
		}
	}

	for (uint64_t i = 0; i < collectionMax; i++)
	{
		// size
		
		const sf::Vector2u& collectionSize = m_collections[i]->getSize();
		
		// position

		sf::Vector2f collectionPosition = m_collections[i]->getPosition();
		collectionPosition += getPosition();

		if (position.x > collectionPosition.x && position.x < collectionPosition.x + collectionSize.x &&
			position.y > collectionPosition.y && position.y < collectionPosition.y + collectionSize.y)
		{
			for (uint64_t a = 0; a < m_collections[i]->GetStacks().size(); a++)
			{
				// size

				sf::Vector2u stackSize;

				for (uint64_t b = 0; b < m_collections[i]->GetStacks()[a]->GetBlocks().size(); b++)
				{
					if (m_collections[i]->GetStacks()[a]->GetBlocks()[b]->GetWidth() > stackSize.x)
						stackSize.x = m_collections[i]->GetStacks()[a]->GetBlocks()[b]->GetWidth();
				}

				stackSize.y = m_collections[i]->GetStacks()[a]->GetBlocks().size() * Global::BlockHeight;

				// position

				sf::Vector2f stackPosition = m_collections[i]->GetStacks()[a]->getPosition();
				stackPosition += collectionPosition;

				if (position.x > stackPosition.x && position.x < stackPosition.x + stackSize.x &&
					position.y > stackPosition.y && position.y < stackPosition.y + stackSize.y)
				{
					for (uint64_t b = 0; b < m_collections[i]->GetStacks()[a]->GetBlocks().size(); b++)
					{
						// size

						sf::Vector2u blockSize;

						blockSize.x = m_collections[i]->GetStacks()[a]->GetBlocks()[b]->GetWidth();
						blockSize.y = Global::BlockHeight;

						// position

						sf::Vector2f blockPosition = m_collections[i]->GetStacks()[a]->GetBlocks()[b]->getPosition();
						blockPosition += stackPosition;

						if (position.x > blockPosition.x && position.x < blockPosition.x + stackSize.x &&
							position.y > blockPosition.y && position.y < blockPosition.y + stackSize.y)
						{
							Logger::Debug("mouseUpdate inside block");

							if (down && !DraggingStack())
							{
								// not dragging and mouse down

								if (button == sf::Mouse::Button::Left)
								{
									Logger::Debug("mouse down on block");

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

									activeCollection->setPosition(m_collections[i]->getPosition() + sf::Vector2f(0, b * Global::BlockHeight) + (sf::Vector2f)activeStack->getPosition());
									
									if (m_toolbar)
										activeCollection->setSize(sf::Vector2u(activeStack->GetWidestBlock() + 100, activeStack->GetBlocks().size() * Global::BlockHeight + 100));
									else
										activeCollection->setSize(m_collections[i]->getSize());

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
									ContextHandler::Enable((sf::Vector2f)position + sf::Vector2f(5, 5), &m_contextCallback);
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
#if FALSE
	if (m_textureMapTexture.size() < 1)
		return;
	
	sf::Sprite sp;
	sp.setTexture(m_textureMapTexture[0]);
	sp.setPosition(500, 50);
	
	window.draw(sp);
	return;
#endif

	uint16_t collectionMax = m_collections.size();

	if (DraggingStack() || DraggingCollection())
		collectionMax--;

	// render each batch collection

	for (uint16_t i = 0; i < collectionMax; i++)
	{
		if (!m_collections[i]->GetEnabled())
			continue;

		// setup render

		sf::RenderStates states;
		states.transform = m_vertexBufferTransform[i];
		//states.transform.translate(getPosition());

		if (m_textureMapEnabled[i])
			m_shader.setUniform("texture", m_textureMapTexture[i]);

		states.shader = &m_shader;

		// render VBO
		window.draw(m_vertexBuffers[i], states);
	}

	// render inner position coordinates

	window.draw(m_innerText);
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

		window.draw(m_vertexBuffers.back(), states);
	}
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
		size.x + COLLECTION_OUTLINE_WIDTH, COLLECTION_TAB_PART,
		// 5
		size.x + (COLLECTION_OUTLINE_WIDTH * 2), COLLECTION_TAB_PART,
		size.x + (COLLECTION_OUTLINE_WIDTH * 2), COLLECTION_TAB_PART + size.y + COLLECTION_OUTLINE_WIDTH,
		size.x + (COLLECTION_OUTLINE_WIDTH * 2), COLLECTION_TAB_PART + size.y + (COLLECTION_OUTLINE_WIDTH * 2),
		COLLECTION_OUTLINE_WIDTH, COLLECTION_TAB_PART + size.y + (COLLECTION_OUTLINE_WIDTH * 2),
		0.0f, COLLECTION_TAB_PART + size.y + (COLLECTION_OUTLINE_WIDTH * 2),
		// 10
		0.0f, COLLECTION_OUTLINE_WIDTH,
		COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH,
		COLLECTION_OUTLINE_WIDTH + (COLLECTION_TAB_PART * COLLECTION_TAB_PART_COUNT), COLLECTION_OUTLINE_WIDTH,
		COLLECTION_OUTLINE_WIDTH + (COLLECTION_TAB_PART * COLLECTION_TAB_PART_COUNT), COLLECTION_TAB_PART,
		COLLECTION_OUTLINE_WIDTH + (COLLECTION_TAB_PART * COLLECTION_TAB_PART_COUNT), COLLECTION_TAB_PART + COLLECTION_OUTLINE_WIDTH,
		// 15
		size.x + COLLECTION_OUTLINE_WIDTH, COLLECTION_TAB_PART + COLLECTION_OUTLINE_WIDTH,
		size.x + COLLECTION_OUTLINE_WIDTH, COLLECTION_TAB_PART + COLLECTION_OUTLINE_WIDTH + size.y,
		COLLECTION_OUTLINE_WIDTH, COLLECTION_TAB_PART + COLLECTION_OUTLINE_WIDTH + size.y,
		COLLECTION_OUTLINE_WIDTH, COLLECTION_TAB_PART + COLLECTION_OUTLINE_WIDTH,
		// 19 (tab texture)
		COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH,
		COLLECTION_OUTLINE_WIDTH + (COLLECTION_TAB_PART * COLLECTION_TAB_PART_COUNT), COLLECTION_OUTLINE_WIDTH,
		COLLECTION_OUTLINE_WIDTH + (COLLECTION_TAB_PART * COLLECTION_TAB_PART_COUNT), COLLECTION_TAB_PART + COLLECTION_OUTLINE_WIDTH,
		COLLECTION_OUTLINE_WIDTH, COLLECTION_TAB_PART + COLLECTION_OUTLINE_WIDTH,
		// 23 (background)
		COLLECTION_OUTLINE_WIDTH, COLLECTION_TAB_PART + COLLECTION_OUTLINE_WIDTH,
		size.x + COLLECTION_OUTLINE_WIDTH, COLLECTION_TAB_PART + COLLECTION_OUTLINE_WIDTH,
		size.x + COLLECTION_OUTLINE_WIDTH, COLLECTION_TAB_PART + COLLECTION_OUTLINE_WIDTH + size.y,
		COLLECTION_OUTLINE_WIDTH, COLLECTION_TAB_PART + COLLECTION_OUTLINE_WIDTH + size.y,
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

	for (uint8_t i = 0; i < 31 * 2; i++)
	{
		if (i % 2 == 0)
			positions[i] -= COLLECTION_OUTLINE_WIDTH;
		else
			positions[i] -= COLLECTION_OUTLINE_WIDTH + COLLECTION_TAB_PART;
	}

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
	// clean and update collection VAO
	
	m_vertexArrays[bufferIdx].clear();

	if (m_collectionVanity[bufferIdx])
		UpdateCollectionVAO(&(m_vertexArrays[bufferIdx]), m_collections[bufferIdx]->getSize());

	// pull vertex buffers from children

	std::vector<sf::VertexArray> va;
	std::vector<const std::vector<bool>*> vaCoords;
	std::vector<const sf::Image*> vaTextures;
	std::vector<std::vector<sf::Vertex*>> vaBox;

	for (uint64_t a = 0; a < m_collections[bufferIdx]->GetStacks().size(); a++)
	{
		for (uint64_t b = 0; b < m_collections[bufferIdx]->GetStacks()[a]->GetBlocks().size(); b++)
		{
			vaBox.push_back(std::vector<sf::Vertex*>());

			// add block vertices

			float blockWidth = static_cast<float>(Global::BlockBorder);

			sf::Vector2f blockPos = m_collections[bufferIdx]->GetStacks()[a]->getPosition() + sf::Vector2f(0, b * Global::BlockHeight);
			uint32_t blockShellWidth = m_collections[bufferIdx]->GetStacks()[a]->GetBlocks()[b]->GetWidth();
			const sf::Color blockColor = m_collections[bufferIdx]->GetStacks()[a]->GetBlocks()[b]->GetModCategory()->GetColor();

			m_vertexArrays[bufferIdx].push_back(sf::Vertex(blockPos + sf::Vector2f(0, 0), blockColor, sf::Vector2f(0, 0)));
			vaBox.back().push_back(&m_vertexArrays[bufferIdx].back());
			m_vertexArrays[bufferIdx].push_back(sf::Vertex(blockPos + sf::Vector2f(blockShellWidth, 0), blockColor, sf::Vector2f(0, 0)));
			vaBox.back().push_back(&m_vertexArrays[bufferIdx].back());
			m_vertexArrays[bufferIdx].push_back(sf::Vertex(blockPos + sf::Vector2f(blockShellWidth, Global::BlockHeight), blockColor, sf::Vector2f(0, 0)));
			vaBox.back().push_back(&m_vertexArrays[bufferIdx].back());

			m_vertexArrays[bufferIdx].push_back(sf::Vertex(blockPos + sf::Vector2f(0, 0), blockColor, sf::Vector2f(0, 0)));
			vaBox.back().push_back(&m_vertexArrays[bufferIdx].back());
			m_vertexArrays[bufferIdx].push_back(sf::Vertex(blockPos + sf::Vector2f(blockShellWidth, Global::BlockHeight), blockColor, sf::Vector2f(0, 0)));
			vaBox.back().push_back(&m_vertexArrays[bufferIdx].back());
			m_vertexArrays[bufferIdx].push_back(sf::Vertex(blockPos + sf::Vector2f(0, Global::BlockHeight), blockColor, sf::Vector2f(0, 0)));
			vaBox.back().push_back(&m_vertexArrays[bufferIdx].back());

			// get argument information

			for (uint64_t c = 0; c < m_collections[bufferIdx]->GetStacks()[a]->GetBlocks()[b]->GetArguments().size(); c++)
			{
				Argument* arg = m_collections[bufferIdx]->GetStacks()[a]->GetBlocks()[b]->GetArguments()[c];

				va.push_back(*(arg->GetVertexArray()));
				vaCoords.push_back(arg->GetVertexArrayCoords());
				vaTextures.push_back(arg->GetVertexArrayTexture());

				for (uint64_t d = 0; d < va.back().getVertexCount(); d++)
				{
					// make VAO absolute

					va.back()[d].position += sf::Vector2f(static_cast<float>(blockWidth) + blockPos.x, blockPos.y + ((float)Global::BlockBorder / 2.0f));
				}

				blockWidth += static_cast<float>(arg->GetWidth());
				blockWidth += static_cast<float>(Global::BlockBorder);
			}

			// for every block
		}

		// for every stack
	}

	// change argument textureCoords to fit map of textures

	uint32_t textureHeight = 1;
	uint32_t textureWidth = 0;

	for (uint64_t a = 0; a < vaTextures.size(); a++)
	{
		uint64_t tIdx = (vaTextures.size() - a) - 1;

		// if the argument doesnt use a texture

		if (vaTextures[tIdx] == nullptr)
			continue;

		// make texture wider if its to small

		if (vaTextures[tIdx]->getSize().x > textureWidth)
			textureWidth = vaTextures[tIdx]->getSize().x;

		// offset non-normalized texture coords

		for (uint64_t b = 0; b < va[tIdx].getVertexCount(); b++)
		{
			if (vaCoords[tIdx]->at(b))
				va[tIdx][b].texCoords.y += textureHeight;
		}

		textureHeight += vaTextures[tIdx]->getSize().y;
	}

	if (m_collectionVanity[bufferIdx])
	{
		// TODO replace with intrinsic functions

		for (uint8_t i = 0; i < 6; i++)
		{
			m_vertexArrays[bufferIdx][20 * 3 + i].texCoords.y += textureHeight;
		}

		textureHeight += m_textureCollectionOpen.getSize().y;

		if (m_textureCollectionOpen.getSize().x > textureWidth)
			textureWidth = m_textureCollectionOpen.getSize().x;
	}

	// get power of 2 texture

	uint8_t powI = 8;
	sf::Vector2u powUnits(0, 0);
	uint64_t powCurrent = 0;

	while (powUnits.x == 0 || powUnits.y == 0)
	{
		powCurrent = std::pow(2, powI);

		if (textureWidth < powCurrent)
			powUnits.x = powCurrent;

		if (textureHeight < powCurrent)
			powUnits.y = powCurrent;

		powI++;
	}

	textureWidth = powCurrent;
	textureHeight = powCurrent;

	if (m_collectionVanity[bufferIdx])
	{
		// normalize

		for (uint8_t a = 0; a < 22 * 3; a++)
			m_vertexArrays[bufferIdx][a].texCoords = sf::Vector2f(m_vertexArrays[bufferIdx][a].texCoords.x / (float)textureWidth, m_vertexArrays[bufferIdx][a].texCoords.y / (float)textureHeight);
	}

	// normalize

	for (uint64_t a = 0; a < vaTextures.size(); a++)
	{
		for (uint64_t b = 0; b < va[a].getVertexCount(); b++)
			va[a][b].texCoords = sf::Vector2f(va[a][b].texCoords.x / (float)textureWidth, va[a][b].texCoords.y / (float)textureHeight);
	}

	// normalize

	for (uint64_t a = 0; a < vaBox.size(); a++)
	{
		for (uint8_t b = 0; b < 6; b++)
			vaBox[a][b]->texCoords = sf::Vector2f(vaBox[a][b]->texCoords.x / (float)textureWidth, vaBox[a][b]->texCoords.y / (float)textureHeight);
	}

	if (textureWidth > 0 && textureHeight > 0)
	{
		// create map

		sf::RenderTexture textureMap;
		textureMap.create(textureWidth, textureHeight);
		textureMap.clear(sf::Color(0, 0, 0, 0));

		sf::Texture tempTexture;
		sf::Sprite tempSprite;

		int64_t incHeight = textureHeight;

		for (uint64_t a = 0; a < vaTextures.size(); a++)
		{
			// proceed if the argument has a texture

			uint64_t tIdx = (vaTextures.size() - a) - 1;

			if (vaTextures[tIdx] == nullptr)
				continue;
			
			tempTexture.loadFromImage(*(vaTextures[tIdx]));
			incHeight -= vaTextures[tIdx]->getSize().y;

			tempSprite.setTexture(tempTexture, true);
			tempSprite.setPosition(0, incHeight);
			
			textureMap.draw(tempSprite);
		}

		// render collection tab

		if (m_collectionVanity[bufferIdx])
		{
			incHeight -= m_textureCollectionOpen.getSize().y;

			tempSprite.setTexture(m_textureCollectionOpen, true);
			tempSprite.setPosition(0, incHeight);

			textureMap.draw(tempSprite);
		}

		// retrieve render

		m_textureMapImage[bufferIdx] = textureMap.getTexture().copyToImage();
		m_textureMapImage[bufferIdx].setPixel(0, 0, sf::Color::White);
		
		m_textureMapTexture[bufferIdx].loadFromImage(m_textureMapImage[bufferIdx]);
		m_textureMapEnabled[bufferIdx] = true;
	}
	else
		m_textureMapEnabled[bufferIdx] = false;

	// add argument's vertices

	for (uint64_t i = 0; i < va.size(); i++)
	{
		for (uint64_t a = 0; a < va[i].getVertexCount(); a++)
			m_vertexArrays[bufferIdx].push_back(va[i][a]);
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

	//std::cout << "starting pos " << m_draggingBeginObject.x << ", " << m_draggingBeginObject.y << std::endl;
}

void Plane::UnDrag(const sf::Vector2i& position)
{
	if (DraggingStack())
	{
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

				sf::Vector2f colPos = Plane::PrimaryPlane->GetCollections()[i]->getPosition() + Plane::PrimaryPlane->getPosition();
				sf::Vector2f colSize = (sf::Vector2f)Plane::PrimaryPlane->GetCollections()[i]->getSize();

				if (position.x > colPos.x && position.x < colPos.x + colSize.x &&
					position.y > colPos.y && position.y < colPos.y + colSize.y)
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
				m_draggingCollection->setPosition((m_draggingBeginObject + (sf::Vector2f)(Global::MousePosition - m_draggingBeginMouse)) - Plane::PrimaryPlane->getPosition() + getPosition());

				DeleteCollection(m_collections.size() - 1, false);
				Plane::PrimaryPlane->AddCollection(m_draggingCollection, true);
			}
		}
	}

	m_draggingUp = false;
	m_draggingCollection = nullptr;
	m_draggingStack = nullptr;
}

bool Plane::DraggingCollection()
{
	return m_draggingCollection != nullptr && m_draggingStack == nullptr;
}

bool Plane::DraggingStack()
{
	return m_draggingCollection != nullptr && m_draggingStack != nullptr;
}

void Plane::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	throw std::logic_error("use Plane::render instead");
}
