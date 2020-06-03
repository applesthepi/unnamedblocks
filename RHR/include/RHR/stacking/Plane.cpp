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

Plane::Plane(const Plane& plane)
{
	Setup();

	for (uint64_t i = 0; i < m_collections.size(); i++)
		delete m_collections[i];

	m_collections.clear();

	for (uint64_t i = 0; i < plane.m_collections.size(); i++)
		m_collections.push_back(new Collection(*plane.m_collections[i]));
}

Plane::Plane()
{
	Setup();

	m_collections.reserve(5);
	m_collectionVanity.reserve(5);

	m_vertexArrays.reserve(5);
	m_collectionVertexArrays.reserve(5);

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

void Plane::DeleteCollection(uint64_t idx)
{
	m_collectionVertexArrays.erase(m_collectionVertexArrays.begin() + idx);
	m_vertexArrays.erase(m_vertexArrays.begin() + idx);
	m_vertexBuffers.erase(m_vertexBuffers.begin() + idx);
	m_vertexBufferTransform.erase(m_vertexBufferTransform.begin() + idx);
	m_textureMapImage.erase(m_textureMapImage.begin() + idx);
	m_textureMapTexture.erase(m_textureMapTexture.begin() + idx);
	m_textureMapEnabled.erase(m_textureMapEnabled.begin() + idx);
	m_collectionVanity.erase(m_collectionVanity.begin() + idx);

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

void Plane::DeleteContents()
{
	for (uint64_t i = 0; i < m_collections.size(); i++)
		DeleteCollection(0);
}

void Plane::frameUpdate(double deltaTime)
{
	// set innerText to a visual representation of the inner position coordinates

	m_innerText.setPosition(getPosition().x + 5, getPosition().y + getSize().y - 18);
	char innerText[20];
	sprintf(innerText, "%d, %d", m_innerPosition.x, m_innerPosition.y); // TODO make sure "%d" has no trailing "0"s.
 	m_innerText.setString(innerText);

	// transform applies offset to the vertex buffers

	for (uint16_t i = 0; i < m_vertexBufferTransform.size(); i++)
		m_vertexBufferTransform[i].translate((sf::Vector2f)m_innerPosition);

	// dragging stack

	if (DraggingStack())
		m_draggingStack->setPosition(getPosition() + m_draggingBeginObject + (sf::Vector2f)(Global::MousePosition - m_draggingBeginMouse));

	// dragging collection

	if (DraggingCollection())
		m_draggingCollection->setPosition(getPosition() + m_draggingBeginObject + (sf::Vector2f)(Global::MousePosition - m_draggingBeginMouse));
}

bool Plane::mouseButton(bool down, const sf::Vector2i& position, const sf::Mouse::Button& button)
{
	for (uint64_t i = 0; i < m_collections.size(); i++)
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

				const sf::Vector2u& stackSize = m_collections[i]->GetStacks()[a]->getSize();

				// position

				sf::Vector2f stackPosition = m_collections[i]->GetStacks()[a]->getPosition();
				stackPosition += getPosition();
				stackPosition += collectionPosition;

				if (position.x > stackPosition.x && position.x < stackPosition.x + stackSize.x &&
					position.y > stackPosition.y && position.y < stackPosition.y + stackSize.y)
				{
					for (uint64_t b = 0; b < m_collections[i]->GetStacks()[a]->GetBlocks().size(); b++)
					{
						// size

						const sf::Vector2u& blockSize = m_collections[i]->GetStacks()[a]->GetBlocks()[b]->getSize();

						// position

						sf::Vector2f blockPosition = m_collections[i]->GetStacks()[a]->GetBlocks()[b]->getPosition();
						blockPosition += getPosition();
						blockPosition += collectionPosition;
						blockPosition += stackPosition;

						if (position.x > blockPosition.x && position.x < blockPosition.x + stackSize.x &&
							position.y > blockPosition.y && position.y < blockPosition.y + stackSize.y)
						{
							if (down && !DraggingStack())
							{
								// not dragging and mouse down

								if (button == sf::Mouse::Button::Left)
								{
									// remove context menu

									ContextHandler::Disable();
									UnSelect();

									Stack* currentStack = m_collections[i]->GetStacks()[a];

									if (b > 0)
									{
										// create stack that is left behind (no the one picked up)

										Stack* stack = new Stack();
										stack->setPosition(currentStack->getPosition());
										
										for (uint64_t c = 0; c < b; c++)
											stack->AddBlock(m_collections[i]->GetStacks()[a]->GetBlocks()[c]);

										m_collections[i]->AddStack(stack);

										for (uint64_t c = 0; c < b; c++)
											currentStack->RemoveBlock(0);

										currentStack->setPosition(stack->getPosition() + sf::Vector2f(0, b * Global::BlockHeight));
									}

									// remove it from vertex array

									m_collections[i]->RemoveStack(a);
									UpdateBuffer(i);

									// drag the current stack (excludes the stack that was left behind)

									DragStack(i, a, false);
								}
								else if (button == sf::Mouse::Button::Right)
								{
									// startup context menu on block

									ContextHandler::Disable();
									ContextHandler::Enable((sf::Vector2f)position + sf::Vector2f(5, 5), &m_contextCallback);
									SelectContext(i, a, b);
								}
							}
							else if (!down && DraggingStack() && !m_draggingUp)
							{
								// dragging and mouse released (used when dragging a stack)

								UnDrag();
							}
							else if (down && DraggingStack() && m_draggingUp)
							{
								// dragging and mouse pressed (used when duplicating stack)

								UnDrag();
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
	// render each batch collection

	for (uint16_t i = 0; i < m_vertexBuffers.size(); i++)
	{
		// setup render

		sf::RenderStates states;
		states.transform = m_vertexBufferTransform[i];

		if (m_textureMapEnabled[i])
			m_shader.setUniform("texture", m_textureMapTexture[i]);

		states.shader = &m_shader;

		// render VBO

		window.draw(m_vertexBuffers[i], states);
	}

	// render inner position coordinates

	window.draw(m_innerText);
}

void Plane::Setup()
{
	m_selected = false;
	m_selectedContext = false;

	m_draggingCollection = nullptr;
	m_draggingStack = nullptr;
	m_draggingUp = false;

	m_shader.loadFromFile("res/shaders/blocks.vert", "res/shaders/blocks.frag");
}

void Plane::CreateBuffer(uint16_t collectionIdx, bool displayCollectionVanity)
{
	m_vertexArrays.push_back(std::vector<sf::Vertex>());
	m_vertexArrays.back().reserve(200);

	m_collectionVertexArrays.push_back(std::vector<sf::Vertex>());

	if (displayCollectionVanity)
	{
		m_collectionVertexArrays.back().reserve(24);

		// ====================================================================================================================================================
		// =============== add collection geometry to vertex array; see "dev/collection_geometry.png"
		// ====================================================================================================================================================

		const sf::Vector2f& pos = m_collections[collectionIdx]->getPosition();
		const sf::Vector2u& size = m_collections[collectionIdx]->getSize();

		// 0

		m_collectionVertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(), COLLECTION_COLOR_OUTLINE));
		m_collectionVertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(size.x, COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_OUTLINE));
		m_collectionVertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(0, COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_OUTLINE));

		// 1

		m_collectionVertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH, 0), COLLECTION_COLOR_OUTLINE));
		m_collectionVertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_OUTLINE));
		m_collectionVertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(), COLLECTION_COLOR_OUTLINE));

		// 2

		m_collectionVertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH, 0), COLLECTION_COLOR_OUTLINE));
		m_collectionVertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_OUTLINE));
		m_collectionVertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH, 0), COLLECTION_COLOR_OUTLINE));

		// 3

		m_collectionVertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_OUTLINE));
		m_collectionVertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_OUTLINE));
		m_collectionVertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH, 0), COLLECTION_COLOR_OUTLINE));

		// 4

		m_collectionVertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_OUTLINE));
		m_collectionVertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_OUTLINE));
		m_collectionVertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_OUTLINE));

		// 5

		m_collectionVertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_OUTLINE));
		m_collectionVertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_OUTLINE));
		m_collectionVertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_OUTLINE));

		// 6

		m_collectionVertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(0, size.y + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_OUTLINE));
		m_collectionVertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_OUTLINE));
		m_collectionVertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_OUTLINE));

		// 7

		m_collectionVertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(0, COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_OUTLINE));
		m_collectionVertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_OUTLINE));
		m_collectionVertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(0, size.y + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_OUTLINE));

		// 8

		m_collectionVertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_FILL));
		m_collectionVertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_FILL));
		m_collectionVertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_FILL));

		// 9

		m_collectionVertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_FILL));
		m_collectionVertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_FILL));
		m_collectionVertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_FILL));
	}

	// ====================================================================================================================================================
	// =============== Create Vertex Buffer
	// ====================================================================================================================================================

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
	// clear vertex array

	m_vertexArrays[bufferIdx].clear();

	// add template vertex array (collection decoration)

	for (uint8_t i = 0; i < m_collectionVertexArrays[bufferIdx].size(); i++)
		m_vertexArrays[bufferIdx].push_back(m_collectionVertexArrays[bufferIdx][i]);

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

			uint16_t blockWidth = Global::BlockBorder;

			sf::Vector2f blockPos = getPosition() + m_collections[bufferIdx]->getPosition() + m_collections[bufferIdx]->GetStacks()[a]->getPosition() + sf::Vector2f(0, b * Global::BlockHeight);
			uint32_t blockShellWidth = m_collections[bufferIdx]->GetStacks()[a]->GetBlocks()[b]->GetWidth();
			const sf::Color blockColor = m_collections[bufferIdx]->GetStacks()[a]->GetBlocks()[b]->GetModCategory()->GetColor();

			m_vertexArrays[bufferIdx].push_back(sf::Vertex(blockPos + sf::Vector2f(0, 0), blockColor, sf::Vector2f(0, 0)));
			vaBox.back().push_back(&m_vertexArrays[bufferIdx].back());
			m_vertexArrays[bufferIdx].push_back(sf::Vertex(blockPos + sf::Vector2f(blockShellWidth, 0), blockColor, sf::Vector2f(1, 0)));
			vaBox.back().push_back(&m_vertexArrays[bufferIdx].back());
			m_vertexArrays[bufferIdx].push_back(sf::Vertex(blockPos + sf::Vector2f(blockShellWidth, Global::BlockHeight), blockColor, sf::Vector2f(1, 1)));
			vaBox.back().push_back(&m_vertexArrays[bufferIdx].back());

			m_vertexArrays[bufferIdx].push_back(sf::Vertex(blockPos + sf::Vector2f(0, 0), blockColor, sf::Vector2f(0, 0)));
			vaBox.back().push_back(&m_vertexArrays[bufferIdx].back());
			m_vertexArrays[bufferIdx].push_back(sf::Vertex(blockPos + sf::Vector2f(blockShellWidth, Global::BlockHeight), blockColor, sf::Vector2f(1, 1)));
			vaBox.back().push_back(&m_vertexArrays[bufferIdx].back());
			m_vertexArrays[bufferIdx].push_back(sf::Vertex(blockPos + sf::Vector2f(0, Global::BlockHeight), blockColor, sf::Vector2f(0, 1)));
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

					va.back()[d].position += sf::Vector2f(blockWidth + blockPos.x, blockPos.y + ((float)Global::BlockBorder / 2.0f));
				}

				blockWidth += arg->GetWidth();
				blockWidth += Global::BlockBorder;
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
		// if the argument doesnt use a texture

		if (vaTextures[a] == nullptr)
			continue;

		// make texture wider if its to small

		if (vaTextures[a]->getSize().x > textureWidth)
			textureWidth = vaTextures[a]->getSize().x;

		// offset non-normalized texture coords

		for (uint64_t b = 0; b < va[a].getVertexCount(); b++)
		{
			if (vaCoords[a]->at(b))
				va[a][b].texCoords.y += textureHeight;
		}

		textureHeight += vaTextures[a]->getSize().y;
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
		textureMap.clear(sf::Color(255, 255, 255, 255));

		sf::Texture tempTexture;
		sf::Sprite tempSprite;

		uint32_t incHeight = 0;

		for (uint64_t a = 0; a < vaTextures.size(); a++)
		{
			// proceed if the argument has a texture
		
			if (vaTextures[a] == nullptr)
				continue;
			
			tempTexture.loadFromImage(*(vaTextures[a]));
		
			tempSprite.setTexture(tempTexture);
			tempSprite.setPosition(0, incHeight);
		
			textureMap.draw(tempSprite);
			incHeight += vaTextures[a]->getSize().y;
		}

		// retrieve render

		m_textureMapImage[bufferIdx] = textureMap.getTexture().copyToImage();
		
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

void Plane::DragCollection(uint64_t collection, bool up)
{
	if (m_draggingStack != nullptr || m_draggingCollection != nullptr)
		return;

	m_draggingUp = up;

	m_draggingCollection = m_collections[collection];
	m_draggingStack = nullptr;

	m_draggingBeginMouse = Global::MousePosition;
	m_draggingBeginObject = m_draggingCollection->getPosition();
}

void Plane::DragStack(uint64_t collection, uint64_t stack, bool up)
{
	if (m_draggingStack != nullptr || m_draggingCollection != nullptr)
		return;

	m_draggingUp = up;

	m_draggingCollection = m_collections[collection];
	m_draggingStack = m_collections[collection]->GetStacks()[stack];

	m_draggingBeginMouse = Global::MousePosition;
	m_draggingBeginObject = m_draggingStack->getPosition();
}

void Plane::UnDrag()
{
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
