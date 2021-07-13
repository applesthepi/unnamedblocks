#include "RenderFrame.hpp"

#include "ui/Renderer.hpp"

// TODO: remove testing
#include <iostream>

vui::RenderFrame::RenderFrame()
	: m_Space(vui::PlaneSpace::HORIZONTAL)
	, m_HasSpace(false)
	, m_HasFrame(false)
	, m_HasContent(false)
	, m_LinkUp(false)
	, m_LinkDown(false)
	, m_LinkLeft(false)
	, m_LinkRight(false)
	, m_MouseButtonRegistered(false)
	, m_Dragging(false)
	, m_DraggingBarIdx(0)
	, m_DraggingMouseBegin({ 0, 0 })
	, m_DraggingObjectBegin({ 0, 0 })
	, m_Padding(10)
	, m_MouseButtonData(new RenderFrameMouseButtonData())
{
	
}

void vui::RenderFrame::SetPadding(uint8_t padding)
{
	m_Padding = padding;
	UpdateContentDimentions();
}

void vui::RenderFrame::SetFrame(std::shared_ptr<RenderFrame>& frame)
{
	if (m_HasContent)
	{
		Logger::Error("can not set frame of RenderFrame that has content");
		return;
	}

	ResetDrag();

	m_HasSpace = false;
	m_HasFrame = true;

	m_Frames.clear();
	m_Frames.push_back(frame);
}

void vui::RenderFrame::AddFrame(std::shared_ptr<RenderFrame>& frame, LocalCardinal cardinal)
{
	if (m_HasContent)
	{
		Logger::Error("can not add frame to RenderFrame that has content");
		return;
	}

	ResetDrag();

	m_HasFrame = true;

	if (!m_HasSpace)
	{
		m_HasSpace = true;

		if (cardinal == LocalCardinal::UP || cardinal == LocalCardinal::DOWN)
			m_Space = PlaneSpace::VERTICAL;
		else if (cardinal == LocalCardinal::LEFT || cardinal == LocalCardinal::RIGHT)
			m_Space = PlaneSpace::HORIZONTAL;
	}

	if (cardinal == LocalCardinal::UP && m_Space == PlaneSpace::VERTICAL)
		m_Frames.insert(m_Frames.begin(), frame);
	else if (cardinal == LocalCardinal::DOWN && m_Space == PlaneSpace::VERTICAL)
		m_Frames.insert(m_Frames.end(), frame);
	else if (cardinal == LocalCardinal::LEFT && m_Space == PlaneSpace::HORIZONTAL)
		m_Frames.insert(m_Frames.begin(), frame);
	else if (cardinal == LocalCardinal::RIGHT && m_Space == PlaneSpace::HORIZONTAL)
		m_Frames.insert(m_Frames.end(), frame);
	else
		Logger::Error("wrong direction when adding a frame to a RenderFrame");

	if (m_Frames.size() > 1)
	{
		SubmitNewBarPosition(cardinal);
		EqualizeBars();
	}

	UpdateLinks();
	UpdateContentDimentions();
}

void vui::RenderFrame::AddContent(std::weak_ptr<IRenderable>&& renderable, std::weak_ptr<IUpdatable>&& updatable, std::weak_ptr<IPositionable>&& positionable, std::weak_ptr<ISizeable>&& sizeable, LocalCardinal cardinal)
{
	if (m_HasFrame)
	{
		Logger::Error("can not add content to RenderFrame that has internal frames");
		return;
	}

	ResetDrag();

	m_HasContent = true;

	if (!m_HasSpace)
	{
		m_HasSpace = true;

		if (cardinal == LocalCardinal::UP || cardinal == LocalCardinal::DOWN)
			m_Space = PlaneSpace::VERTICAL;
		else if (cardinal == LocalCardinal::LEFT || cardinal == LocalCardinal::RIGHT)
			m_Space = PlaneSpace::HORIZONTAL;
	}

	if (auto weak = positionable.lock())
		weak->SetSuperOffset(m_Position + m_SuperOffset);

	if (cardinal == LocalCardinal::UP && m_Space == PlaneSpace::VERTICAL)
		m_Content.insert(m_Content.begin(), { std::move(renderable), std::move(updatable), std::move(positionable), std::move(sizeable) });
	else if (cardinal == LocalCardinal::DOWN && m_Space == PlaneSpace::VERTICAL)
		m_Content.insert(m_Content.end(), { std::move(renderable), std::move(updatable), std::move(positionable), std::move(sizeable) });
	else if (cardinal == LocalCardinal::LEFT && m_Space == PlaneSpace::HORIZONTAL)
		m_Content.insert(m_Content.begin(), { std::move(renderable), std::move(updatable), std::move(positionable), std::move(sizeable) });
	else if (cardinal == LocalCardinal::RIGHT && m_Space == PlaneSpace::HORIZONTAL)
		m_Content.insert(m_Content.end(), { std::move(renderable), std::move(updatable), std::move(positionable), std::move(sizeable) });
	else
		Logger::Error("wrong direction when adding a frame to a RenderFrame");

	if (m_Content.size() > 1)
	{
		SubmitNewBarPosition(cardinal);
		EqualizeBars();
	}

	UpdateContentDimentions();
}

