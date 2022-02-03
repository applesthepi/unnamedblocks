#pragma once
#include "config.h"

#include "rhr/rendering/interfaces/i_ui.hpp"
#include "rhr/stacking/block.hpp"

#include <cappuccino/utils.hpp>

namespace rhr::stack
{
///
class stack : public rhr::render::interfaces::i_ui
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
	void remove_blocks_end(u64 offset);

	///
	void update_size();

	///
	const std::vector<std::shared_ptr<rhr::stack::block>>& get_blocks();

	///
	void set_collection_update_function(std::function<void()>* function_collection_update);

private:
	///
	void ui_transform_update(i_ui::transform_update_spec transform_update_spec) override;

	///
	void ui_render() override;

	///
	void ui_reload_swap_chain() override;

	///
	void ui_update_buffers() override;

	///
	void ui_chain_update_buffers() override;

	///
	void ui_frame_update(f64 delta_time) override;

	///
	std::vector<std::shared_ptr<rhr::stack::block>> m_blocks;

	///
	std::function<void()>* m_function_collection_update;

	///
	std::function<void()> m_function_stack_update;
};
} // namespace rhr::stack