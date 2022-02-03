#pragma once
#include "config.h"

#include <cappuccino/utils.hpp>

namespace rhr::render::interfaces
{
///
class i_ui
{
public:
	///
	typedef u8 transform_update_spec;

	///
	i_ui();

	///
	void set_weak(std::weak_ptr<i_ui> weak);

	///
	void set_position_local_physical(const glm::vec<2, i32>& offset, bool update_child = true);

	///
	void set_position_local_virtual_offset(const glm::vec<2, i32>& offset, bool update_child = true);

	///
	void set_position_parent_physical(const glm::vec<2, i32>& offset, bool update_child = true);

	///
	void set_position_parent_virtual_offset(const glm::vec<2, i32>& offset, bool update_child = true);

	///
	void set_size_local(const glm::vec<2, i32>& size, bool update_child = true);

	///
	void set_size_parent(const glm::vec<2, i32>& size, bool update_child = true);

	///
	void set_size_max(bool update_child = true);

	///
	const glm::vec<2, i32>& get_position_local_physical();

	///
	const glm::vec<2, i32>& get_position_local_virtual_offset();

	///
	const glm::vec<2, i32>& get_position_parent_physical();

	///
	const glm::vec<2, i32>& get_position_parent_virtual_offset();

	///
	const glm::vec<2, i32>& get_position_physical_absolute();

	///
	const glm::vec<2, i32>& get_position_virtual_offset();

	///
	const glm::vec<2, i32>& get_position_virtual_absolute();

	///
	const glm::vec<2, i32>& get_size_local();

	///
	const glm::vec<2, i32>& get_size_parent();

	///
	void update_transform(i_ui::transform_update_spec transform_update_spec);

	///
	void update_child_transform(const std::shared_ptr<rhr::render::interfaces::i_ui>& ui, bool update_child = true);

	///
	void set_enabled(bool enabled);

	///
	bool get_enabled();

	///
	void render();

	///
	void reload_swap_chain();

	///
	void update_buffers();

	///
	void frame_update(f64 delta_time);

protected:
	///
	void mark_dirty();

	/// Event called after any position or size update functions get run.
	virtual void ui_transform_update(i_ui::transform_update_spec transform_update_spec);

	///
	virtual void ui_render();

	///
	virtual void ui_reload_swap_chain();

	///
	virtual void ui_update_buffers();

	///
	virtual void ui_chain_update_buffers();

	///
	virtual void ui_frame_update(f64 delta_time);

	///
	static transform_update_spec transform_update_spec_position;

	///
	static transform_update_spec transform_update_spec_size;

private:
	///
	bool is_weak();

	///
	glm::vec<2, i32> m_position_local_physical;

	///
	glm::vec<2, i32> m_position_local_virtual_offset;

	///
	glm::vec<2, i32> m_position_parent_physical;

	///
	glm::vec<2, i32> m_position_parent_virtual_offset;

	///
	glm::vec<2, i32> m_position_physical_absolute;

	///
	glm::vec<2, i32> m_position_virtual_offset;

	///
	glm::vec<2, i32> m_position_virtual_absolute;

	///
	glm::vec<2, i32> m_size_local;

	///
	glm::vec<2, i32> m_size_parent;

	///
	bool m_enabled;

	///
	bool m_dirty;

	///
	bool m_weak_set;

	///
	std::weak_ptr<i_ui> m_weak;
};
} // namespace rhr::render::interfaces