void vui::RenderFrame::MouseButton(glm::vec<2, int32_t> position, float scroll, MouseOperation operation)
{
	if (operation == MouseOperation::Press)
	{
		for (size_t i = 0; i < m_Bars.size(); i++)
		{
			if (position.x >= m_Bars[i].AbsolutePosition.x && position.x < m_Bars[i].AbsolutePosition.x + m_Bars[i].Size.x &&
				position.y >= m_Bars[i].AbsolutePosition.y && position.y < m_Bars[i].AbsolutePosition.y + m_Bars[i].Size.y)
			{
				//Logger::Debug("dragging" + std::to_string(i));

				m_Dragging = true;
				m_DraggingBarIdx = i;
				m_DraggingMouseBegin = position;
				m_DraggingObjectBegin = m_Bars[i].AbsolutePosition;

				break;
			}
		}
	}
	else if (operation == MouseOperation::Release)
	{
		if (m_Dragging)
		{
			//Logger::Debug("releasing" + std::to_string(m_DraggingBarIdx));

			m_Dragging = false;
			m_DraggingBarIdx = -1;
			m_DraggingMouseBegin = { 0, 0 };
			m_DraggingObjectBegin = { 0, 0 };
		}
	}
	else if (operation == MouseOperation::Move)
	{
		if (m_Dragging)
		{
			//Logger::Debug("moving");
			m_Bars[m_DraggingBarIdx].AbsolutePosition = m_DraggingObjectBegin + (position - m_DraggingMouseBegin);
			UpdateBarsFromAbsolute();
			UpdateContentDimentions();
		}
	}
}

void vui::RenderFrame::Link(LocalCardinal cardinal)
{
	if (cardinal == LocalCardinal::UP)
		m_LinkUp = true;
	else if (cardinal == LocalCardinal::DOWN)
		m_LinkDown = true;
	else if (cardinal == LocalCardinal::LEFT)
		m_LinkLeft = true;
	else if (cardinal == LocalCardinal::RIGHT)
		m_LinkRight = true;
}

void vui::RenderFrame::Unlink(LocalCardinal cardinal)
{
	if (cardinal == LocalCardinal::UP)
		m_LinkUp = false;
	else if (cardinal == LocalCardinal::DOWN)
		m_LinkDown = false;
	else if (cardinal == LocalCardinal::LEFT)
		m_LinkLeft = false;
	else if (cardinal == LocalCardinal::RIGHT)
		m_LinkRight = false;
}

