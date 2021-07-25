#pragma once
#include "config.h"

#include "rhr/stacking/collection.hpp"
#include "rhr/rendering/interfaces/i_positionable.hpp"
#include "rhr/rendering/interfaces/i_updateable.hpp"
#include "rhr/rendering/interfaces/i_renderable.hpp"
#include "rhr/rendering/interfaces/i_sizeable.hpp"
#include "rhr/rendering/objects/rectangle.hpp"
#include "rhr/rendering/objects/text.hpp"

#include <cappuccino/utils.hpp>
#include <cappuccino/intrinsics.hpp>
#include <espresso/input_handler.hpp>

#define SNAP_DISTANCE 30.0f
#define SNAP_GRAPHIC_HEIGHT 2.0f

namespace rhr::stack
{
///
class plane : public rhr::render::interfaces::i_positionable<2, i32>, public rhr::render::interfaces::i_sizeable<2, i32>, public rhr::render::interfaces::i_renderable, public rhr::render::interfaces::i_updateable
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
	void delete_contents();

	///
	void mouse_button(glm::vec<2, i32> position, f32 scroll, MouseOperation operation);

	///
	static std::shared_ptr<rhr::stack::plane> primary_plane;

	///
	static std::shared_ptr<rhr::stack::plane> toolbar_plane;
	
	///
	void frame_update(f64 delta_time) override;
private:
	///
	void on_render() override;

	///
	void on_update_buffers() override;

	///
	void on_reload_swap_chain() override;

	///
	void post_position_update() override;

	///
	void post_size_update() override;

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
	std::shared_ptr<rhr::stack::collection> m_selected_collection;

	///
	std::shared_ptr<rhr::stack::stack> m_selected_stack;

	///
	std::shared_ptr<rhr::stack::block> m_selected_block;
	
	///
	std::shared_ptr<rhr::stack::argument::argument> m_selected_argument;

	///
	std::function<void(u8)> m_context_callback;

	///
	void drag_collection(std::shared_ptr<rhr::stack::collection>& collection, bool up);

	///
	void drag_stack(std::shared_ptr<rhr::stack::collection>& collection, std::shared_ptr<rhr::stack::stack>& stack, bool up);

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
	void set_snap(std::shared_ptr<rhr::stack::collection> collection, u64 stackLoc, std::shared_ptr<rhr::stack::stack> stack);

	///
	void clear_snap();

	///
	bool is_snap();
	
	///
	rhr::render::object::rectangle m_dragging_shape;

	///
	std::shared_ptr<rhr::stack::collection> m_dragging_snap_collection;

	///
	u64 m_dragging_snap_stack_loc;

	///
	std::shared_ptr<rhr::stack::stack> m_dragging_snap_stack;

	///
	bool m_dragging_snap;

	///
	std::vector<bool> m_collection_vanity;

	///
	std::vector<std::shared_ptr<rhr::stack::collection>> m_collections;

	///
	bool m_toolbar;

	///
	rhr::render::object::text m_inner_text;

	///
	std::shared_ptr<rhr::render::object::rectangle> m_background;
};
}
