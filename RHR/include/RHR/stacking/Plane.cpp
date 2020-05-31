#include "RHR/registries/ShaderRegistry.h"
#include "Plane.h"

#include <Espresso/Global.h>
#include <Cappuccino/Logger.h>
#include <cassert>
#include <exception>
#include <iostream>

Plane::Plane()
{
	m_collections.reserve(5);
	m_collectionVanity.reserve(5);

	m_vertexArrays.reserve(5);

	// m_innerText displays the inner position of the plane

	m_innerText = sf::Text("0, 0", Global::Font, 12);
	m_innerText.setFillColor(MOD_BUTTON_TEXT_FG);
}

Plane::Plane(const Plane& plane)
{
	for (uint64_t i = 0; i < m_collections.size(); i++)
		delete m_collections[i];

	m_collections.clear();

	for (uint64_t i = 0; i < plane.m_collections.size(); i++)
		m_collections.push_back(new Collection(plane.m_collections[i]));
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
	}
}

const std::vector<Collection*>& Plane::GetCollections()
{
	return m_collections;
}

void Plane::TranslateInnerPosition(const sf::Vector2i& position)
{
	m_innerPosition += position;
}

void Plane::DeleteContents()
{
	for (uint64_t i = 0; i < m_collections.size(); i++)
		delete m_collections[i];

	m_collections.clear();
	m_collectionVanity.clear();
}

void Plane::frameUpdate(const double& deltaTime)
{
	// set innerText to a visual representation of the inner position coordinates

	m_innerText.setPosition(getPosition().x + 5, getPosition().y + getSize().y - 18);
	char innerText[20];
	sprintf(innerText, "%d, %d", m_innerPosition.x, m_innerPosition.y); // TODO make sure "%d" has no trailing "0"s.
 	m_innerText.setString(innerText);

	// transform applies offset to the vertex buffers

	for (uint16_t i = 0; i < m_vertexBufferTransform.size(); i++)
		m_vertexBufferTransform[i].translate((sf::Vector2f)m_innerPosition);
}

const bool Plane::mouseButton(const bool& down, const sf::Vector2i& position, const sf::Mouse::Button& button)
{

}

Plane* Plane::PrimaryPlane;

Plane* Plane::ToolbarPlane;

void Plane::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// render each batch collection

	for (uint16_t i = 0; i < m_vertexBuffers.size(); i++)
		target.draw(m_vertexBuffers[i], m_vertexBufferTransform[i]);

	// render inner position coordinates

	target.draw(m_innerText);
}

void Plane::CreateBuffer(const uint16_t& collectionIdx)
{
	m_vertexArrays.push_back(std::vector<sf::Vertex>());
	m_vertexArrays.back().reserve(VA_DATA_OFFSET + 100);

	// ====================================================================================================================================================
	// =============== add collection geometry to vertex array; see "dev/collection_geometry.png"
	// ====================================================================================================================================================

	const sf::Vector2f& pos = m_collections[collectionIdx]->getPosition();
	const sf::Vector2u& size = m_collections[collectionIdx]->getSize();

	// 0

	m_vertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(), COLLECTION_COLOR_OUTLINE));
	m_vertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(size.x, COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_OUTLINE));
	m_vertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(0, COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_OUTLINE));

	// 1

	m_vertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH, 0), COLLECTION_COLOR_OUTLINE));
	m_vertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_OUTLINE));
	m_vertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(), COLLECTION_COLOR_OUTLINE));

	// 2

	m_vertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH, 0), COLLECTION_COLOR_OUTLINE));
	m_vertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_OUTLINE));
	m_vertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH, 0), COLLECTION_COLOR_OUTLINE));

	// 3

	m_vertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_OUTLINE));
	m_vertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_OUTLINE));
	m_vertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH, 0), COLLECTION_COLOR_OUTLINE));

	// 4

	m_vertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_OUTLINE));
	m_vertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_OUTLINE));
	m_vertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_OUTLINE));

	// 5

	m_vertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_OUTLINE));
	m_vertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_OUTLINE));
	m_vertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_OUTLINE));

	// 6

	m_vertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(0, size.y + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_OUTLINE));
	m_vertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_OUTLINE));
	m_vertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_OUTLINE));

	// 7

	m_vertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(0, COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_OUTLINE));
	m_vertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_OUTLINE));
	m_vertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(0, size.y + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_OUTLINE));

	// 8

	m_vertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_FILL));
	m_vertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_FILL));
	m_vertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_FILL));

	// 9

	m_vertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_FILL));
	m_vertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(size.x + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_FILL));
	m_vertexArrays.back().push_back(sf::Vertex(pos + sf::Vector2f(COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH), COLLECTION_COLOR_FILL));

	// ====================================================================================================================================================
	// =============== create buffer
	// ====================================================================================================================================================

	// "Triangles" because we are batch rendering and can not use the other optimized connected primitive types
	// "Dynamic" because "Occasionally changing data", the buffer will only be changed during a user induced event
	m_vertexBuffers.push_back(sf::VertexBuffer(sf::PrimitiveType::Triangles, sf::VertexBuffer::Usage::Dynamic));
}

