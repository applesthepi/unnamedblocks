#include "RenderFrame.hpp"

#include "ui/Renderer.hpp"

vui::RenderFrame::RenderFrame()
	: m_Space(vui::PlaneSpace::HORIZONTAL)
	, m_HasSpace(false)
	, m_HasFrame(false)
	, m_HasContent(false)
	, m_LinkUp(false)
	, m_LinkDown(false)
	, m_LinkLeft(false)
	, m_LinkRight(false)
	, m_Padding(6)
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

	m_HasSpace = false;
	m_HasFrame = true;

	m_Frames.clear();
	m_Frames.push_back(std::move(frame));
}

void vui::RenderFrame::AddFrame(std::shared_ptr<RenderFrame>& frame, LocalCardinal cardinal)
{
	if (m_HasContent)
	{
		Logger::Error("can not add frame to RenderFrame that has content");
		return;
	}

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

	m_HasContent = true;

	if (!m_HasSpace)
	{
		m_HasSpace = true;

		if (cardinal == LocalCardinal::UP || cardinal == LocalCardinal::DOWN)
			m_Space = PlaneSpace::VERTICAL;
		else if (cardinal == LocalCardinal::LEFT || cardinal == LocalCardinal::RIGHT)
			m_Space = PlaneSpace::HORIZONTAL;
	}

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

bool vui::RenderFrame::OnPositionUpdate(const glm::vec<2, int32_t>& position, const glm::vec<2, int32_t>& offset)
{
	UpdateContentDimentions();
	return true;
}

bool vui::RenderFrame::OnSizeUpdate(const glm::vec<2, int32_t>& size, const glm::vec<2, int32_t>& bounds)
{
	UpdateContentDimentions();
	return true;
}

void vui::RenderFrame::UpdateContentDimentions()
{
	if (m_HasFrame)
	{
		//if (m_Space == vui::PlaneSpace::HORIZONTAL)
		//{
		//	int32_t padding = m_Padding;
		//	int32_t absOffset = m_Position.x + padding;
		//	int32_t totalContentWidth = m_Size.x - (padding * 2);
		//	int32_t totalContentHeight = m_Size.y - (padding * 2);
		//	int32_t singleContentWidth = totalContentWidth / static_cast<int32_t>(m_Frames.size());
		//	int32_t singleCompleteWidth = singleContentWidth + padding;

		//	for (size_t i = 0; i < m_Frames.size(); i++)
		//	{
		//		m_Frames[i]->SetPosition({ absOffset + (i * singleCompleteWidth), padding });
		//		m_Frames[i]->SetSize({ singleContentWidth, totalContentHeight });
		//		m_Frames[i]->SetSuperBounds({ singleContentWidth, totalContentHeight });
		//	}
		//}
		//else if (m_Space == vui::PlaneSpace::VERTICAL)
		//{
		//	int32_t padding = m_Padding;
		//	int32_t absOffset = m_Position.y + padding;
		//	int32_t totalContentHeight = m_Size.y - (padding * 2);
		//	int32_t totalContentWidth = m_Size.x - (padding * 2);
		//	int32_t singleContentHeight = totalContentHeight / static_cast<int32_t>(m_Frames.size());
		//	int32_t singleCompleteHeight = singleContentHeight + padding;

		//	for (size_t i = 0; i < m_Frames.size(); i++)
		//	{
		//		m_Frames[i]->SetPosition({ padding, absOffset + (i * singleCompleteHeight) });
		//		m_Frames[i]->SetSize({ totalContentWidth, singleContentHeight });
		//		m_Frames[i]->SetSuperBounds({ totalContentWidth, singleContentHeight });
		//	}
		//}

		glm::vec<2, int32_t> runningPosition = m_Position + m_SuperOffset;
		glm::vec<2, int32_t> runningSize;

		for (size_t i = 0; i < m_Frames.size(); i++)
		{
			if (i > 0)
			{
				if (m_Space == PlaneSpace::HORIZONTAL)
					runningPosition.x = m_Position.x + m_SuperOffset.x + m_BarOffsets[i - 1];
				else if (m_Space == PlaneSpace::VERTICAL)
					runningPosition.y = m_Position.y + m_SuperOffset.y + m_BarOffsets[i - 1];
			}

			if (m_Frames.size() > 1)
			{
				int32_t offset = 0;

				if (m_Space == PlaneSpace::HORIZONTAL)
				{
					if (i == m_Frames.size() - 1)
						offset = m_Size.x - m_BarOffsets[i - 1];
					else if (i == 0)
						offset = m_BarOffsets[i];
					else
						offset = m_BarOffsets[i] - m_BarOffsets[i - 1];

					runningSize = { offset - static_cast<int32_t>(static_cast<float>(0) * 1.5), m_Size.y - (static_cast<int32_t>(0) * 2) };
				}
				else if (m_Space == PlaneSpace::VERTICAL)
				{
					if (i == m_Frames.size() - 1)
						offset = m_Size.y - m_BarOffsets[i - 1];
					else if (i == 0)
						offset = m_BarOffsets[i];
					else
						offset = m_BarOffsets[i] - m_BarOffsets[i - 1];

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
				//if (m_Space == PlaneSpace::HORIZONTAL)
				//	runningPosition = { m_Position.x + m_SuperOffset.x + m_BarOffsets[i - 1], m_Position.y + m_SuperOffset.y };
				//else if (m_Space == PlaneSpace::VERTICAL)
				//	runningPosition = { m_Position.x + m_SuperOffset.x, m_Position.y + m_SuperOffset.y + m_BarOffsets[i - 1] };
			}
			
			if (m_Content.size() > 1)
			{
				if (m_Space == PlaneSpace::HORIZONTAL)
				{
					if (i == m_Content.size() - 1)
						runningSize.x = (m_Size.x + m_Position.x + m_SuperOffset.x) - runningPosition.x;
					else if (i == 0)
						runningSize.x = (m_BarOffsets[i] + m_Position.x + m_SuperOffset.x) - runningPosition.x;
					else
						runningSize.x = (m_BarOffsets[i] + m_Position.x + m_SuperOffset.x) - runningPosition.x;

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
						runningSize.y = (m_BarOffsets[i] + m_Position.y + m_SuperOffset.y) - runningPosition.y;
					else
						runningSize.y = (m_BarOffsets[i] + m_Position.y + m_SuperOffset.y) - runningPosition.y;

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

					//if (i == m_Content.size() - 1)
					//	offset = m_Size.y - m_BarOffsets[i - 1];
					//else if (i == 0)
					//	offset = m_BarOffsets[i];
					//else
					//	offset = m_BarOffsets[i] - m_BarOffsets[i - 1];

					//if (m_LinkDown)
					//	runningSize = { m_Size.x - (static_cast<int32_t>(m_Padding) * 2), offset - static_cast<int32_t>(static_cast<float>(m_Padding) * 2.0) };
					//else
					//	runningSize = { m_Size.x - (static_cast<int32_t>(m_Padding) * 2), offset - static_cast<int32_t>(static_cast<float>(m_Padding) * 1.0) };

					//if (i == m_Content.size() - 1)
					//	offset.y = m_Size.y - m_BarOffsets[i - 1];
					//else if (i == 0)
					//	offset.y = m_BarOffsets[i];
					//else
					//	offset.y = m_BarOffsets[i] - m_BarOffsets[i - 1];

					//offset.y -= static_cast<int32_t>(m_Padding) * 2;

					//if (m_LinkUp)
					//	offset.y += static_cast<int32_t>(m_Padding) / 2;
					//if (m_LinkDown)
					//	offset.y += static_cast<int32_t>(m_Padding) / 2;

					//offset.x = m_Size.x - static_cast<int32_t>(m_Padding) * 2;

					//if (m_LinkLeft)
					//	offset.x += static_cast<int32_t>(m_Padding) / 2;
					//if (m_LinkRight)
					//	offset.x += static_cast<int32_t>(m_Padding) / 2;

					//runningSize = offset;
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
				if (i == 0)
				{
					//if (m_Space == PlaneSpace::HORIZONTAL)
					//{
					//	if (m_LinkLeft)
					//		content->SetSuperOffset(runningPosition + glm::vec<2, int32_t>(static_cast<int32_t>(m_Padding) / 2, static_cast<int32_t>(m_Padding)));
					//	else
					//		content->SetSuperOffset(runningPosition + static_cast<int32_t>(m_Padding));
					//}
					//else if (m_Space == PlaneSpace::VERTICAL)
					//{
					//	if (m_LinkUp)
					//		content->SetSuperOffset(runningPosition + glm::vec<2, int32_t>(static_cast<int32_t>(m_Padding), static_cast<int32_t>(m_Padding) / 2));
					//	else
					//		content->SetSuperOffset(runningPosition + static_cast<int32_t>(m_Padding));
					//}


					//if (m_LinkLeft)
					//	runningPosition += glm::vec<2, int32_t>(static_cast<int32_t>(m_Padding) / 2, 0);
					//else
					//	runningPosition += glm::vec<2, int32_t>(static_cast<int32_t>(m_Padding), 0);

					//if (m_LinkUp)
					//	runningPosition += glm::vec<2, int32_t>(0, static_cast<int32_t>(m_Padding) / 2);
					//else
					//	runningPosition += glm::vec<2, int32_t>(0, static_cast<int32_t>(m_Padding));

					content->SetSuperOffset(runningPosition);
				}
				else
				{
					//if (m_Space == PlaneSpace::HORIZONTAL)
					//	content->SetSuperOffset(runningPosition + glm::vec<2, int32_t>(static_cast<int32_t>(m_Padding) / 2, static_cast<int32_t>(m_Padding)));
					//else if (m_Space == PlaneSpace::VERTICAL)
					//	content->SetSuperOffset(runningPosition + glm::vec<2, int32_t>(static_cast<int32_t>(m_Padding), static_cast<int32_t>(m_Padding) / 2));

					//if (m_Space == PlaneSpace::HORIZONTAL)
					//{
					//	runningPosition += glm::vec<2, int32_t>(static_cast<int32_t>(m_Padding), 0);

					//	if (m_LinkUp)
					//		runningPosition += glm::vec<2, int32_t>(0, static_cast<int32_t>(m_Padding) / 2);
					//	else
					//		runningPosition += glm::vec<2, int32_t>(0, static_cast<int32_t>(m_Padding));
					//}
					//else if (m_Space == PlaneSpace::VERTICAL)
					//{
					//	if (m_LinkLeft)
					//		runningPosition += glm::vec<2, int32_t>(static_cast<int32_t>(m_Padding) / 2, 0);
					//	else
					//		runningPosition += glm::vec<2, int32_t>(static_cast<int32_t>(m_Padding), 0);

					//	runningPosition += glm::vec<2, int32_t>(0, static_cast<int32_t>(m_Padding));
					//}

					//if (m_LinkLeft)
					//	runningPosition += glm::vec<2, int32_t>(static_cast<int32_t>(m_Padding) / 2, 0);
					//else
					//	runningPosition += glm::vec<2, int32_t>(static_cast<int32_t>(m_Padding), 0);

					//if (m_LinkUp)
					//	runningPosition += glm::vec<2, int32_t>(0, static_cast<int32_t>(m_Padding) / 2);
					//else
					//	runningPosition += glm::vec<2, int32_t>(0, static_cast<int32_t>(m_Padding));

					content->SetSuperOffset(runningPosition);
				}

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
	if (m_BarOffsets.size() == 0)
	{
		if (cardinal == LocalCardinal::LEFT || cardinal == LocalCardinal::RIGHT)
			m_BarOffsets.push_back(m_Size.x / 2);
		else if (cardinal == LocalCardinal::DOWN || cardinal == LocalCardinal::UP)
			m_BarOffsets.push_back(m_Size.y / 2);
	}
	else
	{
		if (cardinal == LocalCardinal::RIGHT)
			m_BarOffsets.insert(m_BarOffsets.end(), (m_Size.x - m_BarOffsets.back()) / 2 + m_BarOffsets.back());
		else if (cardinal == LocalCardinal::LEFT)
			m_BarOffsets.insert(m_BarOffsets.begin(), m_BarOffsets.front() / 2);
		else if (cardinal == LocalCardinal::DOWN)
			m_BarOffsets.insert(m_BarOffsets.end(), (m_Size.y - m_BarOffsets.back()) / 2 + m_BarOffsets.back());
		else if (cardinal == LocalCardinal::UP)
			m_BarOffsets.insert(m_BarOffsets.begin(), m_BarOffsets.front() / 2);
	}
}

void vui::RenderFrame::EqualizeBars()
{
	if (m_Space == PlaneSpace::HORIZONTAL)
	{
		for (size_t i = 0; i < m_BarOffsets.size(); i++)
			m_BarOffsets[i] = (static_cast<float>(i + 1) / static_cast<float>(m_BarOffsets.size() + 1)) * m_Size.x;
	}
	else if (m_Space == PlaneSpace::VERTICAL)
	{
		for (size_t i = 0; i < m_BarOffsets.size(); i++)
			m_BarOffsets[i] = (static_cast<float>(i + 1) / static_cast<float>(m_BarOffsets.size() + 1)) * m_Size.y;
	}
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
