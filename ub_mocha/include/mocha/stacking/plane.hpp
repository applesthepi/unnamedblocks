#pragma once
#include "config.h"

#include "lungo/handlers/field.hpp"
#include "lungo/handlers/input.hpp"
#include "lungo/interfaces/i_ui.hpp"
#include "lungo/objects/line.hpp"
#include "lungo/objects/rectangle.hpp"
#include "lungo/objects/text.hpp"
#include "mocha/stacking/collection.hpp"

#include <cappuccino/intrinsics.hpp>
#include <latte/utils.hpp>

#define SNAP_DISTANCE		30.0f
#define SNAP_GRAPHIC_HEIGHT 2.0f

namespace rhr::stack
{
///
class plane : public rhr::render::interfaces::i_ui
{
public:
	///
	plane(bool toolbar);

	/// Adds a collection to the plane. displayCollectionVanity should be true on PrimaryPlane and false on ToolbarPlane
	void add_collection(std::shared_ptr<rhr::stack::collection>& collection, bool displayCollectionVanity);

	///
	const std::vector<std::shared_ptr<rhr::stack::collection>>& get_collections();

	///
	void delete_collection(u64 idx);

	///
	bool is_toolbar();

	///
	void delete_contents(bool disable_collections = false);

	///
	void mouse_button(glm::vec<2, i32> position, f32 scroll, rhr::handler::input::mouse_operation operation, rhr::handler::input::mouse_button button);

	///
	static std::shared_ptr<rhr::stack::plane> primary_plane;

	///
	static std::shared_ptr<rhr::stack::plane> toolbar_plane;

	///
	rhr::handler::field& get_field();

	///
	void render_master_pass();

private:
	void ui_initialize() override;
	void ui_transform_update(i_ui::transform_update_spec transform_update_spec) override;
	void ui_frame_update(f64 delta_time) override;
	void ui_render() override;
	void ui_reload_swap_chain() override;
	void ui_update_buffers() override;
	void ui_chain_update_buffers() override;
	void ui_static_offset_update() override;
	void ui_serialize(latte::serializer::node& node) override;
	void ui_deserialize(latte::serializer::node& node) override;

	///
	void select(u64 collection, u64 stack, u64 block, u64 argument);

	///
	void select_context(u64 collection, u64 stack, u64 block);

	///
	void unselect();

	///
	bool m_selected;

	///
	bool m_selected_context;

	///
	std::weak_ptr<rhr::stack::collection> m_selected_collection;

	///
	std::weak_ptr<rhr::stack::stack> m_selected_stack;

	///
	std::weak_ptr<rhr::stack::block> m_selected_block;

	///
	rhr::stack::argument::argument* m_selected_argument;

	///
	std::function<void(u8)> m_context_callback;

	///
	void drag_collection(std::shared_ptr<rhr::stack::collection> collection, bool up);

	///
	void
	drag_stack(std::shared_ptr<rhr::stack::collection> collection, std::shared_ptr<rhr::stack::stack> stack, bool up);

	///
	void undrag(const glm::vec<2, i32>& position);

	///
	void dragging_stack_update();

	///
	bool dragging_collection();

	///
	bool dragging_stack();

	///
	std::shared_ptr<rhr::stack::collection> m_dragging_collection;

	///
	std::shared_ptr<rhr::stack::stack> m_dragging_stack;

	///
	glm::vec<2, i32> m_dragging_begin_object;

	///
	glm::vec<2, i32> m_dragging_begin_mouse;

	///
	bool m_dragging_up;

	///
	void
	set_snap(std::weak_ptr<rhr::stack::collection> collection, u64 stackLoc, std::weak_ptr<rhr::stack::stack> stack);

	///
	void clear_snap();

	///
	bool is_snap();

	///
	rhr::render::object::rectangle m_dragging_shape;

	///
	std::weak_ptr<rhr::stack::collection> m_dragging_snap_collection;

	///
	u64 m_dragging_snap_stack_loc;

	///
	std::weak_ptr<rhr::stack::stack> m_dragging_snap_stack;

	///
	bool m_dragging_snap;

	///
	std::vector<bool> m_collection_vanity;

	///
	std::vector<std::shared_ptr<rhr::stack::collection>> m_collections;

	///
	bool m_toolbar;

	///
	// rhr::render::object::text m_inner_text;

	///
	std::shared_ptr<rhr::render::object::rectangle> m_background;

	///
	std::shared_ptr<rhr::render::object::line> m_dragging_connecting_line;

	///
	rhr::handler::field m_field;

	///
	std::function<void(rhr::handler::input::mouse_button_data)> m_mouse_button;

	///
	u64 m_mouse_button_idx;
};
} // namespace rhr::stack
