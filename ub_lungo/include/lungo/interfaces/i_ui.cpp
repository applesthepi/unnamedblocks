#include "i_ui.hpp"

#include "lungo/renderer.hpp"

rhr::render::interfaces::i_ui::i_ui()
	: m_position_local_physical({})
	, m_position_local_virtual_offset({})
	, m_position_parent_physical({})
	, m_position_parent_virtual_offset({})
	, m_position_physical_absolute({})
	, m_position_virtual_offset({})
	, m_position_virtual_absolute({})
	, m_size_local({})
	, m_size_parent({})
	, m_enabled(true)
	, m_dirty(false)
	, m_initialized(false)
	, m_offset_cap(false)
{}

void rhr::render::interfaces::i_ui::initialize()
{
	if (m_initialized)
		return;

	m_initialized = true;
	ui_initialize();
}

void rhr::render::interfaces::i_ui::set_position_local_physical(const glm::vec<2, i32>& offset, bool update_child)
{
	m_position_local_physical	 = offset;
	m_position_physical_absolute = m_position_local_physical + m_position_parent_physical;
	m_position_virtual_absolute	 = m_position_virtual_offset + m_position_physical_absolute;

	if (update_child)
	{
#ifdef NDEBUG
		update_transform(i_ui::transform_update_spec_position);
#else
		if (m_initialized)
			update_transform(i_ui::transform_update_spec_position);
		else
			latte::logger::error(latte::logger::level::EDITOR, __FILE__, __LINE__, "can not update child transform without the child being initialized");
#endif
	}
}

void rhr::render::interfaces::i_ui::set_position_local_virtual_offset(const glm::vec<2, i32>& offset, bool update_child)
{
	m_position_local_virtual_offset = offset;
	m_position_virtual_offset		= m_position_local_virtual_offset + m_position_parent_virtual_offset;
	m_position_virtual_absolute		= m_position_virtual_offset + m_position_physical_absolute;

	if (update_child)
	{
#ifdef NDEBUG
		update_transform(i_ui::transform_update_spec_position);
#else
		if (m_initialized)
			update_transform(i_ui::transform_update_spec_position);
		else
			latte::logger::error(latte::logger::level::EDITOR, __FILE__, __LINE__, "can not update child transform without the child being initialized");
#endif
	}
}

void rhr::render::interfaces::i_ui::set_position_parent_physical(const glm::vec<2, i32>& offset, bool update_child)
{
	m_position_parent_physical	 = offset;
	m_position_physical_absolute = m_position_local_physical + m_position_parent_physical;
	m_position_virtual_absolute	 = m_position_virtual_offset + m_position_physical_absolute;

	if (update_child)
	{
#ifdef NDEBUG
		update_transform(i_ui::transform_update_spec_position);
#else
		if (m_initialized)
			update_transform(i_ui::transform_update_spec_position);
		else
			latte::logger::error(latte::logger::level::EDITOR, __FILE__, __LINE__, "can not update child transform without the child being initialized");
#endif
	}
}

void rhr::render::interfaces::i_ui::set_position_parent_virtual_offset(
	const glm::vec<2, i32>& offset, bool update_child)
{
	m_position_parent_virtual_offset = offset;
	m_position_virtual_offset		 = m_position_local_virtual_offset + m_position_parent_virtual_offset;
	m_position_virtual_absolute		 = m_position_virtual_offset + m_position_physical_absolute;

	if (update_child)
	{
#ifdef NDEBUG
		update_transform(i_ui::transform_update_spec_position);
#else
		if (m_initialized)
			update_transform(i_ui::transform_update_spec_position);
		else
			latte::logger::error(latte::logger::level::EDITOR, __FILE__, __LINE__, "can not update child transform without the child being initialized");
#endif
	}
}

void rhr::render::interfaces::i_ui::set_size_local(const glm::vec<2, i32>& size, bool update_child)
{
	m_size_local = size;

	if (update_child)
	{
#ifdef NDEBUG
		update_transform(i_ui::transform_update_spec_size);
#else
		if (m_initialized)
			update_transform(i_ui::transform_update_spec_size);
		else
			latte::logger::error(latte::logger::level::EDITOR, __FILE__, __LINE__, "can not update child transform without the child being initialized");
#endif
	}
}

void rhr::render::interfaces::i_ui::set_size_parent(const glm::vec<2, i32>& size, bool update_child)
{
	m_size_parent = size;

	if (update_child)
	{
#ifdef NDEBUG
		update_transform(i_ui::transform_update_spec_size);
#else
		if (m_initialized)
			update_transform(i_ui::transform_update_spec_size);
		else
			latte::logger::error(latte::logger::level::EDITOR, __FILE__, __LINE__, "can not update child transform without the child being initialized");
#endif
	}
}

void rhr::render::interfaces::i_ui::set_size_max(bool update_child)
{
	m_size_local = m_size_parent;

	if (update_child)
	{
#ifdef NDEBUG
		update_transform(i_ui::transform_update_spec_size);
#else
		if (m_initialized)
			update_transform(i_ui::transform_update_spec_size);
		else
			latte::logger::error(latte::logger::level::EDITOR, __FILE__, __LINE__, "can not update child transform without the child being initialized");
#endif
	}
}