void vui::RenderFrame::UpdateLinks()
{
	if (m_HasContent)
	{
		Logger::Error("can not update links of a RenderFrame that has content");
		return;
	}

	if (m_Space == PlaneSpace::HORIZONTAL)
	{
		if (m_Frames.size() == 1)
		{
			m_Frames.front()->Unlink(LocalCardinal::LEFT);
			m_Frames.front()->Unlink(LocalCardinal::RIGHT);
		}
		else
		{
			for (size_t i = 0; i < m_Frames.size(); i++)
			{
				PushLinks(m_Frames[i]);

				if (i == 0)
				{
					m_Frames[i]->Link(LocalCardinal::RIGHT);
				}
				else if (i == m_Frames.size() - 1)
				{
					m_Frames[i]->Link(LocalCardinal::LEFT);
				}
				else
				{
					m_Frames[i]->Link(LocalCardinal::LEFT);
					m_Frames[i]->Link(LocalCardinal::RIGHT);
				}
			}
		}
	}
	else if (m_Space == PlaneSpace::VERTICAL)
	{
		if (m_Frames.size() == 1)
		{
			m_Frames.front()->Unlink(LocalCardinal::UP);
			m_Frames.front()->Unlink(LocalCardinal::DOWN);
		}
		else
		{
			for (size_t i = 0; i < m_Frames.size(); i++)
			{
				PushLinks(m_Frames[i]);

				if (i == 0)
				{
					m_Frames[i]->Link(LocalCardinal::DOWN);
				}
				else if (i == m_Frames.size() - 1)
				{
					m_Frames[i]->Link(LocalCardinal::UP);
				}
				else
				{
					m_Frames[i]->Link(LocalCardinal::UP);
					m_Frames[i]->Link(LocalCardinal::DOWN);
				}
			}
		}
	}
}

void vui::RenderFrame::SetBar(size_t idx, int32_t offset)
{
	m_Bars[idx].Offset = offset;
	UpdateBarsFromRelative();
	UpdateContentDimentions();
}

void vui::RenderFrame::OnRender()
{
	if (m_HasFrame)
	{
		for (auto& frame : m_Frames)
			((IRenderable*)frame.get())->Render();
	}
	else if (m_HasContent)
	{
		bool erased = false;

		for (size_t i = 0; i < m_Content.size(); i++)
		{
			if (erased)
			{
				erased = false;
				i--;
			}

			if (auto content = m_Content[i].Renderable.lock())
				content->Render();
			else
			{
			m_Content.erase(m_Content.begin() + i);
			erased = true;
			}
		}
	}
}

void vui::RenderFrame::OnUpdateBuffers()
{
	ClearDirty();
	bool erased = false;

	for (size_t i = 0; i < m_Content.size(); i++)
	{
		if (erased)
		{
			erased = false;
			i--;
		}

		if (auto content = m_Content[i].Renderable.lock())
			content->UpdateBuffers();
		else
		{
			m_Content.erase(m_Content.begin() + i);
			erased = true;
		}
	}
}

void vui::RenderFrame::OnReloadSwapChain()
{
	if (m_HasFrame)
	{
		bool erased = false;

		for (auto& frame : m_Frames)
			((IRenderable*)frame.get())->ReloadSwapChain();
	}
	else if (m_HasContent)
	{
		bool erased = false;

		for (size_t i = 0; i < m_Content.size(); i++)
		{
			if (erased)
			{
				erased = false;
				i--;
			}

			if (auto content = m_Content[i].Renderable.lock())
				content->ReloadSwapChain();
			else
			{
				m_Content.erase(m_Content.begin() + i);
				erased = true;
			}
		}
	}
}

void vui::RenderFrame::OnSetWeak()
{
	m_MouseButtonData->Weak = m_Weak;
	m_MouseButtonData->Ptr = this;
}

void vui::RenderFrame::PostPositionUpdate()
{
	SetSizeMax();
	UpdateContentDimentions();
}

void vui::RenderFrame::PostSizeUpdate()
{
	UpdateContentDimentions();
}

void vui::RenderFrame::RenderFrameMouseButton(glm::vec<2, int32_t> position, float scroll, MouseOperation operation, void* data)
{
	RenderFrameMouseButtonData* system = (RenderFrameMouseButtonData*)data;
	if (auto frame = system->Weak.lock())
		system->Ptr->MouseButton(position, scroll, operation);
	else
		delete system;
}