void Plane::UpdateBuffer(const uint16_t& bufferIdx)
{
	m_vertexBuffers[bufferIdx].update(&(m_vertexArrays[bufferIdx][0]));
}

/*
Plane::Plane(sf::Vector2u position, sf::Vector2u size, bool toolbar)
{
	m_position = new sf::Vector2u();
	m_innerPosition = new sf::Vector2i();
	m_absolutePosition = new sf::Vector2u();
	*m_position = position;

	m_size = size;
	m_toolbar = toolbar;

	m_background = sf::RectangleShape(sf::Vector2f());
	m_background.setFillColor(sf::Color(30, 30, 30));
	m_background.setPosition(position.x, position.y);
	m_background.setSize((sf::Vector2f)size);

	m_innerPositionText = sf::Text("0, 0", *Global::Font, 12);
	m_innerPositionText.setFillColor(MOD_BUTTON_TEXT_FG);

	m_functionAdd = new std::function<void(Stack* stack)>();
	*m_functionAdd = [&](Stack* stack)
	{
		if (m_toolbar)
		{
			delete stack;
		}
		else
		{
			AddStack(stack);
		}
	};

	m_functionRemove = new std::function<void(Stack* stack)>();
	*m_functionRemove = [&](Stack* stack)
	{
		if (m_toolbar)
		{
			for (unsigned int i = 0; i < m_stacks.size(); i++)
			{
				if (m_stacks[i] == stack)
				{
					m_stacks.erase(m_stacks.begin() + i);
					return;
				}
			}
		}
		else
		{
			for (unsigned int i = 0; i < m_stacks.size(); i++)
			{
				if (m_stacks[i] == stack)
				{
					m_stacks.erase(m_stacks.begin() + i);
					return;
				}
			}
		}
	};

	m_functionMoveTop = new std::function<void(Stack* stack)>();
	*m_functionMoveTop = [&](Stack* stack)
	{
		for (unsigned int i = 0; i < m_stacks.size(); i++)
		{
			if (m_stacks[i] == stack)
			{
				m_stacks.erase(m_stacks.begin() + i);
				m_stacks.push_back(stack);
				return;
			}
		}
	};

	m_functionAddOver = new std::function<void(Stack* stack)>();
	*m_functionAddOver = [&](Stack* stack)
	{
		((Plane*)Global::DraggingPlaneOver)->AddStack(stack);
		stack->SetRelitivePosition(stack->GetSetPosition() - (*m_innerPosition - ((Plane*)Global::DraggingPlaneOver)->GetInnerPosition()));
	};

	m_draggingConnection = sf::RectangleShape(sf::Vector2f(0, 4));
	m_useDraggingConnection = false;
	m_dragging = false;

	if (m_size.x > 0 && m_size.y > 0)
	{
		m_renderBuffer.create(m_size.x, m_size.y);
		m_renderSprite.setTexture(m_renderBuffer.getTexture());
		m_renderSprite.setTextureRect(sf::IntRect(0, static_cast<int32_t>(m_size.y), static_cast<int32_t>(m_size.x), -1 * static_cast<int32_t>(m_size.y)));
		m_renderSprite.setPosition(sf::Vector2f(m_position->x, m_position->y));
	}
	else
		m_renderBuffer.create(10, 10);
}

Plane::~Plane()
{
	delete m_position;
	delete m_functionAdd;
	delete m_functionRemove;
	delete m_functionAddOver;
	delete m_functionMoveTop;
	delete m_innerPosition;
	delete m_absolutePosition;

	for (unsigned int i = 0; i < m_stacks.size(); i++)
	{
		delete m_stacks[i];
	}
}

void Plane::frameUpdate(const double& deltaTime)
{
	throw std::logic_error("The method or operation is not implemented.");
}

const bool Plane::mouseButton(const bool& down, const sf::Vector2i& position, const sf::Mouse::Button& button)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void Plane::draw(RenderTarget& target, RenderStates states) const
{
	throw std::logic_error("The method or operation is not implemented.");
}

void Plane::AddStack(Stack* stack)
{
	if (stack->GetAbsolutePosition() != sf::Vector2i(0, 0))
	{
		sf::Vector2u nPlanePos = stack->GetPlanePosition() - *m_position;
		stack->SetRelitivePosition(((sf::Vector2i)nPlanePos) + stack->GetRelitivePosition());
	}
	
	stack->SetupInPlane(m_position, m_innerPosition, (void*)this, m_functionAdd, m_functionRemove, m_functionMoveTop, m_functionAddOver);
	stack->ReRender();
	m_stacks.push_back(stack);
}

void Plane::Render(sf::RenderWindow* window)
{
	m_renderBuffer.clear(sf::Color(0, 0, 0, 0));

	for (unsigned int i = 0; i < m_stacks.size(); i++)
	{
		m_stacks[i]->Render(&m_renderBuffer, window);

		if (Global::CutRenderingPlane)
		{
			Global::CutRenderingPlane = false;
			break;
		}
	}
	
	if (Global::Dragging && Global::DraggingPlaneOver == this)
	{
		sf::RectangleShape shape = sf::RectangleShape(sf::Vector2f(m_background.getSize().x + 2, m_background.getSize().y + 2));
		shape.setPosition(m_position->x - 1, m_position->y - 1);
		shape.setFillColor(MOD_BUTTON_TEXT_FG);

		window->draw(shape);
	}

	sf::Shader* shader = ShaderRegistry::GetShader("dots");
	//if (Global::GpuType == GpuMan::NVIDIA)
		shader->setUniform("windowSize", sf::Vector2f(200.0f, 200.0f));
	
	shader->setUniform("planeInnerPosition", sf::Glsl::Vec2(m_innerPosition->x / 2, m_innerPosition->y / -2));
	//TODO future settings
	shader->setUniform("dotMod", 30.0f);
	shader->setUniform("backgroundColor", sf::Glsl::Vec3(MOD_BACKGROUND_LOW_C / 255.0f, MOD_BACKGROUND_LOW_C / 255.0f, MOD_BACKGROUND_LOW_C / 255.0f));
	shader->setUniform("dotColor", sf::Glsl::Vec3(MOD_BACKGROUND_HIGH_C / 255.0f, MOD_BACKGROUND_HIGH_C / 255.0f, MOD_BACKGROUND_HIGH_C / 255.0f));

	window->draw(m_background, shader);
	//m_renderSprite.setPosition(10, 500);
	window->draw(m_renderSprite);//all blocks inside the stack

	if (!m_toolbar)
	{
		if (m_useDraggingConnection)
			window->draw(m_draggingConnection);

		window->draw(m_innerPositionText);
	}
}

void Plane::RenderConnection(sf::RenderWindow* window)
{
	if (m_useDraggingConnection)
		window->draw(m_draggingConnection);
}

void Plane::FrameUpdate(BlockRegistry* blockRegistry, bool overrideBounding)
{
	if (m_dragging)
	{
		m_innerPosition->x = (m_draggingMouseStart.x - Global::MousePosition.x) + (int)m_draggingStart.x;
		m_innerPosition->y = (m_draggingMouseStart.y - Global::MousePosition.y) + (int)m_draggingStart.y;
	}
	int64_t x = static_cast<int32_t>(m_position->x) + m_innerPosition->x;
	int64_t y = static_cast<int32_t>(m_position->y) + m_innerPosition->y;
	
	// FIXME: These trigger
	//UB_ASSERT(x > 0);
	//UB_ASSERT(y > 0);

	m_absolutePosition->x = static_cast<uint32_t>(x);
	m_absolutePosition->y = static_cast<uint32_t>(y);

	m_background.setPosition(m_position->x, m_position->y);
	m_background.setSize((sf::Vector2f)m_size);

	m_innerPositionText.setPosition(m_position->x + 5, m_position->y + m_size.y - 18);
	m_innerPositionText.setString(std::to_string(m_innerPosition->x) + ", " + std::to_string(m_innerPosition->y));

	m_useDraggingConnection = false;

	if (Global::Dragging && Global::DraggingPlane == this)
	{
		Global::DraggingStackConnected = nullptr;
		Global::DraggingPlaneOver = nullptr;

		Stack* draggingStack = (Stack*)Global::DraggingStack;

		for (unsigned int i = 0; i < Planes->size(); i++)
		{
			if (draggingStack->GetAbsolutePosition().x > static_cast<int32_t>((*Planes)[i]->GetPosition().x) && draggingStack->GetAbsolutePosition().x < static_cast<int32_t>((*Planes)[i]->GetPosition().x + (*Planes)[i]->GetSize().x) &&
				draggingStack->GetAbsolutePosition().y > static_cast<int32_t>((*Planes)[i]->GetPosition().y) && draggingStack->GetAbsolutePosition().y < static_cast<int32_t>((*Planes)[i]->GetPosition().y + (*Planes)[i]->GetSize().y))
			{
				Global::DraggingPlaneOver = (void*)((*Planes)[i]);
				break;
			}
		}

		if (!blockRegistry->GetBlock(draggingStack->GetBlock(0)->GetUnlocalizedName())->IsTopical())
		{
			if (Global::DraggingPlaneOver != nullptr)
			{
				if (!((Plane*)Global::DraggingPlaneOver)->IsToolbar())
				{
					const std::vector<Stack*>* overPlaneStacks = ((Plane*)Global::DraggingPlaneOver)->GetAllStacks();

					for (unsigned int i = 0; i < overPlaneStacks->size(); i++)
					{
						if ((*overPlaneStacks)[i] == draggingStack)
							continue;

						unsigned int blockCount = (*overPlaneStacks)[i]->GetBlockCount();

						if (draggingStack->GetAbsolutePosition().x > (*overPlaneStacks)[i]->GetAbsolutePosition().x - (int)Global::BlockConnectDistance && draggingStack->GetAbsolutePosition().x < (*overPlaneStacks)[i]->GetAbsolutePosition().x + (int)Global::BlockConnectDistance &&
							draggingStack->GetAbsolutePosition().y >(*overPlaneStacks)[i]->GetAbsolutePosition().y - (int)(Global::BlockHeight / 2) && draggingStack->GetAbsolutePosition().y < (*overPlaneStacks)[i]->GetAbsolutePosition().y + (int)(Global::BlockHeight / 2) + (int)(blockCount * Global::BlockHeight))
						{
							for (unsigned int a = 0; a < blockCount + 1; a++)
							{
								if (draggingStack->GetAbsolutePosition().x > (*overPlaneStacks)[i]->GetAbsolutePosition().x - (int)Global::BlockConnectDistance && draggingStack->GetAbsolutePosition().x < (*overPlaneStacks)[i]->GetAbsolutePosition().x + (int)Global::BlockConnectDistance &&
									draggingStack->GetAbsolutePosition().y >= (*overPlaneStacks)[i]->GetAbsolutePosition().y - (int)(Global::BlockHeight / 2) + (int)(a * Global::BlockHeight) && draggingStack->GetAbsolutePosition().y < (*overPlaneStacks)[i]->GetAbsolutePosition().y + (int)(Global::BlockHeight / 2) + (int)(a * Global::BlockHeight))
								{
									if (a < blockCount && blockRegistry->GetBlock(overPlaneStacks->at(i)->GetBlock(a)->GetUnlocalizedName())->IsTopical())
										continue;

									unsigned int blockWidth = 0;
									if (a == 0)
										blockWidth = (*overPlaneStacks)[i]->GetBlockWidth(0);
									else
										blockWidth = (*overPlaneStacks)[i]->GetBlockWidth(a - 1);

									m_draggingConnection.setSize(sf::Vector2f(blockWidth, Global::BlockBorder / 2));
									m_draggingConnection.setFillColor(sf::Color(200, 200, 200));
									m_draggingConnection.setPosition((*overPlaneStacks)[i]->GetAbsolutePosition().x, (*overPlaneStacks)[i]->GetAbsolutePosition().y + static_cast<int64_t>(a * Global::BlockHeight));
									m_useDraggingConnection = true;

									Global::DraggingStackConnected = (void*)(*overPlaneStacks)[i];
									Global::DraggingStackConnectedIndex = a;

									return;
								}
							}
						}
					}
				}
			}
		}
	}

	for (int64_t i = static_cast<int64_t>(m_stacks.size()) - 1; i >= 0; i--)
		m_stacks[static_cast<uint64_t>(i)]->FrameUpdate(m_stacks[static_cast<uint64_t>(i)]->IsBounding((sf::Vector2f)Global::MousePosition), overrideBounding);
}

void Plane::DeleteAllBlocks()
{
	for (unsigned int i = 0; i < m_stacks.size(); i++)
	{
		m_stacks[i]->~Stack();
	}

	m_stacks.clear();
}

void Plane::CopyEverything(Plane* plane, BlockRegistry* registry)
{
	*m_position = plane->GetPosition();
	*m_innerPosition = plane->GetInnerPosition();
	*m_absolutePosition = plane->GetAbsolutePosition();
	
	for (unsigned int i = 0; i < plane->GetStackCount(); i++)
	{
		Stack* stack = new Stack(sf::Vector2i(0, 0), registry);
		AddStack(stack);
		stack->CopyEverything(plane->GetStack(i), registry);
	}
}

unsigned int Plane::GetStackCount() const
{
	return m_stacks.size();
}

sf::Vector2u Plane::GetSize() const
{
	return m_size;
}

sf::Vector2u Plane::GetPosition() const
{
	return *m_position;
}

sf::Vector2i Plane::GetInnerPosition() const
{
	return *m_innerPosition;
}

void Plane::SetInnerPosition(sf::Vector2i position)
{
	*m_innerPosition = position;
}

sf::Vector2u Plane::GetAbsolutePosition() const
{
	return *m_absolutePosition;
}

void Plane::SetPosition(sf::Vector2u position)
{
	*m_position = position;
	m_renderSprite.setPosition(sf::Vector2f(m_position->x, m_position->y));
}

void Plane::SetSize(sf::Vector2u size)
{
	m_size = size;
	m_renderBuffer.create(m_size.x, m_size.y);
	m_renderSprite.setTexture(m_renderBuffer.getTexture());
	m_renderSprite.setTextureRect(sf::IntRect(0, static_cast<int32_t>(m_size.y), static_cast<int32_t>(m_size.x), -1 * static_cast<int32_t>(m_size.y)));
}

Stack* Plane::GetStack(unsigned int index) const
{
	return m_stacks[index];
}

bool Plane::IsToolbar() const
{
	return m_toolbar;
}

const std::vector<Stack*>* Plane::GetAllStacks() const
{
	return &m_stacks;
}

void Plane::ReloadVanity()
{
	for (uint32_t i = 0; i < m_stacks.size(); i++)
		m_stacks[i]->ReloadVanity();
}

void Plane::AddInnerPosition(const sf::Vector2i& change)
{
	*m_innerPosition += change;
}

void Plane::MouseButton(bool down, sf::Vector2i position, sf::Mouse::Button button)
{
	if (position.x > (int32_t)m_position->x&& position.x < (int32_t)(m_position->x + m_size.x) &&
		position.y > (int32_t)m_position->y&& position.y < (int32_t)(m_position->y + m_size.y))
	{
		if (m_dragging && !down)
		{
			m_dragging = false;
			return;
		}

		bool drag = true;

		for (int64_t i = static_cast<int64_t>(m_stacks.size()) - 1; i >= 0; i--)
		{
			if ((m_stacks[static_cast<uint64_t>(i)]->IsBounding((sf::Vector2f)position) || m_stacks[i] == Global::SelectedStack) && m_stacks[static_cast<uint64_t>(i)]->MouseButton(down, position, button))
			{
				drag = false;
				break;
			}
		}

		if (drag && down && !m_toolbar && button == sf::Mouse::Left)
		{
			m_dragging = true;
			m_draggingMouseStart = position;
			m_draggingStart = *m_innerPosition;

			Global::ContextActive = false;
			if (Global::SelectedArgument != nullptr)
			{
				Argument* gArg = (Argument*)Global::SelectedArgument;
				Global::SelectedArgument = nullptr;
				gArg->FrameUpdate();
				gArg->UpdateTexture();
			}
			else
				Global::SelectedArgument = nullptr;
		}
	}
}

std::vector<Plane*>* Plane::Planes;
*/