const glm::vec<2, i32>& rhr::render::interfaces::i_ui::get_position_local_physical()
{
#ifdef NDEBUG
	return m_position_local_physical;
#else
	if (!m_initialized)
		latte::logger::error(latte::logger::level::EDITOR, __FILE__, __LINE__, "can not get transform information without the child being initialized");

	return m_position_local_physical;
#endif
}

const glm::vec<2, i32>& rhr::render::interfaces::i_ui::get_position_local_virtual_offset()
{
#ifdef NDEBUG
	return m_position_local_virtual_offset;
#else
	if (!m_initialized)
		latte::logger::error(latte::logger::level::EDITOR, __FILE__, __LINE__, "can not get transform information without the child being initialized");

	return m_position_local_virtual_offset;
#endif
}

const glm::vec<2, i32>& rhr::render::interfaces::i_ui::get_position_parent_physical()
{
#ifdef NDEBUG
	return m_position_parent_physical;
#else
	if (!m_initialized)
		latte::logger::error(latte::logger::level::EDITOR, __FILE__, __LINE__, "can not get transform information without the child being initialized");

	return m_position_parent_physical;
#endif
}

const glm::vec<2, i32>& rhr::render::interfaces::i_ui::get_position_parent_virtual_offset()
{
#ifdef NDEBUG
	return m_position_parent_virtual_offset;
#else
	if (!m_initialized)
		latte::logger::error(latte::logger::level::EDITOR, __FILE__, __LINE__, "can not get transform information without the child being initialized");

	return m_position_parent_virtual_offset;
#endif
}

const glm::vec<2, i32>& rhr::render::interfaces::i_ui::get_position_physical_absolute()
{
#ifdef NDEBUG
	return m_position_physical_absolute;
#else
	if (!m_initialized)
		latte::logger::fatal(latte::logger::level::EDITOR, __FILE__, __LINE__, "can not get transform information without the child being initialized");

	return m_position_physical_absolute;
#endif
}

const glm::vec<2, i32>& rhr::render::interfaces::i_ui::get_position_virtual_offset()
{
#ifdef NDEBUG
	return m_position_virtual_offset;
#else
	if (!m_initialized)
		latte::logger::error(latte::logger::level::EDITOR, __FILE__, __LINE__, "can not get transform information without the child being initialized");

	return m_position_virtual_offset;
#endif
}

const glm::vec<2, i32>& rhr::render::interfaces::i_ui::get_position_virtual_absolute()
{
#ifdef NDEBUG
	return m_position_virtual_absolute;
#else
	if (!m_initialized)
		latte::logger::error(latte::logger::level::EDITOR, __FILE__, __LINE__, "can not get transform information without the child being initialized");

	return m_position_virtual_absolute;
#endif
}

const glm::vec<2, i32>& rhr::render::interfaces::i_ui::get_size_local()
{
#ifdef NDEBUG
	return m_size_local;
#else
	if (!m_initialized)
		latte::logger::error(latte::logger::level::EDITOR, __FILE__, __LINE__, "can not get transform information without the child being initialized");

	return m_size_local;
#endif
}

const glm::vec<2, i32>& rhr::render::interfaces::i_ui::get_size_parent()
{
#ifdef NDEBUG
	return m_size_parent;
#else
	if (!m_initialized)
		latte::logger::error(latte::logger::level::EDITOR, __FILE__, __LINE__, "can not get transform information without the child being initialized");

	return m_size_parent;
#endif
}

void rhr::render::interfaces::i_ui::update_transform(
	rhr::render::interfaces::i_ui::transform_update_spec transform_update_spec)
{
	// This is the only place where it is ok to update the transform of the child before initialization.
	ui_transform_update(transform_update_spec);
}

void rhr::render::interfaces::i_ui::update_child_transform(
	const std::shared_ptr<rhr::render::interfaces::i_ui>& ui, i_ui::transform_update_spec transform_update_spec)
{
	ui->set_position_parent_physical(get_position_physical_absolute(), false);
	ui->set_position_parent_virtual_offset(get_position_virtual_offset(), false);
	ui->set_size_parent(get_size_local(), false);

	// TODO: for now it says here but move to an initialize style function that adds or creates the child.
	ui->set_static_offset(get_static_offset());

	// Update transform is called on the child here after the parent information is given to the child. This all happens
	// before initialization. Once the initializer runs this on the parent with the child, it will run initialize next. That
	// will unlock the access to the child's transform because now it has the information required from the parent to have
	// an accurate transform.
	if (transform_update_spec)
		ui->update_transform(transform_update_spec);
}

