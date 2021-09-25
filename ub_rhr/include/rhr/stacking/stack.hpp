#pragma once
#include "config.h"

#include "rhr/stacking/block.hpp"
#include "rhr/rendering/interfaces/i_positionable.hpp"
#include "rhr/rendering/interfaces/i_renderable.hpp"
#include "rhr/rendering/interfaces/i_updateable.hpp"
#include "rhr/rendering/interfaces/i_sizeable.hpp"
#include "rhr/rendering/interfaces/i_enableable.hpp"
#include "rhr/rendering/interfaces/i_ui.hpp"

#include <cappuccino/utils.hpp>

namespace rhr::stack
{
///
class stack :
	public rhr::render::interfaces::i_ui,
	public rhr::render::interfaces::i_renderable,
	public rhr::render::interfaces::i_updateable,
	public rhr::render::interfaces::i_enableable
{
public:
	///
	stack();

	///
	void add_block(std::shared_ptr<rhr::stack::block> block);

	///
	void add_blocks(const std::vector<std::shared_ptr<rhr::stack::block>>& blocks);

	///
	void insert_blocks(const std::vector<std::shared_ptr<rhr::stack::block>>& blocks, u64 idx);

	///
	u64 get_widest_block();

	///
	void remove_block(u64 idx);

	///
	void update_size();

	///
	const std::vector<std::shared_ptr<rhr::stack::block>>& get_blocks();

	///
	void frame_update(f64 delta_time) override;

	///
	void set_collection_update_function(std::function<void()>* function_collection_update);
private:
	///
	void on_render() override;
	
	///
	void on_update_buffers() override;

	///
	void on_reload_swap_chain() override;

	///
	void post_transform_update() override;

	///
	void post_enable_update(bool enabled) override;

	///
	std::vector<std::shared_ptr<rhr::stack::block>> m_blocks;

	///
	std::function<void()>* m_function_collection_update;

	///
	std::function<void()> m_function_stack_update;
};
}