void vui::RenderFrame::UpdateContentDimentions()
{
	if (m_HasFrame)
	{
		glm::vec<2, int32_t> runningPosition = m_Position + m_SuperOffset;
		glm::vec<2, int32_t> runningSize;

		for (size_t i = 0; i < m_Frames.size(); i++)
		{
			if (i > 0)
			{
				if (m_Space == PlaneSpace::HORIZONTAL)
					runningPosition.x = m_Position.x + m_SuperOffset.x + m_Bars[i - 1].Offset;
				else if (m_Space == PlaneSpace::VERTICAL)
					runningPosition.y = m_Position.y + m_SuperOffset.y + m_Bars[i - 1].Offset;
			}

			if (m_Frames.size() > 1)
			{
				int32_t offset = 0;

				if (m_Space == PlaneSpace::HORIZONTAL)
				{
					if (i == m_Frames.size() - 1)
						offset = m_Size.x - m_Bars[i - 1].Offset;
					else if (i == 0)
						offset = m_Bars[i].Offset;
					else
						offset = m_Bars[i].Offset - m_Bars[i - 1].Offset;

					runningSize = { offset - static_cast<int32_t>(static_cast<float>(0) * 1.5), m_Size.y - (static_cast<int32_t>(0) * 2) };
				}
				else if (m_Space == PlaneSpace::VERTICAL)
				{
					if (i == m_Frames.size() - 1)
						offset = m_Size.y - m_Bars[i - 1].Offset;
					else if (i == 0)
						offset = m_Bars[i].Offset;
					else
						offset = m_Bars[i].Offset - m_Bars[i - 1].Offset;

					runningSize = { m_Size.x - (static_cast<int32_t>(0) * 2), offset - static_cast<int32_t>(static_cast<float>(0) * 1.5) };
				}
			}
			else
			{
				runningSize = m_Size - (static_cast<int32_t>(0) * 2);
			}

			if (i == 0)
				m_Frames[i]->SetSuperOffset(runningPosition + static_cast<int32_t>(0));
			else
			{
				if (m_Space == PlaneSpace::HORIZONTAL)
					m_Frames[i]->SetSuperOffset(runningPosition + glm::vec<2, int32_t>(static_cast<int32_t>(0) / 2, static_cast<int32_t>(0)));
				else if (m_Space == PlaneSpace::VERTICAL)
					m_Frames[i]->SetSuperOffset(runningPosition + glm::vec<2, int32_t>(static_cast<int32_t>(0), static_cast<int32_t>(0) / 2));
			}

			m_Frames[i]->SetSuperBounds(runningSize);
		}
	}
	else if (m_HasContent)
	{
		bool erased = false;

		glm::vec<2, int32_t> runningPosition = m_Position + m_SuperOffset;
		glm::vec<2, int32_t> runningSize = { 0, 0 };

		if (m_LinkUp)
			runningPosition.y += static_cast<int32_t>(m_Padding) / 2;
		else
			runningPosition.y += static_cast<int32_t>(m_Padding);

		if (m_LinkLeft)
			runningPosition.x += static_cast<int32_t>(m_Padding) / 2;
		else
			runningPosition.x += static_cast<int32_t>(m_Padding);

		for (size_t i = 0; i < m_Content.size(); i++)
		{
			if (erased)
			{
				erased = false;
				i--;
			}

			if (i > 0)
			{
				if (m_Space == PlaneSpace::HORIZONTAL)
					runningPosition.x += runningSize.x + static_cast<int32_t>(m_Padding);
				else if (m_Space == PlaneSpace::VERTICAL)
					runningPosition.y += runningSize.y + static_cast<int32_t>(m_Padding);
			}
			
			if (m_Content.size() > 1)
			{
				if (m_Space == PlaneSpace::HORIZONTAL)
				{
					if (i == m_Content.size() - 1)
						runningSize.x = (m_Size.x + m_Position.x + m_SuperOffset.x) - runningPosition.x;
					else if (i == 0)
						runningSize.x = (m_Bars[i].Offset + m_Position.x + m_SuperOffset.x) - runningPosition.x;
					else
						runningSize.x = (m_Bars[i].Offset + m_Position.x + m_SuperOffset.x) - runningPosition.x;

					if (i < m_Content.size() - 1)
						runningSize.x -= static_cast<int32_t>(m_Padding) / 2;
					else
					{
						if (m_LinkRight)
							runningSize.x -= static_cast<int32_t>(m_Padding) / 2;
						else
							runningSize.x -= static_cast<int32_t>(m_Padding);
					}

					runningSize.y = m_Size.y - static_cast<int32_t>(m_Padding) * 2;

					if (m_LinkUp)
						runningSize.y += static_cast<int32_t>(m_Padding) / 2;
					if (m_LinkDown)
						runningSize.y += static_cast<int32_t>(m_Padding) / 2;
				}
				else if (m_Space == PlaneSpace::VERTICAL)
				{
					if (i == m_Content.size() - 1)
						runningSize.y = (m_Size.y + m_Position.y + m_SuperOffset.y) - runningPosition.y;
					else if (i == 0)
						runningSize.y = (m_Bars[i].Offset + m_Position.y + m_SuperOffset.y) - runningPosition.y;
					else
						runningSize.y = (m_Bars[i].Offset + m_Position.y + m_SuperOffset.y) - runningPosition.y;

					if (i < m_Content.size() - 1)
						runningSize.y -= static_cast<int32_t>(m_Padding) / 2;
					else
					{
						if (m_LinkDown)
							runningSize.y -= static_cast<int32_t>(m_Padding) / 2;
						else
							runningSize.y -= static_cast<int32_t>(m_Padding);
					}

					runningSize.x = m_Size.x - static_cast<int32_t>(m_Padding) * 2;

					if (m_LinkLeft)
						runningSize.x += static_cast<int32_t>(m_Padding) / 2;
					if (m_LinkRight)
						runningSize.x += static_cast<int32_t>(m_Padding) / 2;
				}
			}
			else
			{
				runningSize = m_Size;

				if (m_LinkUp)
					runningSize.y -= static_cast<int32_t>(m_Padding) / 2;
				else
					runningSize.y -= static_cast<int32_t>(m_Padding);

				if (m_LinkDown)
					runningSize.y -= static_cast<int32_t>(m_Padding) / 2;
				else
					runningSize.y -= static_cast<int32_t>(m_Padding);

				if (m_LinkLeft)
					runningSize.x -= static_cast<int32_t>(m_Padding) / 2;
				else
					runningSize.x -= static_cast<int32_t>(m_Padding);

				if (m_LinkRight)
					runningSize.x -= static_cast<int32_t>(m_Padding) / 2;
				else
					runningSize.x -= static_cast<int32_t>(m_Padding);

				//runningSize = m_Size - (static_cast<int32_t>(m_Padding) * 2);
			}

			if (auto content = m_Content[i].Positionable.lock())
			{
				content->SetSuperOffset(runningPosition);

				if (auto content = m_Content[i].Sizeable.lock())
					content->SetSuperBounds(runningSize);
				else
				{
					m_Content.erase(m_Content.begin() + i);
					erased = true;
				}
			}
			else
			{
				m_Content.erase(m_Content.begin() + i);
				erased = true;
			}


		}

		MarkDirty();
	}
}

