#pragma once
#include "config.h"

#include "rhr/rendering/cardinal.hpp"
#include "rhr/rendering/interfaces/i_renderable.hpp"
#include "rhr/rendering/interfaces/i_updateable.hpp"
#include "rhr/rendering/interfaces/i_positionable.hpp"
#include "rhr/rendering/interfaces/i_sizeable.hpp"
#include "rhr/rendering/objects/rectangle.hpp"

#include <cappuccino/utils.hpp>
#include <espresso/input_handler.hpp>

namespace rhr::render
{
///
class frame : public rhr::render::interfaces::i_renderable, public rhr::render::interfaces::i_updateable, public rhr::render::interfaces::i_positionable<2, i32>, public rhr::render::interfaces::i_sizeable<2, i32>
{
public:
	///
	frame();

	///
	void set_padding(u8 padding);

	///
	void set_frame(std::shared_ptr<frame>& frame);

	///
	usize add_frame(std::shared_ptr<frame>& frame, rhr::render::cardinal::local cardinal);

	///
	void enable_frame(usize idx, bool enabled);

	///
	void add_content(std::weak_ptr<rhr::render::interfaces::i_renderable>&& renderable,
					 std::weak_ptr<rhr::render::interfaces::i_updateable>&& updatable,
					 std::weak_ptr<rhr::render::interfaces::i_positionable<2, i32>>&& positionable,
					 std::weak_ptr<rhr::render::interfaces::i_sizeable<2, i32>>&& sizeable,
					 std::weak_ptr<rhr::render::interfaces::i_enableable>&& enableable,
					 rhr::render::cardinal::local cardinal);

	///
	void mouse_button(glm::vec<2, i32> position, f32 scroll, MouseOperation operation);

	///
	void link(rhr::render::cardinal::local cardinal);

	///
	void unlink(rhr::render::cardinal::local cardinal);

	///
	void update_links();

	///
	void set_bar(usize idx, i32 offset);

	///
	void enable_background(const cap::color& color);

	///
	void disable_bar_movement();

	///
	void update_content_dimentions();
protected:
	///
	void on_render() override;

	///
	void on_update_buffers() override;

	///
	void on_reload_swap_chain() override;

	///
	void on_set_weak() override;

	///
	void post_position_update() override;

	///
	void post_size_update() override;
private:
	///
	struct content
	{
		/// Weak pointers to the content's interfaces. Used to call common functions for the content.
		std::weak_ptr<rhr::render::interfaces::i_renderable> renderable;
		std::weak_ptr<rhr::render::interfaces::i_updateable> updateable;
		std::weak_ptr<rhr::render::interfaces::i_positionable<2, i32>> positionable;
		std::weak_ptr<rhr::render::interfaces::i_sizeable<2, i32>> sizeable;
		std::weak_ptr<rhr::render::interfaces::i_enableable> enableable;
	};

	///
	struct bar
	{
		///
		bar(i32 offset)
			: offset(offset)
		{

		}
		
		///
		i32 offset;

		///
		glm::vec<2, i32> absolute_position;

		///
		glm::vec<2, i32> size;
	};

	///
	void submit_new_bar_position(rhr::render::cardinal::local cardinal);

	///
	void equalize_bars(bool size_to_content);

	///
	void push_links(std::shared_ptr<frame>& frame);

	///
	void update_mouse_button_status(bool enabled);

	///
	void update_bars_from_relative();

	///
	void update_bars_from_absolute();

	///
	void reset_drag();

	///
	void update_background();

	///
	bool m_has_frame;

	///
	bool m_has_space;

	///
	bool m_has_content;

	///
	bool m_link_up, m_link_down, m_link_left, m_link_right;

	///
	bool m_mouse_button_registered;

	///
	bool m_dragging;

	///
	usize m_dragging_bar_idx;

	///
	glm::vec<2, i32> m_dragging_mouse_begin;

	///
	glm::vec<2, i32> m_dragging_object_begin;

	///
	rhr::render::cardinal::plane m_plane;

	///
	std::vector<std::shared_ptr<frame>> m_frames;

	///
	std::vector<bool> m_frames_enabled;

	///
	std::vector<content> m_content;

	///
	std::vector<bar> m_bars;

	///
	u8 m_padding;

	///
	bool m_background_enabled;

	///
	bool m_disable_bar_movement;

	///
	std::shared_ptr<rhr::render::object::rectangle> m_background;
};
}