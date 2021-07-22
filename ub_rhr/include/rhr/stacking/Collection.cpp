#include "Collection.hpp"

#include "ui/Renderer.hpp"

Collection::Collection()
	: IEnableable(true)
	, m_Background(std::make_shared<vui::RenderRectangle>())
	, m_DisplayVanity(true)
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

void Collection::AddStack(std::shared_ptr<Stack> stack, bool autoSize)
{
	stack->SetSuperOffset(m_Position + m_SuperOffset);
	m_stacks.push_back(stack);

	if (autoSize)
	{
		int32_t emptySpace = COLLECTION_EMPTY_SPACE;

		if (!m_DisplayVanity)
			emptySpace = 0;

		glm::vec<2, int32_t> size = GetSize();
		glm::vec<2, int32_t> position = GetPosition();

		if (size.x < emptySpace)
			size.x = emptySpace;

		if (size.y < emptySpace)
			size.y = emptySpace;

		if (stack->GetPosition().x + stack->GetWidestBlock() > size.x - emptySpace)
			size.x = stack->GetPosition().x + stack->GetWidestBlock() + emptySpace;

		if (stack->GetPosition().x < emptySpace)
		{
			int32_t diff = emptySpace - stack->GetPosition().x;
			position.x -= diff;
			size.x += diff;
			stack->SetPosition(stack->GetPosition() + glm::vec<2, int32_t>(diff, 0));
		}

		if (stack->GetPosition().y + (stack->GetBlocks().size() * Block::Height) > size.y - emptySpace)
			size.y = stack->GetPosition().y + (stack->GetBlocks().size() * Block::Height) + emptySpace;

		if (stack->GetPosition().y < emptySpace)
		{
			int32_t diff = emptySpace - stack->GetPosition().y;
			position.y -= diff;
			size.y += diff;
			stack->SetPosition(stack->GetPosition() + glm::vec<2, int32_t>(0, diff));
		}

		SetSize(size);
		SetPosition(position);
	}

	//glm::vec<2, int32_t> maxBounds = /*m_Position + m_SuperOffset + */m_Size;
	//glm::vec<2, int32_t> queryPosition = stack->GetPosition()/* + stack->GetSuperOffset()*/;
	//queryPosition.x += stack->GetWidestBlock();
	//queryPosition.y += stack->GetBlocks().size() * Block::Height;

	//if (queryPosition.x > maxBounds.x || queryPosition.y > maxBounds.y)
	//	SetSize(queryPosition - m_Position - m_SuperOffset + 100);
}

void Collection::AddStacks(const std::vector<std::shared_ptr<Stack>>& stacks, bool autoSize)
{
	if (m_stacks.size() + stacks.size() >= m_stacks.capacity())
		m_stacks.reserve((uint64_t)std::ceil((float)(m_stacks.size() + stacks.size()) * 1.5f + 10.0f));

	int32_t emptySpace = COLLECTION_EMPTY_SPACE;

	if (!m_DisplayVanity)
		emptySpace = 0;

	for (uint64_t i = 0; i < stacks.size(); i++)
	{
		stacks[i]->SetSuperOffset(m_Position + m_SuperOffset);
		m_stacks.push_back(stacks[i]);

		if (autoSize)
		{
			glm::vec<2, int32_t> size = GetSize();
			glm::vec<2, int32_t> position = GetPosition();

			if (stacks[i]->GetPosition().x + stacks[i]->GetWidestBlock() > size.x - emptySpace)
				size.x = stacks[i]->GetPosition().x + stacks[i]->GetWidestBlock() + emptySpace;

			if (stacks[i]->GetPosition().x < emptySpace)
			{
				int32_t diff = emptySpace - stacks[i]->GetPosition().x;
				position.x -= diff;
				size.x += diff;
			}

			if (stacks[i]->GetPosition().y + (stacks[i]->GetBlocks().size() * Block::Height) > size.y - emptySpace)
				size.y = stacks[i]->GetPosition().y + (stacks[i]->GetBlocks().size() * Block::Height) + emptySpace;

			if (stacks[i]->GetPosition().y < emptySpace)
			{
				int32_t diff = emptySpace - stacks[i]->GetPosition().y;
				position.y -= diff;
				size.y += diff;
			}

			SetSize(size);
			SetPosition(position);
		}

		//glm::vec<2, int32_t> maxBounds = m_Position + m_SuperOffset + m_Size;
		//glm::vec<2, int32_t> queryPosition = stacks[i]->GetPosition();
		//queryPosition.x += stacks[i]->GetWidestBlock();
		//queryPosition.y += stacks[i]->GetBlocks().size() * Block::Height;

		//if (queryPosition.x > maxBounds.x || queryPosition.y > maxBounds.y)
		//	SetSize(queryPosition - m_Position - m_SuperOffset + 100);
	}
}