void vui::RenderFrame::SubmitNewBarPosition(LocalCardinal cardinal)
{
	if (m_Bars.size() == 0)
	{
		if (cardinal == LocalCardinal::LEFT || cardinal == LocalCardinal::RIGHT)
			m_Bars.push_back(Bar(m_Size.x / 2));
		else if (cardinal == LocalCardinal::DOWN || cardinal == LocalCardinal::UP)
			m_Bars.push_back(Bar(m_Size.y / 2));
	}
	else
	{
		if (cardinal == LocalCardinal::RIGHT)
			m_Bars.insert(m_Bars.end(), (m_Size.x - m_Bars.back().Offset) / 2 + m_Bars.back().Offset);
		else if (cardinal == LocalCardinal::LEFT)
			m_Bars.insert(m_Bars.begin(), m_Bars.front().Offset / 2);
		else if (cardinal == LocalCardinal::DOWN)
			m_Bars.insert(m_Bars.end(), (m_Size.y - m_Bars.back().Offset) / 2 + m_Bars.back().Offset);
		else if (cardinal == LocalCardinal::UP)
			m_Bars.insert(m_Bars.begin(), m_Bars.front().Offset / 2);
	}

	UpdateBarsFromRelative();
	UpdateMouseButtonStatus(true);
}

