#include "i_ui.hpp"

rhr::render::interfaces::i_ui::i_ui()
	: m_position_local_physical({})
	, m_position_local_virtual_offset({})
	, m_position_parent_physical({})
	, m_position_parent_virtual_offset({})
	, m_position_physical_absolute({})
	, m_position_virtual_offset({})
	, m_size_local({})
	, m_size_parent({})
{

}

void rhr::render::interfaces::i_ui::set_position_local_physical(const glm::vec<2, i32>& offset, bool update_child)
{
	m_position_local_physical = offset;
	m_position_physical_absolute = m_position_local_physical + m_position_parent_physical;

	if (update_child)
		update_transform();
}

void rhr::render::interfaces::i_ui::set_position_local_virtual_offset(const glm::vec<2, i32>& offset, bool update_child)
{
	m_position_local_virtual_offset = offset;
	m_position_virtual_offset = m_position_local_virtual_offset + m_position_parent_virtual_offset;
	m_position_virtual_absolute = m_position_virtual_offset + m_position_physical_absolute;

	if (update_child)
		update_transform();
}

void rhr::render::interfaces::i_ui::set_position_parent_physical(const glm::vec<2, i32>& offset, bool update_child)
{
	m_position_parent_physical = offset;
	m_position_physical_absolute = m_position_local_physical + m_position_parent_physical;

	if (update_child)
		update_transform();
}

void rhr::render::interfaces::i_ui::set_position_parent_virtual_offset(const glm::vec<2, i32>& offset, bool update_child)
{
	m_position_parent_virtual_offset = offset;
	m_position_virtual_offset = m_position_local_virtual_offset + m_position_parent_virtual_offset;
	m_position_virtual_absolute = m_position_virtual_offset + m_position_physical_absolute;

	if (update_child)
		update_transform();
}

void rhr::render::interfaces::i_ui::set_size_local(const glm::vec<2, i32>& size, bool update_child)
{
	m_size_local = size;

	if (update_child)
		update_transform();
}

void rhr::render::interfaces::i_ui::set_size_parent(const glm::vec<2, i32>& size, bool update_child)
{
	m_size_parent = size;

	if (update_child)
		update_transform();
}

void rhr::render::interfaces::i_ui::set_size_max(bool update_child)
{
	m_size_local = m_size_parent;

	if (update_child)
		update_transform();
}

const glm::vec<2, i32>& rhr::render::interfaces::i_ui::get_position_local_physical()
{
	return m_position_local_physical;
}

const glm::vec<2, i32>& rhr::render::interfaces::i_ui::get_position_local_virtual_offset()
{
	return m_position_local_virtual_offset;
}

const glm::vec<2, i32>& rhr::render::interfaces::i_ui::get_position_parent_physical()
{
	return m_position_parent_physical;
}

const glm::vec<2, i32>& rhr::render::interfaces::i_ui::get_position_parent_virtual_offset()
{
	return m_position_parent_virtual_offset;
}

const glm::vec<2, i32>& rhr::render::interfaces::i_ui::get_position_physical_absolute()
{
	return m_position_physical_absolute;
}

const glm::vec<2, i32>& rhr::render::interfaces::i_ui::get_position_virtual_offset()
{
	return m_position_virtual_offset;
}

const glm::vec<2, i32>& rhr::render::interfaces::i_ui::get_position_virtual_absolute()
{
	return m_position_virtual_absolute;
}

const glm::vec<2, i32>& rhr::render::interfaces::i_ui::get_size_local()
{
	return m_size_local;
}

const glm::vec<2, i32>& rhr::render::interfaces::i_ui::get_size_parent()
{
	return m_size_parent;
}

void rhr::render::interfaces::i_ui::update_transform()
{
	post_transform_update();
}

void rhr::render::interfaces::i_ui::update_child_transform(const std::shared_ptr<rhr::render::interfaces::i_ui>& ui, bool update_child)
{
	ui->set_position_parent_physical(get_position_physical_absolute(), false);
	ui->set_position_parent_virtual_offset(get_position_virtual_offset(), false);
	ui->set_size_parent(get_size_local(), false);

	if (update_child)
		ui->update_transform();
}

void rhr::render::interfaces::i_ui::post_transform_update()
{

}