void Collection::SizeToStacks(bool offsetStacks, bool usePadding)
{
	// Config

	if (m_stacks.size() == 0)
		return;

	int32_t padding = 0;

	if (usePadding && m_DisplayVanity)
		padding = COLLECTION_EMPTY_SPACE;

	glm::vec<2, int32_t> earliestPosition = m_stacks.front()->GetPosition();

	for (auto& stack : m_stacks)
	{
		if (stack->GetPosition().x < earliestPosition.x)
			earliestPosition.x = stack->GetPosition().x;

		if (stack->GetPosition().y < earliestPosition.y)
			earliestPosition.y = stack->GetPosition().y;
	}

	// Position

	if (offsetStacks)
	{
		glm::vec<2, int32_t> shift = glm::vec<2, int32_t>(padding, padding) - earliestPosition;

		for (auto& stack : m_stacks)
		{
			stack->SetPosition(stack->GetPosition() + shift);
		}
	}
	else
	{
		glm::vec<2, int32_t> shift = earliestPosition - glm::vec<2, int32_t>(padding, padding);
		SetPosition(GetPosition() + shift);

		for (auto& stack : m_stacks)
		{
			stack->SetPosition(stack->GetPosition() - shift);
		}
	}

	glm::vec<2, int32_t> latestPosition = m_stacks.front()->GetPosition();

	for (auto& stack : m_stacks)
	{
		glm::vec<2, int32_t> stackLastBounds = stack->GetPosition() + stack->GetSize();

		if (stackLastBounds.x > latestPosition.x)
			latestPosition.x = stackLastBounds.x;

		if (stackLastBounds.y > latestPosition.y)
			latestPosition.y = stackLastBounds.y;
	}

	// Size

	glm::vec<2, int32_t> size = latestPosition + glm::vec<2, int32_t>(padding, padding);
	SetSize(size);
}

void Collection::CheckBounds()
{
	int32_t emptySpace = COLLECTION_EMPTY_SPACE;

	if (!m_DisplayVanity)
		emptySpace = 0;

	for (size_t i = 0; i < m_stacks.size(); i++)
	{
		glm::vec<2, int32_t> size = GetSize();
		glm::vec<2, int32_t> position = GetPosition();

		if (m_stacks[i]->GetPosition().x + m_stacks[i]->GetWidestBlock() > size.x - emptySpace)
			size.x = m_stacks[i]->GetPosition().x + m_stacks[i]->GetWidestBlock() + emptySpace;

		if (m_stacks[i]->GetPosition().x < emptySpace)
		{
			int32_t diff = emptySpace - m_stacks[i]->GetPosition().x;
			position.x -= diff;
			size.x += diff;
		}

		if (m_stacks[i]->GetPosition().y + (m_stacks[i]->GetBlocks().size() * Block::Height) > size.y - emptySpace)
			size.y = m_stacks[i]->GetPosition().y + (m_stacks[i]->GetBlocks().size() * Block::Height) + emptySpace;

		if (m_stacks[i]->GetPosition().y < emptySpace)
		{
			int32_t diff = emptySpace - m_stacks[i]->GetPosition().y;
			position.y -= diff;
			size.y += diff;
		}

		SetSize(size);
		SetPosition(position);
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

void Collection::DisplayVanity(bool enabled)
{
	m_DisplayVanity = enabled;
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

	if (m_DisplayVanity)
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
