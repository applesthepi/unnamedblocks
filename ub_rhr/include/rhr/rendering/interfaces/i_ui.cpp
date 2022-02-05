#include "i_ui.hpp"

#include "rhr/rendering/renderer.hpp"

rhr::render::interfaces::i_ui::i_ui()
	: m_position_local_physical({})
	, m_position_local_virtual_offset({})
	, m_position_parent_physical({})
	, m_position_parent_virtual_offset({})
	, m_position_physical_absolute({})
	, m_position_virtual_offset({})
	, m_size_local({})
	, m_size_parent({})
	, m_enabled(true)
	, m_dirty(false)
	, m_weak_set(false)
{}

void rhr::render::interfaces::i_ui::set_weak(std::weak_ptr<i_ui> weak)
{
	m_weak	   = std::move(weak);
	m_weak_set = true;
}

void rhr::render::interfaces::i_ui::set_position_local_physical(const glm::vec<2, i32>& offset, bool update_child)
{
	m_position_local_physical	 = offset;
	m_position_physical_absolute = m_position_local_physical + m_position_parent_physical;
	m_position_virtual_absolute	 = m_position_virtual_offset + m_position_physical_absolute;

	if (update_child)
		update_transform(i_ui::transform_update_spec_position);
}

void rhr::render::interfaces::i_ui::set_position_local_virtual_offset(const glm::vec<2, i32>& offset, bool update_child)
{
	m_position_local_virtual_offset = offset;
	m_position_virtual_offset		= m_position_local_virtual_offset + m_position_parent_virtual_offset;
	m_position_virtual_absolute		= m_position_virtual_offset + m_position_physical_absolute;

	if (update_child)
		update_transform(i_ui::transform_update_spec_position);
}

void rhr::render::interfaces::i_ui::set_position_parent_physical(const glm::vec<2, i32>& offset, bool update_child)
{
	m_position_parent_physical	 = offset;
	m_position_physical_absolute = m_position_local_physical + m_position_parent_physical;
	m_position_virtual_absolute	 = m_position_virtual_offset + m_position_physical_absolute;

	if (update_child)
		update_transform(i_ui::transform_update_spec_position);
}

void rhr::render::interfaces::i_ui::set_position_parent_virtual_offset(const glm::vec<2, i32>& offset, bool update_child)
{
	m_position_parent_virtual_offset = offset;
	m_position_virtual_offset		 = m_position_local_virtual_offset + m_position_parent_virtual_offset;
	m_position_virtual_absolute		 = m_position_virtual_offset + m_position_physical_absolute;

	if (update_child)
		update_transform(i_ui::transform_update_spec_position);
}

void rhr::render::interfaces::i_ui::set_size_local(const glm::vec<2, i32>& size, bool update_child)
{
	m_size_local = size;

	if (update_child)
		update_transform(i_ui::transform_update_spec_size);
}

void rhr::render::interfaces::i_ui::set_size_parent(const glm::vec<2, i32>& size, bool update_child)
{
	m_size_parent = size;

	if (update_child)
		update_transform(i_ui::transform_update_spec_size);
}

void rhr::render::interfaces::i_ui::set_size_max(bool update_child)
{
	m_size_local = m_size_parent;

	if (update_child)
		update_transform(i_ui::transform_update_spec_size);
}

const glm::vec<2, i32>& rhr::render::interfaces::i_ui::get_position_local_physical() { return m_position_local_physical; }

const glm::vec<2, i32>& rhr::render::interfaces::i_ui::get_position_local_virtual_offset() { return m_position_local_virtual_offset; }

const glm::vec<2, i32>& rhr::render::interfaces::i_ui::get_position_parent_physical() { return m_position_parent_physical; }

const glm::vec<2, i32>& rhr::render::interfaces::i_ui::get_position_parent_virtual_offset() { return m_position_parent_virtual_offset; }

const glm::vec<2, i32>& rhr::render::interfaces::i_ui::get_position_physical_absolute() { return m_position_physical_absolute; }

const glm::vec<2, i32>& rhr::render::interfaces::i_ui::get_position_virtual_offset() { return m_position_virtual_offset; }

const glm::vec<2, i32>& rhr::render::interfaces::i_ui::get_position_virtual_absolute() { return m_position_virtual_absolute; }

const glm::vec<2, i32>& rhr::render::interfaces::i_ui::get_size_local() { return m_size_local; }

const glm::vec<2, i32>& rhr::render::interfaces::i_ui::get_size_parent() { return m_size_parent; }

void rhr::render::interfaces::i_ui::update_transform(rhr::render::interfaces::i_ui::transform_update_spec transform_update_spec) { ui_transform_update(transform_update_spec); }

void rhr::render::interfaces::i_ui::update_child_transform(const std::shared_ptr<rhr::render::interfaces::i_ui>& ui, i_ui::transform_update_spec transform_update_spec)
{
	ui->set_position_parent_physical(get_position_physical_absolute(), false);
	ui->set_position_parent_virtual_offset(get_position_virtual_offset(), false);
	ui->set_size_parent(get_size_local(), false);

	if (transform_update_spec)
		ui->update_transform(transform_update_spec);
}

void rhr::render::interfaces::i_ui::set_enabled(bool enabled) { m_enabled = enabled; }

bool rhr::render::interfaces::i_ui::get_enabled() { return m_enabled; }

void rhr::render::interfaces::i_ui::render()
{
	if (m_enabled)
		ui_render();
}

void rhr::render::interfaces::i_ui::reload_swap_chain() { ui_reload_swap_chain(); }

void rhr::render::interfaces::i_ui::update_buffers()
{
	if (m_dirty)
	{
		m_dirty = false;
		ui_update_buffers();
	}

	ui_chain_update_buffers();
}

void rhr::render::interfaces::i_ui::frame_update(f64 delta_time)
{
	if (m_enabled)
		ui_frame_update(delta_time);
}

void rhr::render::interfaces::i_ui::mark_dirty()
{
	m_dirty = true;
	//if (!is_weak() || m_dirty)
	//	return;
	//
	//m_dirty = true;
	//rhr::render::renderer::add_dirty(m_weak);
}

void rhr::render::interfaces::i_ui::ui_transform_update(i_ui::transform_update_spec transform_update_spec) {}

void rhr::render::interfaces::i_ui::ui_render() {}

void rhr::render::interfaces::i_ui::ui_reload_swap_chain() {}

void rhr::render::interfaces::i_ui::ui_update_buffers() {}

void rhr::render::interfaces::i_ui::ui_chain_update_buffers() {}

void rhr::render::interfaces::i_ui::ui_frame_update(f64 delta_time) {}

rhr::render::interfaces::i_ui::transform_update_spec rhr::render::interfaces::i_ui::transform_update_spec_position = 0x1;

rhr::render::interfaces::i_ui::transform_update_spec rhr::render::interfaces::i_ui::transform_update_spec_size = 0x2;

bool rhr::render::interfaces::i_ui::is_weak()
{
	if (!m_weak_set)
	{
		cap::logger::warn(cap::logger::level::SYSTEM, "rhr::render::interfaces::i_ui::is_weak() returning false");
		return false;
	}

	return true;
}
