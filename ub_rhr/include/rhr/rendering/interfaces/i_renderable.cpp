#include "i_renderable.hpp"

#include "rhr/rendering/renderer.hpp"

rhr::render::interfaces::i_renderable::i_renderable()
	: m_dirty(false)
{}

void rhr::render::interfaces::i_renderable::render()
{
	// if (!is_weak())
	//	return;

	on_render();
}

void rhr::render::interfaces::i_renderable::update_buffers()
{
	// if (!is_weak())
	//	return;

	m_dirty = false;
	on_update_buffers();
}

void rhr::render::interfaces::i_renderable::reload_swap_chain()
{
	// if (!is_weak())
	//	return;

	on_reload_swap_chain();
}

void rhr::render::interfaces::i_renderable::on_render()
{}

void rhr::render::interfaces::i_renderable::on_update_buffers()
{}

void rhr::render::interfaces::i_renderable::on_reload_swap_chain()
{}

void rhr::render::interfaces::i_renderable::mark_dirty()
{
	if (/*!is_weak()||*/ m_dirty)
		return;

	on_mark_dirty();

	m_dirty = true;
	// rhr::render::renderer::add_dirty(m_weak);
}

void rhr::render::interfaces::i_renderable::clear_dirty()
{
	m_dirty = false;
}

void rhr::render::interfaces::i_renderable::on_mark_dirty()
{}
