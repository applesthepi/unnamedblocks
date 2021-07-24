#include "plane.hpp"

// #include "rhr/registries/ShaderRegistry.hpp"
#include "rhr/handlers/context.hpp"
#include "rhr/rendering/renderer.hpp"

// TODO: remove include (used for testing)
#include <iostream>

#ifdef ENABLE_AVX2
#ifdef WIN32
#include <immintrin.h>
#else
// TODO add Unix intrinsics
#endif
#endif

static void PrimaryPlaneMouseButton(glm::vec<2, i32> position, float scroll, MouseOperation operation, void* data)
{
	Plane::PrimaryPlane->MouseButton(position, scroll, operation);
}

static void ToolbarPlaneMouseButton(glm::vec<2, i32> position, float scroll, MouseOperation operation, void* data)
{
	Plane::ToolbarPlane->MouseButton(position, scroll, operation);
}

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
	, m_Background(std::make_shared<rhr::render::object::rectangle>())
{
	if (toolbar)
		InputHandler::RegisterMouseCallback(ToolbarPlaneMouseButton, nullptr);
	else
		InputHandler::RegisterMouseCallback(PrimaryPlaneMouseButton, nullptr);

	m_Background->SetWeak(m_Background);
	m_Background->SetPosition(m_Position + m_SuperOffset);
	m_Background->SetSize(m_Size);
	m_Background->SetColor(Color().FromU8({ 0, 0, 0, 255 }));
	m_Background->SetDepth(Renderer::depth_plane);

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
	//if (m_Toolbar)
	//	InputHandler::UnregisterMouseCallback(ToolbarPlaneMouseButton);
	//else
	//	InputHandler::UnregisterMouseCallback(PrimaryPlaneMouseButton);

	//for (uint64_t i = 0; i < m_Collections.size(); i++)
	//	delete m_Collections[i];
}

void Plane::AddCollection(std::shared_ptr<Collection>& collection, bool displayCollectionVanity)
{
	collection->SetSuperOffset(m_Position + m_SuperOffset);
	m_Collections.push_back(collection);
	m_CollectionVanity.push_back(displayCollectionVanity);

	collection->DisplayVanity(displayCollectionVanity);

	//CreateBuffer(m_Collections.size() - 1, displayCollectionVanity);
}

const std::vector<std::shared_ptr<Collection>>& Plane::GetCollections()
{
	return m_Collections;
}

void Plane::DeleteCollection(uint64_t idx)
{
	//m_VertexArrays.erase(m_vertexArrays.begin() + idx);
	//m_VertexBuffers.erase(m_vertexBuffers.begin() + idx);
	//m_VertexBufferTransform.erase(m_vertexBufferTransform.begin() + idx);
	//m_TextureMapImage.erase(m_textureMapImage.begin() + idx);
	//m_TextureMapTexture.erase(m_textureMapTexture.begin() + idx);
	//m_TextureMapEnabled.erase(m_textureMapEnabled.begin() + idx);
	m_CollectionVanity.erase(m_CollectionVanity.begin() + idx);

	//if (dealloc)
	//	delete m_Collections[idx];

	m_Collections.erase(m_Collections.begin() + idx);
}

bool Plane::IsToolbar()
{
	return m_Toolbar;
}

//void Plane::TranslateInnerPosition(const glm::vec<2, i32>& position)
//{
//	//m_innerPosition += position;
//	m_view.setCenter(m_view.getCenter() + (glm::vec<2, i32>)position);
//}
//
//void Plane::SetInnerPosition(glm::vec<2, i32> position)
//{
//	//m_innerPosition = position;
//	m_view.setCenter((glm::vec<2, i32>)position);
//}
//
//glm::vec<2, i32> Plane::GetInnerPosition()
//{
//	return glm::vec<2, i32>();
//}

void Plane::DeleteContents()
{
	m_Collections.clear();
}

