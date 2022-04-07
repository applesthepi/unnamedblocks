#pragma once
#include "config.h"

#include "lungo/interfaces/i_ui.hpp"
#include "mocha/stacking/block.hpp"

#include <latte/utils.hpp>

#define STACK_SERIALIZE { "p_x", "p_y", "s_x", "s_y" }

namespace rhr::stack
{
///
class stack : public rhr::render::interfaces::i_ui
{
public:
	///
	stack(glm::vec<2, i32>* plane_offset);

	///
	void set_plane_offset(glm::vec<2, i32>* plane_offset);

	///
	glm::vec<2, i32>* get_plane_offset();

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
	void remove_all();

	///
	std::vector<std::shared_ptr<rhr::stack::block>> m_blocks;

	///
	std::function<void()>* m_function_collection_update;

	///
	std::function<void()> m_function_stack_update;

	///
	glm::vec<2, i32>* m_plane_offset;
};
} // namespace rhr::stack