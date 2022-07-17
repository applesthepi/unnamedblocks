#pragma once
#include "config.h"

#include "mocha/stacking/collection.hpp"

#include <latte/utils.hpp>

#include <macchiato/interfaces/i_ui.hpp>
#include <macchiato/shapes/rectangle.hpp>
#include <macchiato/entities/shape_entity.hpp>
#include <macchiato/window.hpp>

#define SNAP_DISTANCE		30.0f
#define SNAP_GRAPHIC_HEIGHT 2.0f

namespace rhr::stack
{
///
class plane : public mac::i_ui
{
public:
	///
	plane(mac::window::state* window_state, bool toolbar);

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
	mac::input::capture mouse_button(const mac::input::mouse_state& mouse_state);

	///
	static std::shared_ptr<rhr::stack::plane> primary_plane;

	///
	static std::shared_ptr<rhr::stack::plane> toolbar_plane;

	///
	// rhr::handler::field& get_field();

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
	// mac::shape_rectangle m_dragging_shape;

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
	mac::shape_entity m_background_entity;
	mac::shape_rectangle m_background_shape;

	///
	// std::shared_ptr<rhr::render::object::line> m_dragging_connecting_line;

	///
	// rhr::handler::field m_field;

	///
	// std::function<void(rhr::handler::input::mouse_button_data)> m_mouse_button;

	///
	u64 m_mouse_button_idx;
};
} // namespace rhr::stack
