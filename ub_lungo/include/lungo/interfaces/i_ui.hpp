#pragma once
#include "config.h"

#include "latte/serializer.hpp"

#include <latte/utils.hpp>

namespace rhr::render::interfaces
{
///
class i_ui
{
public:
	i_ui();
	typedef u8 transform_update_spec;

	/// Many i_ui objects are appended to a parent after the child's constructor is called. This can lead to issues
	/// with the child depending on the parent's transform in the constructor. This should be called by the parent
	/// after it is added as a child. If the element isn't a child, the initializer should call this after the constructor.
	void initialize();

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

	/// This parent updates a child based on this transform data. Possible buffer update, use flags to make sure you
	/// need to call this.
	/// \param ui Child element to update from parent this.
	/// \param transform_update_spec Specification of what parts of the transform is being updated. Use 0x0 to only
	/// 							 update the transform without updating the child's buffers.
	void update_child_transform(
		rhr::render::interfaces::i_ui* ui, i_ui::transform_update_spec transform_update_spec);

	/// This parent updates a child based on this transform data. Possible buffer update, use flags to make sure you
	/// need to call this.
	/// \param ui_parent Parent element to update the child element from.
	/// \param ui_child Child element that will be updated form the parent element.
	/// \param transform_update_spec Specification of what parts of the transform is being updated. Use 0x0 to only
	/// 							 update the transform without updating the child's buffers.
	static void update_child_transform(
		const std::shared_ptr<rhr::render::interfaces::i_ui>& ui_parent,
		const std::shared_ptr<rhr::render::interfaces::i_ui>& ui_child,
		i_ui::transform_update_spec transform_update_spec);

	///
	void set_enabled(bool enabled);

	///
	bool get_enabled();

	///
	void serialize(latte::serializer::node& node);

	///
	void deserialize(latte::serializer::node& node);

	///
	void frame_update(f64 delta_time);

	///
	void render();

	///
	void reload_swap_chain();

	///
	void update_buffers();

	///
	void set_static_offset(glm::vec<2, i32>* offset);

	///
	void mark_dirty();
protected:
	///
	glm::vec<2, i32>* get_static_offset();

	/// Many i_ui objects are appended to a parent after the child's constructor is called. This can lead to issues
	/// with the child depending on the parent's transform in the constructor. This should be called by the parent
	/// after it is added as a child. If the element isn't a child, the initializer should call this after the constructor.
	virtual void ui_initialize() = 0;

	/// Called after any set transform related functions get called during frame update.
	virtual void ui_transform_update(i_ui::transform_update_spec transform_update_spec) = 0;

	///
	virtual void ui_frame_update(f64 delta_time) = 0;

	///
	virtual void ui_render() = 0;

	///
	virtual void ui_reload_swap_chain() = 0;

	///
	virtual void ui_update_buffers() = 0;

	///
	virtual void ui_chain_update_buffers() = 0;

	///
	virtual void ui_static_offset_update() = 0;

	///
	virtual void ui_serialize(latte::serializer::node& node) = 0;

	///
	virtual void ui_deserialize(latte::serializer::node& node) = 0;

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
	bool m_initialized;

	///
	std::string m_serialized_data;

	///
	glm::vec<2, i32>* m_offset;
};
} // namespace rhr::render::interfaces