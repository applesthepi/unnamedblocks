#include "layer.hpp"

rhr::render::layer::layer()
	: rhr::render::interfaces::i_enableable(true)
{
	m_frames.reserve(16);
}

void rhr::render::layer::add_frame(std::weak_ptr<rhr::render::frame>&& frame)
{
	std::unique_lock lock(m_mutex);
	m_frames.push_back(std::move(frame));
}

void rhr::render::layer::render()
{
	if (!m_enabled)
		return;

	std::unique_lock lock(m_mutex);
	bool erased = false;

	for (usize i = 0; i < m_frames.size(); i++)
	{
		if (erased)
		{
			erased = false;
			i--;
		}

		if (auto f = m_frames[i].lock())
			f->render();
		else
		{
			m_frames.erase(m_frames.begin() + i);
			erased = true;
		}
	}
}

void rhr::render::layer::reload_swap_chain()
{
	std::unique_lock lock(m_mutex);
	bool erased = false;

	for (usize i = 0; i < m_frames.size(); i++)
	{
		if (erased)
		{
			erased = false;
			i--;
		}

		if (auto f = m_frames[i].lock())
			f->reload_swap_chain();
		else
		{
			m_frames.erase(m_frames.begin() + i);
			erased = true;
		}
	}
}