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
	i_ui();

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
	void update_transform();

	///
	void update_child_transform(const std::shared_ptr<rhr::render::interfaces::i_ui>& ui, bool update_child = true);
protected:
	/// Event called after any position or size update functions get run.
	virtual void post_transform_update();
private:
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
};
}