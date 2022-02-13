#pragma once
#include "config.h"

#include "rhr/handlers/serializer.hpp"

#include <cappuccino/utils.hpp>

namespace rhr::render::interfaces
{
///
class i_ui
{
public:
	i_ui();
	typedef u8 transform_update_spec;

	/// Possible buffer update, use flags to make sure you need to call this.
	/// \param offset This object's physical position (render space). See (TODO: link) for more clarification.
	/// \param update_child Notifies this of a transform update using i_ui::transform_update_spec_position
	void set_position_local_physical(const glm::vec<2, i32>& offset, bool update_child);

	/// Possible buffer update, use flags to make sure you need to call this.
	/// \param offset This object's virtual position (screen space). See (TODO: link) for more clarification.
	/// \param update_child Notifies this of a transform update using i_ui::transform_update_spec_position
	void set_position_local_virtual_offset(const glm::vec<2, i32>& offset, bool update_child);

	/// Possible buffer update, use flags to make sure you need to call this.
	/// \param offset This object's parent's physical position (render space). See (TODO: link) for more clarification.
	/// \param update_child Notifies this of a transform update using i_ui::transform_update_spec_position
	void set_position_parent_physical(const glm::vec<2, i32>& offset, bool update_child);

	/// Possible buffer update, use flags to make sure you need to call this.
	/// \param offset This object's parent's virtual position (screen space). See (TODO: link) for more clarification.
	/// \param update_child Notifies this of a transform update using i_ui::transform_update_spec_position
	void set_position_parent_virtual_offset(const glm::vec<2, i32>& offset, bool update_child);

	/// Possible buffer update, use flags to make sure you need to call this.
	/// \param size This object's size.
	/// \param update_child Notifies this of a transform update using i_ui::transform_update_spec_size
	void set_size_local(const glm::vec<2, i32>& size, bool update_child);

	/// Possible buffer update, use flags to make sure you need to call this.
	/// \param size This object's parent's size.
	/// \param update_child Notifies this of a transform update using i_ui::transform_update_spec_size
	void set_size_parent(const glm::vec<2, i32>& size, bool update_child);

	/// Maxes out the local size to the parent's size. Possible buffer update, use flags to make sure you need to call
	/// this. \param update_child Notifies this of a transform update using i_ui::transform_update_spec_size
	void set_size_max(bool update_child);

	/// Retrieves this physical position (render space) relative to parent. See (TODO: link) for more clarification.
	const glm::vec<2, i32>& get_position_local_physical();

	/// Retrieves this virtual position (screen space) relative to parent. See (TODO: link) for more clarification.
	const glm::vec<2, i32>& get_position_local_virtual_offset();

	/// Retrieves this parent's physical position (render space) relative to { 0, 0 }. See (TODO: link) for more
	/// clarification.
	const glm::vec<2, i32>& get_position_parent_physical();

	/// Retrieves this parent's virtual position (screen space) relative to { 0, 0 }. See (TODO: link) for more
	/// clarification.
	const glm::vec<2, i32>& get_position_parent_virtual_offset();

	/// Retrieves this physical position (render space) relative to { 0, 0 }. See (TODO: link) for more clarification.
	const glm::vec<2, i32>& get_position_physical_absolute();

	/// Retrieves this virtual position (screen space) relative to { 0, 0 }. See (TODO: link) for more clarification.
	const glm::vec<2, i32>& get_position_virtual_offset();

	/// Retrieves this physical position (render space) relative to { 0, 0 } combined with the local virtual position
	/// relative to parent. See (TODO: link) for more clarification.
	const glm::vec<2, i32>& get_position_virtual_absolute();

	/// Retrieves this size.
	const glm::vec<2, i32>& get_size_local();

	/// Retrieves this parent's size.
	const glm::vec<2, i32>& get_size_parent();

	/// Will invoke a buffer update, use flags to make sure you need to call this.
	void update_transform(i_ui::transform_update_spec transform_update_spec);

	/// This parent updates a child based on this transform data. Possible buffer update, use flags to make sure you
	/// need to call this.
	/// \param ui Child element to update from parent this.
	/// \param transform_update_spec Specification of what parts of the transform is being updated. Use 0x0 to only
	/// 							 update the transform without updating the child's buffers.
	void update_child_transform(
		const std::shared_ptr<rhr::render::interfaces::i_ui>& ui, i_ui::transform_update_spec transform_update_spec);

	///
	void set_enabled(bool enabled);

	///
	bool get_enabled();

	///
	void serialize(rhr::handler::serializer::node& node);

	///
	void deserialize(rhr::handler::serializer::node& node);

	///
	void frame_update(f64 delta_time);

	///
	void render();

	///
	void reload_swap_chain();

	///
	void update_buffers();

protected:
	///
	void mark_dirty();

	/// Called after any set transform related functions get called during frame update.
	virtual void ui_transform_update(i_ui::transform_update_spec transform_update_spec);

	///
	virtual void ui_frame_update(f64 delta_time);

	///
	virtual void ui_render();

	///
	virtual void ui_reload_swap_chain();

	///
	virtual void ui_update_buffers();

	///
	virtual void ui_chain_update_buffers();

	///
	virtual void ui_serialize(rhr::handler::serializer::node& node);

	///
	virtual void ui_deserialize(rhr::handler::serializer::node& node);

	///
	static transform_update_spec transform_update_spec_position;

	///
	static transform_update_spec transform_update_spec_size;

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

	///
	bool m_enabled;

	///
	bool m_dirty;

	///
	std::string m_serialized_data;
};
} // namespace rhr::render::interfaces