void vui::RenderFrame::EqualizeBars()
{
	if (m_Space == PlaneSpace::HORIZONTAL)
	{
		for (size_t i = 0; i < m_Bars.size(); i++)
			m_Bars[i].Offset = (static_cast<float>(i + 1) / static_cast<float>(m_Bars.size() + 1)) * m_Size.x;
	}
	else if (m_Space == PlaneSpace::VERTICAL)
	{
		for (size_t i = 0; i < m_Bars.size(); i++)
			m_Bars[i].Offset = (static_cast<float>(i + 1) / static_cast<float>(m_Bars.size() + 1)) * m_Size.y;
	}

	UpdateBarsFromRelative();
}

void vui::RenderFrame::PushLinks(std::shared_ptr<RenderFrame>& frame)
{
	if (m_LinkUp)
		frame->Link(LocalCardinal::UP);
	if (m_LinkDown)
		frame->Link(LocalCardinal::DOWN);
	if (m_LinkLeft)
		frame->Link(LocalCardinal::LEFT);
	if (m_LinkRight)
		frame->Link(LocalCardinal::RIGHT);
}

void vui::RenderFrame::UpdateMouseButtonStatus(bool enabled)
{
	if (!IsWeak())
		return;

	if (enabled && !m_MouseButtonRegistered)
	{
		m_MouseButtonRegistered = true;
		InputHandler::RegisterMouseCallback(RenderFrameMouseButton, m_MouseButtonData);
	}
	else if (!enabled && m_MouseButtonRegistered)
	{
		m_MouseButtonRegistered = false;
		InputHandler::UnregisterMouseCallback(RenderFrameMouseButton);
	}
}

void vui::RenderFrame::UpdateBarsFromRelative()
{
	for (auto& bar : m_Bars)
	{
		bar.AbsolutePosition = m_Position + m_SuperOffset;

		if (m_Space == PlaneSpace::HORIZONTAL)
		{
			bar.AbsolutePosition.x += bar.Offset;
			bar.AbsolutePosition.x -= static_cast<int32_t>(m_Padding) / 2;

			bar.Size = { static_cast<int32_t>(m_Padding), m_Size.y };

			if (m_LinkUp)
				bar.Size.y -= static_cast<int32_t>(m_Padding) / 2;
			else
				bar.Size.y -= static_cast<int32_t>(m_Padding);

			if (m_LinkDown)
				bar.Size.y -= static_cast<int32_t>(m_Padding) / 2;
			else
				bar.Size.y -= static_cast<int32_t>(m_Padding);

			//std::cout << bar.AbsolutePosition.x << ", " << bar.AbsolutePosition.y << " | " << bar.Size.x << "x" << bar.Size.y << std::endl;
		}
		else if (m_Space == PlaneSpace::VERTICAL)
		{
			bar.AbsolutePosition.y += bar.Offset;
			bar.AbsolutePosition.y -= static_cast<int32_t>(m_Padding) / 2;

			bar.Size = { m_Size.x, static_cast<int32_t>(m_Padding) };

			if (m_LinkLeft)
				bar.Size.x -= static_cast<int32_t>(m_Padding) / 2;
			else
				bar.Size.x -= static_cast<int32_t>(m_Padding);

			if (m_LinkRight)
				bar.Size.x -= static_cast<int32_t>(m_Padding) / 2;
			else
				bar.Size.x -= static_cast<int32_t>(m_Padding);
		}
	}
}

void vui::RenderFrame::UpdateBarsFromAbsolute()
{
	for (auto& bar : m_Bars)
	{
		if (m_Space == PlaneSpace::HORIZONTAL)
			bar.Offset = bar.AbsolutePosition.x - m_Position.x - m_SuperOffset.x + (static_cast<int32_t>(m_Padding) / 2);
		else if (m_Space == PlaneSpace::VERTICAL)
			bar.Offset = bar.AbsolutePosition.y - m_Position.y - m_SuperOffset.y + (static_cast<int32_t>(m_Padding) / 2);
	}
}

void vui::RenderFrame::ResetDrag()
{
	m_Dragging = false;
	m_DraggingBarIdx = -1;
	m_DraggingMouseBegin = { 0 ,0 };
	m_DraggingObjectBegin = { 0, 0 };
}