void Plane::MouseButton(glm::vec<2, i32> position, float scroll, MouseOperation operation)
{
	//if (m_window == nullptr)
	//	return false;

	//glm::vec<2, i32> mmpos = position;

	//uint64_t collectionMax = m_Collections.size();

	//if (collectionMax == 0)
	//	return;

	if (operation != MouseOperation::Press && operation != MouseOperation::Release)
		return;

	if ((m_Toolbar && Plane::PrimaryPlane->DraggingStack()) ||
		(!m_Toolbar && Plane::ToolbarPlane->DraggingStack()))
		return;

	//if (position.x < m_Position.x + m_SuperOffset.x ||
	//	position.x > m_Position.x + m_SuperOffset.x + m_Size.x ||
	//	position.y < m_Position.y + m_SuperOffset.y ||
	//	position.y > m_Position.y + m_SuperOffset.y + m_Size.y)
	//	return;

	//if (DraggingStack() || DraggingCollection())
	//	collectionMax--;


	if (DraggingStack())
	{
		if (operation == MouseOperation::Release && !m_DraggingUp)
		{
			// dragging and mouse released (used when dragging a stack)
			UnDrag(position);
		}
		else if (operation == MouseOperation::Press && m_DraggingUp)
		{
			// dragging and mouse pressed (used when duplicating stack)
			UnDrag(position);
		}
	}

	for (int64_t i = /*collectionMax - 1*/0; i < m_Collections.size(); i++)
	{
		if (!m_Collections[i]->GetEnabled())
			continue;

		glm::vec<2, i32> collectionSize = m_Collections[i]->GetSize();
		glm::vec<2, i32> collectionPosition = m_Collections[i]->GetPosition() + m_Collections[i]->GetSuperOffset();

		if (position.x > collectionPosition.x && position.x < collectionPosition.x + collectionSize.x &&
			position.y > collectionPosition.y && position.y < collectionPosition.y + collectionSize.y)
		{
			for (int64_t a = m_Collections[i]->GetStacks().size() - 1; a >= 0; a--)
			{
				glm::vec<2, i32> stackSize = m_Collections[i]->GetStacks()[a]->GetSize();
				glm::vec<2, i32> stackPosition = m_Collections[i]->GetStacks()[a]->GetPosition() + m_Collections[i]->GetStacks()[a]->GetSuperOffset();

				//stackPosition += collectionPosition;

				if (position.x > stackPosition.x && position.x < stackPosition.x + stackSize.x &&
					position.y > stackPosition.y && position.y < stackPosition.y + stackSize.y)
				{
					for (uint64_t b = 0; b < m_Collections[i]->GetStacks()[a]->GetBlocks().size(); b++)
					{
						glm::vec<2, i32> blockSize = m_Collections[i]->GetStacks()[a]->GetBlocks()[b]->GetSize();
						glm::vec<2, i32> blockPosition = m_Collections[i]->GetStacks()[a]->GetBlocks()[b]->GetPosition() + m_Collections[i]->GetStacks()[a]->GetBlocks()[b]->GetSuperOffset();

						if (position.x >= blockPosition.x && position.x < blockPosition.x + blockSize.x &&
							position.y >= blockPosition.y && position.y < blockPosition.y + blockSize.y)
						{
							if (operation == MouseOperation::Press && !DraggingStack())
							{
								// not dragging and mouse down

								if (true/* left or right click */)
								{
									// remove context menu

									//ContextHandler::Disable();
									UnSelect();

									std::shared_ptr<Collection> activeCollection = std::make_shared<Collection>();
									activeCollection->SetWeak(activeCollection);

									std::shared_ptr<Stack> activeStack = m_Collections[i]->GetStacks()[a];

									if (m_Toolbar)
									{
										std::shared_ptr<Stack> dragging_stack = std::make_shared<Stack>();
										dragging_stack->SetWeak(dragging_stack);

										const std::vector<std::shared_ptr<Block>>& blocks = activeStack->GetBlocks();
										std::vector<std::shared_ptr<Block>> cloned_blocks;

										for (auto block : blocks)
										{
											std::shared_ptr<Block> cloned_block = std::make_shared<Block>(block->GetModBlock()->GetUnlocalizedName());
											cloned_block->SetWeak(cloned_block);

											cloned_blocks.push_back(std::move(cloned_block));
										}

										dragging_stack->AddBlocks(cloned_blocks);
										activeStack = dragging_stack;
									}

									if (b > 0)
									{
										if (m_Toolbar)
										{
											Logger::Error("splitting stacks inside toolbar is not supported");
											return;
										}

										// create stack that is left behind (no the one picked up)

										std::shared_ptr<Stack> leftStack = std::make_shared<Stack>();
										leftStack->SetWeak(leftStack);
										leftStack->SetPosition(activeStack->GetPosition());

										for (uint64_t c = 0; c < b; c++)
											leftStack->AddBlock(m_Collections[i]->GetStacks()[a]->GetBlocks()[c]);

										for (uint64_t c = 0; c < b; c++)
											activeStack->RemoveBlock(0);

										m_Collections[i]->AddStack(leftStack, false);
										stackPosition += glm::vec<2, i32>(0, static_cast<i32>(b) * static_cast<i32>(Block::Height));
									}

									activeCollection->SetPosition(stackPosition);
									activeCollection->SetSize(stackSize);

									activeStack->SetPosition({ 0, 0 });

									// register stack and collection

									if (!m_Toolbar)
										m_Collections[i]->RemoveStack(a);

									activeCollection->AddStack(activeStack, false);
									activeCollection->SizeToStacks(true, false);
									//AddCollection(activeCollection, false);

									//puts("here");

									// update buffer that was taken from

									//if (m_Collections[i]->GetStacks().size() > 0)
									//{
									//	//UpdateBuffer(i);

									//}
									//else
									//	DeleteCollection(i);

									// drag the current stack (excludes the stack that was left behind)

									DragStack(activeCollection, activeStack, false);

									if (m_Collections[i]->GetStacks().size() == 0)
										DeleteCollection(i);

									return;
								}
								else if (false/* left or right click */)
								{
									//Logger::Debug("opening context menu");

									// startup context menu on block

									//ContextHandler::Disable();
									//ContextHandler::Enable(mmpos + glm::vec<2, i32>(5, 5), &m_contextCallback);
									//SelectContext(i, a, b);
								}
							}

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

//void Plane::UpdateVAOPosition(uint64_t idx)
//{
	/*
	uint64_t i = 0;

	if (m_CollectionVanity[idx])
	{
		//const glm::vec<2, i32> pos = m_Collections[idx]->GetPosition() + GetPosition();
		const glm::vec<2, i32> size = (glm::vec<2, i32>)m_Collections[idx]->GetSize();

		m_vertexArrays[idx][i++].position = glm::vec<2, i32>();
		m_vertexArrays[idx][i++].position = glm::vec<2, i32>(size.x, COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = glm::vec<2, i32>(0, COLLECTION_OUTLINE_WIDTH);

		m_vertexArrays[idx][i++].position = glm::vec<2, i32>(size.x + COLLECTION_OUTLINE_WIDTH, 0);
		m_vertexArrays[idx][i++].position = glm::vec<2, i32>(size.x + COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = glm::vec<2, i32>();

		m_vertexArrays[idx][i++].position = glm::vec<2, i32>(size.x + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH, 0);
		m_vertexArrays[idx][i++].position = glm::vec<2, i32>(size.x + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = glm::vec<2, i32>(size.x + COLLECTION_OUTLINE_WIDTH, 0);

		m_vertexArrays[idx][i++].position = glm::vec<2, i32>(size.x + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = glm::vec<2, i32>(size.x + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = glm::vec<2, i32>(size.x + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH, 0);

		m_vertexArrays[idx][i++].position = glm::vec<2, i32>(size.x + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = glm::vec<2, i32>(COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = glm::vec<2, i32>(size.x + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH);

		m_vertexArrays[idx][i++].position = glm::vec<2, i32>(COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = glm::vec<2, i32>(COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = glm::vec<2, i32>(size.x + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH);

		m_vertexArrays[idx][i++].position = glm::vec<2, i32>(0, size.y + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = glm::vec<2, i32>(COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = glm::vec<2, i32>(COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH);

		m_vertexArrays[idx][i++].position = glm::vec<2, i32>(0, COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = glm::vec<2, i32>(COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = glm::vec<2, i32>(0, size.y + COLLECTION_OUTLINE_WIDTH + COLLECTION_OUTLINE_WIDTH);

		m_vertexArrays[idx][i++].position = glm::vec<2, i32>(COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = glm::vec<2, i32>(size.x + COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = glm::vec<2, i32>(size.x + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH);

		m_vertexArrays[idx][i++].position = glm::vec<2, i32>(COLLECTION_OUTLINE_WIDTH, COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = glm::vec<2, i32>(size.x + COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH);
		m_vertexArrays[idx][i++].position = glm::vec<2, i32>(COLLECTION_OUTLINE_WIDTH, size.y + COLLECTION_OUTLINE_WIDTH);
	}
	*/


	// TODO not finished; may not need
//}

//void Plane::UpdateVAO(uint64_t idx)
//{
//	UpdateBuffer(idx);
//}

void Plane::FrameUpdate(double deltaTime)
{
	// set innerText to a visual representation of the inner position coordinates

	if (!m_Toolbar)
	{
		//m_InnerText.SetPosition({ GetPosition().x + 5, GetPosition().y + GetSize().y - 18 });
		char innerText[20];
		memset(innerText, 0, 20);
		sprintf(innerText, "test");
		// TODO display m_view.center thingy instead of innerPosition
		//sprintf(innerText, "%d, %d", GetInnerPosition().x, GetInnerPosition().y); // TODO make sure "%d" has no trailing "0"s.
		//m_InnerText.SetText(innerText);
	}

	// dragging position

	if (DraggingStack() || DraggingCollection())
	{
		m_DraggingCollection->SetPosition(m_DraggingBeginObject + glm::vec<2, i32>(
			(InputHandler::GetMousePosition().x - m_DraggingBeginMouse.x)/* / CalculateZoom().x*/,
			(InputHandler::GetMousePosition().y - m_DraggingBeginMouse.y)/* / CalculateZoom().y*/) - GetPosition());
	}
	
	// dragging connections

	if (DraggingStack())
		DraggingStackUpdate();

	// transform applies offset to the vertex buffers

	//for (uint64_t i = 0; i < m_vertexBufferTransform.size(); i++)
	//	m_vertexBufferTransform[i] = sf::Transform().translate((glm::vec<2, i32>)GetPosition()).translate(m_Collections[i]->GetPosition());

	//if (DraggingStack() || DraggingCollection())
	//{
	//	glm::vec<2, i32> zoom = Plane::PrimaryPlane->CalculateZoom();
	//	m_vertexBufferTransform.back().scale(zoom);
	//}
}

void Plane::OnRender()
{
	//m_Background->Render();

	for (auto& collection : m_Collections)
		collection->Render();

	if (DraggingStack() || DraggingCollection())
		m_DraggingCollection->Render();
}

void Plane::OnUpdateBuffers()
{
	m_Background->UpdateBuffers();

	for (auto& collection : m_Collections)
		collection->UpdateBuffers();

	if (DraggingStack() || DraggingCollection())
		m_DraggingCollection->UpdateBuffers();
}

void Plane::OnReloadSwapChain()
{
	m_Background->ReloadSwapChain();

	for (auto& collection : m_Collections)
		collection->ReloadSwapChain();

	if (DraggingStack() || DraggingCollection())
		m_DraggingCollection->ReloadSwapChain();
}

void Plane::PostPositionUpdate()
{
	SetSizeMax();
	m_Background->SetPosition(m_Position + m_SuperOffset);

	for (auto& collection : m_Collections)
		collection->SetSuperOffset(m_Position + m_SuperOffset);
}

void Plane::PostSizeUpdate()
{
	m_Background->SetSize(m_Size);

	//for (auto collection : m_Collections)
	//	collection->SetSuperBounds(m_Size);
}

std::shared_ptr<Plane> Plane::PrimaryPlane;
std::shared_ptr<Plane> Plane::ToolbarPlane;

//void Plane::render(sf::RenderWindow& window)
//{
	//m_background.setFillColor(MOD_BACKGROUND_LOW);
	//m_background.SetPosition(GetPosition());
	//m_background.SetSize((glm::vec<2, i32>)GetSize());

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
	//	//states.transform.scale(glm::vec<2, i32>(Global::BlockZoom, Global::BlockZoom), GetPosition());//, /*glm::vec<2, i32>(GetSize().x / 2.0f, GetSize().y / 2.0f) - */-(m_Collections[i]->GetPosition() * Global::BlockZoom));
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
	//		glm::vec<2, i32>(0, m_draggingSnapStackLoc * Block::Height)// -
	//		//(glm::vec<2, i32>)Plane::PrimaryPlane->GetInnerPosition()
	//	);

	//	uint64_t refIdx = 0;

	//	if (m_draggingSnapStackLoc > 0)
	//		refIdx = m_draggingSnapStackLoc - 1;

	//	m_draggingShape.SetSize(glm::vec<2, i32>(m_draggingSnapStack->GetBlocks()[refIdx]->GetWidth(), SNAP_GRAPHIC_HEIGHT));

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
	//	//states.transform.translate((glm::vec<2, i32>)GetInnerPosition());

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

//void Plane::UpdateCollectionVAO(std::vector<sf::Vertex>* vao, glm::vec<2, i32> size)
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
//			glm::vec<2, i32> blockPos = m_Collections[bufferIdx]->GetStacks()[a]->GetPosition() + glm::vec<2, i32>(0, b * Block::Height);
//			uint32_t blockShellWidth = m_Collections[bufferIdx]->GetStacks()[a]->GetBlocks()[b]->GetWidth();
//			const sf::Color blockColor = m_Collections[bufferIdx]->GetStacks()[a]->GetBlocks()[b]->GetModCategory()->GetColor();
//
//			vaBox.back().push_back(sf::Vertex(blockPos + glm::vec<2, i32>(0, 0), blockColor, glm::vec<2, i32>(0, 0)));
//			vaBox.back().push_back(sf::Vertex(blockPos + glm::vec<2, i32>(blockShellWidth, 0), blockColor, glm::vec<2, i32>(0, 0)));
//			vaBox.back().push_back(sf::Vertex(blockPos + glm::vec<2, i32>(blockShellWidth, Block::Height), blockColor, glm::vec<2, i32>(0, 0)));
//
//			vaBox.back().push_back(sf::Vertex(blockPos + glm::vec<2, i32>(0, 0), blockColor, glm::vec<2, i32>(0, 0)));
//			vaBox.back().push_back(sf::Vertex(blockPos + glm::vec<2, i32>(blockShellWidth, Block::Height), blockColor, glm::vec<2, i32>(0, 0)));
//			vaBox.back().push_back(sf::Vertex(blockPos + glm::vec<2, i32>(0, Block::Height), blockColor, glm::vec<2, i32>(0, 0)));
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
//						va.back()[d + 0].texCoords = glm::vec<2, i32>(gl.textureRect.left, gl.textureRect.top);
//						va.back()[d + 1].texCoords = glm::vec<2, i32>(gl.textureRect.left + gl.textureRect.width, gl.textureRect.top);
//						va.back()[d + 2].texCoords = glm::vec<2, i32>(gl.textureRect.left + gl.textureRect.width, gl.textureRect.top + gl.textureRect.height);
//						va.back()[d + 3].texCoords = glm::vec<2, i32>(gl.textureRect.left, gl.textureRect.top);
//						va.back()[d + 4].texCoords = glm::vec<2, i32>(gl.textureRect.left + gl.textureRect.width, gl.textureRect.top + gl.textureRect.height);
//						va.back()[d + 5].texCoords = glm::vec<2, i32>(gl.textureRect.left, gl.textureRect.top + gl.textureRect.height);
//
//						va.back()[d].position += glm::vec<2, i32>(
//							static_cast<float>(blockWidth) + blockPos.x,
//							blockPos.y + ((float)Block::Padding / 2.0f)
//						);
//					}
//					else
//					{
//						// make VAO absolute
//						va.back()[d].position += glm::vec<2, i32>(
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
//			m_vertexArrays[bufferIdx][a].texCoords = glm::vec<2, i32>(
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
//			va[a][b].texCoords = glm::vec<2, i32>(
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
//			vaBox[a][b].texCoords = glm::vec<2, i32>(
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
//		vao->push_back(sf::Vertex(glm::vec<2, i32>(positions[idx0 * 2], positions[idx0 * 2 + 1]),
//			sf::Color(colors[idx0 * 3], colors[idx0 * 3 + 1], colors[idx0 * 3 + 2]),
//			glm::vec<2, i32>(textureCoords[idx0 * 2], textureCoords[idx0 * 2 + 1])));
//
//		uint8_t idx1 = indices[i * 3 + 1];
//
//		vao->push_back(sf::Vertex(glm::vec<2, i32>(positions[idx1 * 2], positions[idx1 * 2 + 1]),
//			sf::Color(colors[idx1 * 3], colors[idx1 * 3 + 1], colors[idx1 * 3 + 2]),
//			glm::vec<2, i32>(textureCoords[idx1 * 2], textureCoords[idx1 * 2 + 1])));
//
//		uint8_t idx2 = indices[i * 3 + 2];
//
//		vao->push_back(sf::Vertex(glm::vec<2, i32>(positions[idx2 * 2], positions[idx2 * 2 + 1]),
//			sf::Color(colors[idx2 * 3], colors[idx2 * 3 + 1], colors[idx2 * 3 + 2]),
//			glm::vec<2, i32>(textureCoords[idx2 * 2], textureCoords[idx2 * 2 + 1])));
//	}
//}

void Plane::select(uint64_t collection, uint64_t stack, uint64_t block, uint64_t argument)
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

void Plane::DragCollection(std::shared_ptr<Collection>& collection, bool up)
{
	if (m_DraggingStack != nullptr || m_DraggingCollection != nullptr)
		return;

	m_DraggingUp = up;

	m_DraggingCollection = collection;
	m_DraggingStack = nullptr;

	m_DraggingBeginMouse = InputHandler::GetMousePosition();
	m_DraggingBeginObject = m_DraggingCollection->GetPosition();
}

void Plane::DragStack(std::shared_ptr<Collection>& collection, std::shared_ptr<Stack>& stack, bool up)
{
	if (m_DraggingStack != nullptr || m_DraggingCollection != nullptr)
		return;

	m_DraggingUp = up;

	m_DraggingBeginMouse = InputHandler::GetMousePosition();
	m_DraggingBeginObject = collection->GetPosition();

	m_DraggingCollection = collection;
	m_DraggingStack = stack;
}

void Plane::UnDrag(const glm::vec<2, i32>& position)
{
	if (DraggingStack())
	{
		glm::vec<2, i32> pixelPosition = m_DraggingCollection->GetPosition();

		if (!(pixelPosition.x >= Plane::PrimaryPlane->GetPosition().x && pixelPosition.x < Plane::PrimaryPlane->GetSize().x + Plane::PrimaryPlane->GetPosition().x &&
			pixelPosition.y >= Plane::PrimaryPlane->GetPosition().y && pixelPosition.y < Plane::PrimaryPlane->GetSize().y + Plane::PrimaryPlane->GetPosition().y))
			return;

		if (IsSnap())
		{
			m_DraggingSnapStack->InsertBlocks(m_DraggingStack->GetBlocks(), m_DraggingSnapStackLoc);
			m_DraggingSnapCollection->CheckBounds();
			//m_DraggingCollection->RemoveAll();

			//DeleteCollection(m_Collections.size() - 1);
			//Plane::PrimaryPlane->UpdateBuffer(m_DraggingSnapCollection);
		}
		else
		{
			glm::vec<2, i32> planePrimaryPosition = Plane::PrimaryPlane->GetPosition() + Plane::PrimaryPlane->GetSuperOffset();
			glm::vec<2, i32> planePrimarySize = Plane::PrimaryPlane->GetSize();

			glm::vec<2, i32> planeToolbarPosition = Plane::ToolbarPlane->GetPosition() + Plane::ToolbarPlane->GetSuperOffset();
			glm::vec<2, i32> planeToolbarSize = Plane::ToolbarPlane->GetSize();

			if (pixelPosition.x > planeToolbarPosition.x && pixelPosition.x < planeToolbarPosition.x + planeToolbarSize.x &&
				pixelPosition.y > planeToolbarPosition.y && pixelPosition.y < planeToolbarPosition.y + planeToolbarSize.y)
			{
				// TODO: undo support (or recently deleted)
				Logger::Warn("letting stack deallocate; undo not supported yet");
			}
			else if (pixelPosition.x > planePrimaryPosition.x && pixelPosition.x < planePrimaryPosition.x + planePrimarySize.x &&
					 pixelPosition.y > planePrimaryPosition.y && pixelPosition.y < planePrimaryPosition.y + planePrimarySize.y)
			{
				Logger::Debug("dropping over primary plane");
				bool found = false;

				for (uint64_t i = 0; i < Plane::PrimaryPlane->GetCollections().size(); i++)
				{
					//if (Plane::PrimaryPlane->GetCollections()[i] == m_DraggingCollection)
					//	continue;

					//glm::vec<2, i32> colPos = (glm::vec<2, i32>)m_window->mapCoordsToPixel(
					//	Plane::PrimaryPlane->GetCollections()[i]->GetPosition(),
					//	*Plane::PrimaryPlane->GetView()) + Plane::PrimaryPlane->GetPosition();

					//glm::vec<2, i32> colPos = Plane::PrimaryPlane->GetCollections()[i]->GetPosition() + Plane::PrimaryPlane->GetPosition();

					//glm::vec<2, i32> colSize = glm::vec<2, i32>(
					//	Plane::PrimaryPlane->GetCollections()[i]->GetSize().x,
					//	Plane::PrimaryPlane->GetCollections()[i]->GetSize().y
					//);

					glm::vec<2, i32> collectionPosition = Plane::PrimaryPlane->GetCollections()[i]->GetPosition() + Plane::PrimaryPlane->GetCollections()[i]->GetSuperOffset();
					glm::vec<2, i32> collectionSize = Plane::PrimaryPlane->GetCollections()[i]->GetSize();

					//colSize = colSize * CalculateZoom();

					if (m_DraggingCollection->GetPosition().x > collectionPosition.x && m_DraggingCollection->GetPosition().x < collectionPosition.x + collectionSize.x &&
						m_DraggingCollection->GetPosition().y > collectionPosition.y && m_DraggingCollection->GetPosition().y < collectionPosition.y + collectionSize.y)
					{
						//puts("dropping inside collection");
						//m_DraggingStack->SetPosition((m_DraggingCollection->GetPosition() + GetPosition()) - colPos);
						m_DraggingStack->SetPosition((m_DraggingCollection->GetPosition() + m_DraggingCollection->GetSuperOffset()) - collectionPosition);
						Plane::PrimaryPlane->GetCollections()[i]->AddStack(m_DraggingStack);

						//Plane::PrimaryPlane->UpdateBuffer(i);

						//m_DraggingCollection->RemoveAll();
						//DeleteCollection(m_Collections.size() - 1);

						found = true;
						break;
					}
				}

				if (!found)
				{
					glm::vec<2, i32> collectionPosition = m_DraggingCollection->GetPosition() + m_DraggingCollection->GetSuperOffset();
					Plane::PrimaryPlane->AddCollection(m_DraggingCollection, true);
					m_DraggingCollection->SetPosition(collectionPosition - planePrimaryPosition);
					m_DraggingCollection->SizeToStacks(false, true);

					//m_DraggingCollection->SetPosition((glm::vec<2, i32>)(((glm::vec<2, i32>)position - (glm::vec<2, i32>)Plane::PrimaryPlane->GetPosition()))
					//		  - (glm::vec<2, i32>)(((glm::vec<2, i32>)m_DraggingBeginMouse - m_DraggingBeginObject))
					//		  + (glm::vec<2, i32>)(glm::vec<2, i32>(1.0f, 1.0f) - /*Plane::PrimaryPlane->CalculateZoom() **/ Plane::PrimaryPlane->GetPosition())
					//	 - glm::vec<2, i32>(COLLECTION_EMPTY_SPACE, COLLECTION_EMPTY_SPACE)
					//);

					////std::cout << Plane::PrimaryPlane->CalculateZoom().x << " || " << Plane::PrimaryPlane->CalculateZoom().y << std::endl;

					//m_DraggingCollection->SetSize((glm::vec<2, i32>)m_DraggingCollection->GetSize() + glm::vec<2, i32>(COLLECTION_EMPTY_SPACE * 2, COLLECTION_EMPTY_SPACE * 2));
					//m_DraggingStack->SetPosition({ COLLECTION_EMPTY_SPACE, COLLECTION_EMPTY_SPACE });

					//DeleteCollection(m_Collections.size() - 1);
					//Plane::PrimaryPlane->AddCollection(m_DraggingCollection, true);
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


	glm::vec<2, i32> pixelPosition = /*m_DraggingCollection->GetPosition() + GetPosition()*/ /*InputHandler::GetMousePosition()*/ m_DraggingCollection->GetPosition() + m_DraggingCollection->GetSuperOffset();
	m_DraggingCollection->SetPosition(InputHandler::GetMousePosition() - m_DraggingBeginMouse + m_DraggingBeginObject);

	if (!(pixelPosition.x >= Plane::PrimaryPlane->GetPosition().x && pixelPosition.x < Plane::PrimaryPlane->GetSize().x + Plane::PrimaryPlane->GetPosition().x &&
		pixelPosition.y >= Plane::PrimaryPlane->GetPosition().y && pixelPosition.y < Plane::PrimaryPlane->GetSize().y + Plane::PrimaryPlane->GetPosition().y))
		return;

	std::shared_ptr<Plane> usePlane = Plane::PrimaryPlane;
	const std::vector<std::shared_ptr<Collection>>& useCollections = usePlane->GetCollections();

	uint64_t collectionMax = useCollections.size();

	if (collectionMax == 0)
		return;

	//if (usePlane->IsToolbar() == m_Toolbar && (DraggingStack() || DraggingCollection()))
	//	collectionMax--;

	for (uint64_t i = 0; i < collectionMax; i++)
	{
		// size

		// TODO: consolidate these
		/*glm::vec<2, i32> collectionSize = glm::vec<2, i32>(
			((SNAP_DISTANCE * 2.0f * CalculateZoom().x) + useCollections[i]->GetSize().x),
			((SNAP_DISTANCE * 2.0f * CalculateZoom().y) + useCollections[i]->GetSize().y)
		);*/

		glm::vec<2, i32> collectionSize = useCollections[i]->GetSize();
		glm::vec<2, i32> collectionPosition = useCollections[i]->GetPosition() + useCollections[i]->GetSuperOffset();

		//collectionPosition += usePlane->GetPosition();
		collectionPosition -= glm::vec<2, i32>(SNAP_DISTANCE, SNAP_DISTANCE);
		collectionSize += glm::vec<2, i32>(SNAP_DISTANCE, SNAP_DISTANCE) * 2;

		if (pixelPosition.x > collectionPosition.x && pixelPosition.x < collectionPosition.x + collectionSize.x &&
			pixelPosition.y > collectionPosition.y && pixelPosition.y < collectionPosition.y + collectionSize.y)
		{
			//std::cout << "collection " << i << std::endl;

			for (int64_t a = useCollections[i]->GetStacks().size() - 1; a >= 0; a--)
			{
				glm::vec<2, i32> stackSize = useCollections[i]->GetStacks()[a]->GetSize();
				glm::vec<2, i32> stackPosition = useCollections[i]->GetStacks()[a]->GetPosition() + useCollections[i]->GetStacks()[a]->GetSuperOffset();


				stackPosition -= glm::vec<2, i32>(SNAP_DISTANCE, SNAP_DISTANCE);
				stackSize += glm::vec<2, i32>(SNAP_DISTANCE, SNAP_DISTANCE) * 2;
				//for (uint64_t b = 0; b < useCollections[i]->GetStacks()[a]->GetBlocks().size(); b++)
				//{
				//	if (useCollections[i]->GetStacks()[a]->GetBlocks()[b]->GetWidth() > stackSize.x)
				//		stackSize.x = useCollections[i]->GetStacks()[a]->GetBlocks()[b]->GetWidth();// *CalculateZoom().x;
				//}

				//stackSize.y = useCollections[i]->GetStacks()[a]->GetBlocks().size() * Block::Height;// *CalculateZoom().y;
				//stackSize += glm::vec<2, i32>(SNAP_DISTANCE * 2.0f/* * CalculateZoom().x*/, SNAP_DISTANCE * 2.0f/* * CalculateZoom().y*/);

				// position

				//stackPosition += usePlane->GetPosition();
				//stackPosition -= glm::vec<2, i32>(SNAP_DISTANCE/* * CalculateZoom().x*/, SNAP_DISTANCE/* * CalculateZoom().y*/);

				//stackPosition += collectionPosition;

				if (pixelPosition.x > stackPosition.x && pixelPosition.x < stackPosition.x + stackSize.x &&
					pixelPosition.y > stackPosition.y && pixelPosition.y < stackPosition.y + stackSize.y)
				{
					//std::cout << "stack " << a << std::endl;

					for (uint64_t b = 0; b < useCollections[i]->GetStacks()[a]->GetBlocks().size() + 1; b++)
					{
						uint64_t refIdx = 0;

						if (b > 0)
							refIdx = b - 1;

						glm::vec<2, i32> boundingSize = { useCollections[i]->GetStacks()[a]->GetBlocks()[refIdx]->GetWidth() + (SNAP_DISTANCE * 2), Block::Height };
						glm::vec<2, i32> boundingPos = useCollections[i]->GetStacks()[a]->GetPosition() + useCollections[i]->GetStacks()[a]->GetSuperOffset();

						boundingPos.x -= SNAP_DISTANCE;

						boundingPos.y += static_cast<i32>(Block::Height) * b;
						boundingPos.y -= static_cast<float>(Block::Height) / 2.0f;

						glm::vec<2, i32> draggingCollectionPosition = m_DraggingCollection->GetPosition() + m_DraggingCollection->GetSuperOffset();

						if (draggingCollectionPosition.x >= boundingPos.x && draggingCollectionPosition.x < boundingPos.x + boundingSize.x &&
							draggingCollectionPosition.y >= boundingPos.y && draggingCollectionPosition.y < boundingPos.y + boundingSize.y)
						{
							std::cout << "snapping " << b << std::endl;
							SetSnap(useCollections[i], b, useCollections[i]->GetStacks()[a]);

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

void Plane::SetSnap(std::shared_ptr<Collection> collection, uint64_t stackLoc, std::shared_ptr<Stack> stack)
{
	m_DraggingSnap = true;

	m_DraggingSnapCollection = collection;
	m_DraggingSnapStackLoc = stackLoc;
	m_DraggingSnapStack = stack;
}

void Plane::ClearSnap()
{
	m_DraggingSnap = false;

	m_DraggingSnapCollection = nullptr;
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

//glm::vec<2, i32> Plane::CalculateZoom()
//{
//	return glm::vec<2, i32>(
//		static_cast<float>(Global::WindowSize.x) / m_view.GetSize().x,
//		static_cast<float>(Global::WindowSize.y) / m_view.GetSize().y
//	);
//}

//glm::vec<2, i32> Plane::GetCoordsFromPixel(glm::vec<2, i32> pixel, sf::RenderWindow& window)
//{
//	return window.mapPixelToCoords((glm::vec<2, i32>)pixel, m_view);
//}

//void Plane::draw(sf::RenderTarget& target, sf::RenderStates states) const
//{
//	throw std::logic_error("use Plane::render instead");
//}
