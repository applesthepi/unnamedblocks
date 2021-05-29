#include "RenderLayer.h"

vui::RenderLayer::RenderLayer()
    : m_Enabled(false)
{
	m_Frames.reserve(16);
}

void vui::RenderLayer::SetEnabled(bool enabled)
{
	m_Enabled = enabled;
}

void vui::RenderLayer::AddFrame(std::weak_ptr<RenderFrame>&& frame)
{
	std::lock_guard<SHARED_MUTEX> lock(m_Mutex);
	m_Frames.push_back(frame);
}

void vui::RenderLayer::Render()
{
	if (!m_Enabled)
		return;

	std::lock_guard<SHARED_MUTEX> lock(m_Mutex);
	bool erased = false;

	for (size_t i = 0; i < m_Frames.size(); i++)
	{
		if (erased)
		{
			erased = false;
			i--;
		}

		if (auto f = m_Frames[i].lock())
			f->Render();
		else
		{
			m_Frames.erase(m_Frames.begin() + i);
			erased = true;
		}
	}
}

void vui::RenderLayer::ReloadSwapChain()
{
	std::lock_guard<SHARED_MUTEX> lock(m_Mutex);
	bool erased = false;

	for (size_t i = 0; i < m_Frames.size(); i++)
	{
		if (erased)
		{
			erased = false;
			i--;
		}

		if (auto f = m_Frames[i].lock())
			f->ReloadSwapChain();
		else
		{
			m_Frames.erase(m_Frames.begin() + i);
			erased = true;
		}
	}
}