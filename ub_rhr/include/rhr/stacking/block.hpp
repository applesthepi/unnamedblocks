#pragma once
#include "config.h"

#include "rhr/stacking/arguments/argument.hpp"
#include "rhr/rendering/interfaces/i_renderable.hpp"
#include "rhr/rendering/interfaces/i_updateable.hpp"
#include "rhr/rendering/interfaces/i_positionable.hpp"
#include "rhr/rendering/interfaces/i_sizeable.hpp"
#include "rhr/rendering/interfaces/i_enableable.hpp"
#include "rhr/rendering/objects/rectangle.hpp"

#include <cappuccino/utils.hpp>
#include <cappuccino/mod/block/block.hpp>
#include <espresso/mod/category.hpp>

namespace rhr::stack
{
///
class block : public rhr::render::interfaces::i_renderable, public rhr::render::interfaces::i_updateable, public rhr::render::interfaces::i_positionable<2, i32>, public rhr::render::interfaces::i_sizeable<2, i32>, public rhr::render::interfaces::i_enableable
{
public:
	///
	block(const std::string& unlocalized_name);

	///
	const std::vector<std::shared_ptr<rhr::stack::argument::argument>>& get_arguments();

	///
	u32 get_width();

	///
	const cap::mod::block::block* get_mod_block();

	///
	const esp::mod::category* get_mod_category();

	///
	void frame_update(f64 delta_time) override;

	///
	bool drag_bounds(glm::vec<2, i32> position);

	///
	void update_width();

	///
	void set_stack_update_function(std::function<void()>* function_stack_update);

	///
	static i16 padding;

	///
	static i16 height;

	///
	static i16 height_content;
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
	void post_enable_update(bool enabled) override;

	///
	void update_arguments();

	///
	static void pad_arguments(u32& width, usize i, const std::shared_ptr<rhr::stack::argument::argument>& last_arg, const std::shared_ptr<rhr::stack::argument::argument>& arg, bool last = false);

	///
	u32 m_width;

	///
	std::vector<std::shared_ptr<rhr::stack::argument::argument>> m_arguments;

	///
	const cap::mod::block::block* m_mod_block;

	///
	const esp::mod::category* m_mod_category;

	///
	std::shared_ptr<rhr::render::object::rectangle> m_background;

	///
	std::function<void()>* m_function_stack_update;

	///
	std::function<void()> m_function_block_update;
};
}