void rhr::render::interfaces::i_ui::update_child_transform(
	rhr::render::interfaces::i_ui* ui, rhr::render::interfaces::i_ui::transform_update_spec transform_update_spec
)
{
	ui->set_position_parent_physical(get_position_physical_absolute(), false);
	ui->set_position_parent_virtual_offset(get_position_virtual_offset(), false);
	ui->set_size_parent(get_size_local(), false);

	// Update transform is called on the child here after the parent information is given to the child. This all happens
	// before initialization. Once the initializer runs this on the parent with the child, it will run initialize next. That
	// will unlock the access to the child's transform because now it has the information required from the parent to have
	// an accurate transform.
	if (transform_update_spec)
		ui->update_transform(transform_update_spec);
}

void rhr::render::interfaces::i_ui::update_child_transform(
	const std::shared_ptr<rhr::render::interfaces::i_ui>& ui_parent,
	const std::shared_ptr<rhr::render::interfaces::i_ui>& ui_child,
	rhr::render::interfaces::i_ui::transform_update_spec transform_update_spec
)
{
	ui_child->set_position_parent_physical(ui_parent->get_position_physical_absolute(), false);
	ui_child->set_position_parent_virtual_offset(ui_parent->get_position_virtual_offset(), false);
	ui_child->set_size_parent(ui_parent->get_size_local(), false);

	// Update transform is called on the child here after the parent information is given to the child. This all happens
	// before initialization. Once the initializer runs this on the parent with the child, it will run initialize next. That
	// will unlock the access to the child's transform because now it has the information required from the parent to have
	// an accurate transform.
	if (transform_update_spec)
		ui_child->update_transform(transform_update_spec);
}

void rhr::render::interfaces::i_ui::set_enabled(bool enabled)
{
	m_enabled = enabled;
}

bool rhr::render::interfaces::i_ui::get_enabled()
{
	return m_enabled;
}

void rhr::render::interfaces::i_ui::serialize(latte::serializer::node& node)
{
#ifdef NDEBUG
	ui_serialize(node);
#else
	if (m_initialized)
		ui_serialize(node);
	else
		latte::logger::error(latte::logger::level::EDITOR, __FILE__, __LINE__, "can not serialize without the child being initialized");
#endif
}

void rhr::render::interfaces::i_ui::deserialize(latte::serializer::node& node)
{
#ifdef NDEBUG
	ui_deserialize(node);
#else
	if (m_initialized)
		ui_deserialize(node);
	else
		latte::logger::error(latte::logger::level::EDITOR, __FILE__, __LINE__, "can not deserialize without the child being initialized");
#endif
}

void rhr::render::interfaces::i_ui::frame_update(f64 delta_time)
{
#ifdef NDEBUG
	if (m_enabled)
		ui_frame_update(delta_time);
#else
	if (m_enabled)
	{
		if (m_initialized)
			ui_frame_update(delta_time);
		else
			latte::logger::error(latte::logger::level::EDITOR, __FILE__, __LINE__, "can not call frame_update without the child being initialized");
	}
#endif
}

void rhr::render::interfaces::i_ui::render()
{
#ifdef NDEBUG
	if (m_enabled)
		ui_render();
#else
	if (m_enabled)
	{
		if (m_initialized)
			ui_render();
		else
			latte::logger::error(latte::logger::level::EDITOR, __FILE__, __LINE__, "can not call render without the child being initialized");
	}
#endif
}

void rhr::render::interfaces::i_ui::reload_swap_chain()
{
#ifdef NDEBUG
	ui_reload_swap_chain();
#else
	if (m_initialized)
		ui_reload_swap_chain();
	else
		latte::logger::error(latte::logger::level::EDITOR, __FILE__, __LINE__, "can not call reload_swap_chain without the child being initialized");
#endif
}

void rhr::render::interfaces::i_ui::update_buffers()
{
#ifdef NDEBUG
	if (m_dirty)
	{
		m_dirty = false;
		ui_update_buffers();
	}

	ui_chain_update_buffers();
#else
	if (m_initialized)
	{
		if (m_dirty)
		{
			m_dirty = false;
			ui_update_buffers();
		}

		ui_chain_update_buffers();
	}
	else
		latte::logger::error(latte::logger::level::EDITOR, __FILE__, __LINE__, "can not call update_buffers without the child being initialized");
#endif
}

void rhr::render::interfaces::i_ui::set_static_offset(glm::vec<2, i32>* offset)
{
	m_offset = offset;
	ui_static_offset_update();
}

glm::vec<2, i32>* rhr::render::interfaces::i_ui::get_static_offset()
{
	return m_offset;
}

void rhr::render::interfaces::i_ui::mark_dirty()
{
	m_dirty = true;
}

void rhr::render::interfaces::i_ui::cap_offset()
{
	if (m_offset_cap)
		return;

	m_offset_cap = true;
	m_offset = new glm::vec<2, i32>();
	*m_offset = { 0, 0 };
}

rhr::render::interfaces::i_ui::transform_update_spec rhr::render::interfaces::i_ui::transform_update_spec_position =
	0x1;

rhr::render::interfaces::i_ui::transform_update_spec rhr::render::interfaces::i_ui::transform_update_spec_size = 0x2;
