#include "RenderFrame.h"
#include "../Renderer.h"

vui::RenderFrame::RenderFrame()
    : m_Space(vui::PlaneSpace::HORIZONTAL)
    , m_HasSpace(false)
    , m_HasFrame(false)
    , m_HasContent(false)
    , m_Dirty(false)
{
}

void vui::RenderFrame::SetWeak(std::weak_ptr<RenderFrame>&& weak)
{
	m_Weak = std::move(weak);
}

void vui::RenderFrame::SetPosition(glm::vec<2, int32_t> position)
{
	m_Position = position;
	UpdateContentDimentions();
}

void vui::RenderFrame::SetSize(glm::vec<2, int32_t> size)
{
	m_Size = size;
	UpdateContentDimentions();
}

void vui::RenderFrame::SetPadding(uint8_t padding)
{
	m_Padding = padding;
	UpdateContentDimentions();
}

void vui::RenderFrame::SetFrame(std::unique_ptr<RenderFrame>& frame)
{
	if (m_HasContent)
	{
		Logger::Error(SIDE::CLIENT, "can not set frame of RenderFrame that has content");
		return;
	}

	m_HasSpace = false;
	m_HasFrame = true;

	m_Frames.clear();
	m_Frames.push_back(std::move(frame));
}

void vui::RenderFrame::AddFrame(std::unique_ptr<RenderFrame>& frame, LocalCardinal cardinal)
{
	if (m_HasContent)
	{
		Logger::Error(SIDE::CLIENT, "can not add frame to RenderFrame that has content");
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
		m_Frames.insert(m_Frames.begin(), std::move(frame));
	else if (cardinal == LocalCardinal::DOWN && m_Space == PlaneSpace::VERTICAL)
		m_Frames.insert(m_Frames.end(), std::move(frame));
	else if (cardinal == LocalCardinal::LEFT && m_Space == PlaneSpace::HORIZONTAL)
		m_Frames.insert(m_Frames.begin(), std::move(frame));
	else if (cardinal == LocalCardinal::RIGHT && m_Space == PlaneSpace::HORIZONTAL)
		m_Frames.insert(m_Frames.end(), std::move(frame));
	else
		Logger::Error(SIDE::CLIENT, "wrong direction when adding a frame to a RenderFrame");
}

void vui::RenderFrame::AddContent(std::weak_ptr<RenderUI>&& content)
{
	if (m_HasFrame)
	{
		Logger::Error(SIDE::CLIENT, "can not add content to RenderFrame that has internal frames");
		return;
	}

	m_HasContent = true;
	m_Content.push_back(std::move(content));
	UpdateContentDimentions();
}

void vui::RenderFrame::Render()
{
	if (m_HasFrame)
	{
		for (size_t i = 0; i < m_Frames.size(); i++)
			m_Frames[i]->Render();
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

			if (auto content = m_Content[i].lock())
				content->Render();
			else
			{
				m_Content.erase(m_Content.begin() + i);
				erased = true;
			}
		}
	}
}

void vui::RenderFrame::UpdateBuffers()
{
	m_Dirty = false;
	bool erased = false;

	for (size_t i = 0; i < m_Content.size(); i++)
	{
		if (erased)
		{
			erased = false;
			i--;
		}

		if (auto content = m_Content[i].lock())
			content->UpdateBuffers();
		else
		{
			m_Content.erase(m_Content.begin() + i);
			erased = true;
		}
	}
}

void vui::RenderFrame::ReloadSwapChain()
{
	if (m_HasFrame)
	{
		bool erased = false;

		for (auto& frame : m_Frames)
			frame->ReloadSwapChain();
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

			if (auto content = m_Content[i].lock())
				content->ReloadSwapChain();
			else
			{
				m_Content.erase(m_Content.begin() + i);
				erased = true;
			}
		}
	}
}

void vui::RenderFrame::UpdateContentDimentions()
{
	if (m_HasFrame)
	{
		if (m_Space == vui::PlaneSpace::HORIZONTAL)
		{
			int32_t padding = m_Padding;
			int32_t absOffset = m_Position.x + padding;
			int32_t totalContentWidth = m_Size.x - (padding * 2);
			int32_t totalContentHeight = m_Size.y - (padding * 2);
			int32_t singleContentWidth = totalContentWidth / static_cast<int32_t>(m_Frames.size());
			int32_t singleCompleteWidth = singleContentWidth + padding;

			for (size_t i = 0; i < m_Frames.size(); i++)
			{
				m_Frames[i]->SetPosition({ absOffset + (i * singleCompleteWidth), padding });
				m_Frames[i]->SetSize({ singleContentWidth, totalContentHeight });
			}
		}
		else if (m_Space == vui::PlaneSpace::VERTICAL)
		{
			int32_t padding = m_Padding;
			int32_t absOffset = m_Position.y + padding;
			int32_t totalContentHeight = m_Size.y - (padding * 2);
			int32_t totalContentWidth = m_Size.x - (padding * 2);
			int32_t singleContentHeight = totalContentHeight / static_cast<int32_t>(m_Frames.size());
			int32_t singleCompleteHeight = singleContentHeight + padding;

			for (size_t i = 0; i < m_Frames.size(); i++)
			{
				m_Frames[i]->SetPosition({ padding, absOffset + (i * singleCompleteHeight) });
				m_Frames[i]->SetSize({ totalContentWidth, singleContentHeight });
			}
		}
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

			if (auto content = m_Content[i].lock())
				content->SetParent({ m_Position.x + m_Padding, m_Position.y + m_Padding }, { m_Size.x - (m_Padding * 2), m_Size.y - (m_Padding * 2) });
			else
			{
				m_Content.erase(m_Content.begin() + i);
				erased = true;
			}
		}

		MarkDirty();
	}
}

void vui::RenderFrame::MarkDirty()
{
	if (m_Dirty || !m_HasContent)
		return;

	m_Dirty = true;
	Renderer::AddDirtyFrame(m_Weak);
}