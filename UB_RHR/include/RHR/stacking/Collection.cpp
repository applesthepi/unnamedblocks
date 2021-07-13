#include "Collection.hpp"

#include "ui/Renderer.hpp"

Collection::Collection()
	: IEnableable(true)
	, m_Background(std::make_shared<vui::RenderRectangle>())
{
	m_Background->SetWeak(m_Background);
	m_Background->SetDepth(Renderer::DepthCollection);
	m_stacks.reserve(5);
}

//Collection::Collection(const Collection& collection)
//{
//	//for (uint64_t i = 0; i < m_stacks.size(); i++)
//	//	delete m_stacks[i];
//
//	m_stacks.clear();
//
//	for (uint64_t i = 0; i < collection.m_stacks.size(); i++)
//		m_stacks.push_back(new Stack(*collection.m_stacks[i]));
//}

Collection::~Collection()
{
	//for (uint64_t i = 0; i < m_stacks.size(); i++)
	//	delete m_stacks[i];
}

void Collection::AddStack(std::shared_ptr<Stack> stack)
{
	stack->SetSuperOffset(m_Position + m_SuperOffset);
	m_stacks.push_back(stack);

	glm::vec<2, int32_t> size = GetSize();
	glm::vec<2, int32_t> position = GetPosition();

	if (size.x < COLLECTION_EMPTY_SPACE)
		size.x = COLLECTION_EMPTY_SPACE;

	if (size.y < COLLECTION_EMPTY_SPACE)
		size.y = COLLECTION_EMPTY_SPACE;

	if (stack->GetPosition().x + stack->GetWidestBlock() > size.x - COLLECTION_EMPTY_SPACE)
		size.x = stack->GetPosition().x + stack->GetWidestBlock() + COLLECTION_EMPTY_SPACE;

	if (stack->GetPosition().x < COLLECTION_EMPTY_SPACE)
	{
		int32_t diff = COLLECTION_EMPTY_SPACE - stack->GetPosition().x;
		position.x -= diff;
		size.x += diff;
		stack->SetPosition(stack->GetPosition() + glm::vec<2, int32_t>(diff, 0));
	}

	if (stack->GetPosition().y + (stack->GetBlocks().size() * Block::Height) > size.y - COLLECTION_EMPTY_SPACE)
		size.y = stack->GetPosition().y + (stack->GetBlocks().size() * Block::Height) + COLLECTION_EMPTY_SPACE;

	if (stack->GetPosition().y < COLLECTION_EMPTY_SPACE)
	{
		int32_t diff = COLLECTION_EMPTY_SPACE - stack->GetPosition().y;
		position.y -= diff;
		size.y += diff;
		stack->SetPosition(stack->GetPosition() + glm::vec<2, int32_t>(0, diff));
	}

	SetSize(size);
	SetPosition(position);

	//glm::vec<2, int32_t> maxBounds = /*m_Position + m_SuperOffset + */m_Size;
	//glm::vec<2, int32_t> queryPosition = stack->GetPosition()/* + stack->GetSuperOffset()*/;
	//queryPosition.x += stack->GetWidestBlock();
	//queryPosition.y += stack->GetBlocks().size() * Block::Height;

	//if (queryPosition.x > maxBounds.x || queryPosition.y > maxBounds.y)
	//	SetSize(queryPosition - m_Position - m_SuperOffset + 100);
}

void Collection::AddStacks(const std::vector<std::shared_ptr<Stack>>& stacks)
{
	if (m_stacks.size() + stacks.size() >= m_stacks.capacity())
		m_stacks.reserve((uint64_t)std::ceil((float)(m_stacks.size() + stacks.size()) * 1.5f + 10.0f));

	for (uint64_t i = 0; i < stacks.size(); i++)
	{
		stacks[i]->SetSuperOffset(m_Position + m_SuperOffset);
		m_stacks.push_back(stacks[i]);

		glm::vec<2, int32_t> size = GetSize();
		glm::vec<2, int32_t> position = GetPosition();

		if (stacks[i]->GetPosition().x + stacks[i]->GetWidestBlock() > size.x - COLLECTION_EMPTY_SPACE)
			size.x = stacks[i]->GetPosition().x + stacks[i]->GetWidestBlock() + COLLECTION_EMPTY_SPACE;
		
		if (stacks[i]->GetPosition().x < COLLECTION_EMPTY_SPACE)
		{
			int32_t diff = COLLECTION_EMPTY_SPACE - stacks[i]->GetPosition().x;
			position.x -= diff;
			size.x += diff;
		}

		if (stacks[i]->GetPosition().y + (stacks[i]->GetBlocks().size() * Block::Height) > size.y - COLLECTION_EMPTY_SPACE)
			size.y = stacks[i]->GetPosition().y + (stacks[i]->GetBlocks().size() * Block::Height) + COLLECTION_EMPTY_SPACE;
		
		if (stacks[i]->GetPosition().y < COLLECTION_EMPTY_SPACE)
		{
			int32_t diff = COLLECTION_EMPTY_SPACE - stacks[i]->GetPosition().y;
			position.y -= diff;
			size.y += diff;
		}

		SetSize(size);
		SetPosition(position);

		//glm::vec<2, int32_t> maxBounds = m_Position + m_SuperOffset + m_Size;
		//glm::vec<2, int32_t> queryPosition = stacks[i]->GetPosition();
		//queryPosition.x += stacks[i]->GetWidestBlock();
		//queryPosition.y += stacks[i]->GetBlocks().size() * Block::Height;

		//if (queryPosition.x > maxBounds.x || queryPosition.y > maxBounds.y)
		//	SetSize(queryPosition - m_Position - m_SuperOffset + 100);
	}
}

void Collection::RemoveStack(uint64_t idx)
{
	m_stacks.erase(m_stacks.begin() + idx);
}

void Collection::RemoveAll()
{
	m_stacks.clear();
}

const std::vector<std::shared_ptr<Stack>>& Collection::GetStacks()
{
	return m_stacks;
}

void Collection::FrameUpdate(double deltaTime)
{
	for (auto& stack : m_stacks)
		stack->FrameUpdate(deltaTime);
}

void Collection::OnRender()
{
	for (auto& stack : m_stacks)
		stack->Render();

	m_Background->Render();
}

void Collection::OnUpdateBuffers()
{
	for (auto& stack : m_stacks)
		stack->UpdateBuffers();

	m_Background->UpdateBuffers();
}

void Collection::OnReloadSwapChain()
{
	for (auto& stack : m_stacks)
		stack->ReloadSwapChain();

	m_Background->ReloadSwapChain();
}

void Collection::PostPositionUpdate()
{
	for (auto& stack : m_stacks)
		stack->SetSuperOffset(m_Position + m_SuperOffset);

	m_Background->SetSuperOffset(m_Position + m_SuperOffset);
}

void Collection::PostSizeUpdate()
{
	m_Background->SetSuperBounds(m_Size);
	m_Background->SetSizeMax();
}
