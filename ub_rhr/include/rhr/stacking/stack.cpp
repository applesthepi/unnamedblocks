#include "stack.hpp"

rhr::stack::stack::stack(glm::vec<2, i32>* plane_offset)
	: m_function_collection_update(nullptr)
	, m_plane_offset(plane_offset)
{
	m_function_stack_update = [&]()
	{
		update_size();
		(*m_function_collection_update)();
	};

	m_blocks.reserve(10);
}

void rhr::stack::stack::set_plane_offset(glm::vec<2, i32>* plane_offset)
{
	m_plane_offset = plane_offset;

	for (auto& block : m_blocks)
		block->set_plane_offset(plane_offset);
}

glm::vec<2, i32>* rhr::stack::stack::get_plane_offset()
{
	return m_plane_offset;
}

void rhr::stack::stack::add_block(std::shared_ptr<rhr::stack::block> block)
{
	update_child_transform(block, 0x0);
	block->set_position_local_physical({0, rhr::stack::block::height * m_blocks.size()}, true);
	block->set_stack_update_function(&m_function_stack_update);
	m_blocks.push_back(block);

	update_size();
}

void rhr::stack::stack::add_blocks(const std::vector<std::shared_ptr<rhr::stack::block>>& blocks)
{
	if (m_blocks.size() + blocks.size() >= m_blocks.capacity())
		m_blocks.reserve((u64)std::ceil((f32)(m_blocks.size() + blocks.size()) * 1.5f + 10.0f));

	for (u64 i = 0; i < blocks.size(); i++)
	{
		update_child_transform(blocks[i], 0x0);
		blocks[i]->set_position_local_physical({0, rhr::stack::block::height * (m_blocks.size() + i)}, true);
		blocks[i]->set_stack_update_function(&m_function_stack_update);
		m_blocks.push_back(blocks[i]);
	}

	update_size();
}

void rhr::stack::stack::insert_blocks(const std::vector<std::shared_ptr<rhr::stack::block>>& blocks, u64 idx)
{
	for (usize i = 0; i < blocks.size(); i++)
	{
		update_child_transform(blocks[i], 0x0);
		blocks[i]->set_position_local_physical({0, rhr::stack::block::height * (idx + i)}, true);
		blocks[i]->set_stack_update_function(&m_function_stack_update);
		m_blocks.insert(m_blocks.begin() + idx + i, blocks[i]);
	}

	for (usize i = idx + blocks.size(); i < m_blocks.size(); i++)
	{
		update_child_transform(m_blocks[i], 0x0);
		m_blocks[i]->set_position_local_physical({0, rhr::stack::block::height * i}, true);
	}

	update_size();
}

u64 rhr::stack::stack::get_widest_block()
{
	u64 widest = 0;

	for (auto& m_block : m_blocks)
	{
		if (m_block->get_width() > widest)
			widest = m_block->get_width();
	}

	return widest;
}

void rhr::stack::stack::remove_block(u64 idx)
{
	m_blocks.erase(m_blocks.begin() + static_cast<i64>(idx));

	for (usize i = 0; i < m_blocks.size(); i++)
	{
		update_child_transform(m_blocks[i], 0x0);
		m_blocks[i]->set_position_local_physical({0, rhr::stack::block::height * i}, true);
	}

	update_size();
}

void rhr::stack::stack::remove_blocks_end(u64 offset)
{
	m_blocks.erase(m_blocks.begin() + static_cast<i64>(offset), m_blocks.end());

	for (usize i = 0; i < m_blocks.size(); i++)
	{
		update_child_transform(m_blocks[i], 0x0);
		m_blocks[i]->set_position_local_physical({0, rhr::stack::block::height * i}, true);
	}

	update_size();
}

void rhr::stack::stack::update_size() { set_size_local({get_widest_block(), m_blocks.size() * rhr::stack::block::height}, true); }

const std::vector<std::shared_ptr<rhr::stack::block>>& rhr::stack::stack::get_blocks() { return m_blocks; }

void rhr::stack::stack::ui_transform_update(i_ui::transform_update_spec transform_update_spec)
{
	for (usize i = 0; i < m_blocks.size(); i++)
	{
		update_child_transform(m_blocks[i], 0x0);
		m_blocks[i]->set_position_local_physical({0, rhr::stack::block::height * i}, true);
	}
}

void rhr::stack::stack::ui_render()
{
	for (auto& block : m_blocks)
		block->render();
}

void rhr::stack::stack::ui_reload_swap_chain()
{
	for (auto& block : m_blocks)
		block->reload_swap_chain();
}

void rhr::stack::stack::ui_update_buffers() {}

void rhr::stack::stack::ui_chain_update_buffers()
{
	for (auto& block : m_blocks)
		block->update_buffers();
}

void rhr::stack::stack::ui_frame_update(f64 delta_time)
{
	for (auto& block : m_blocks)
		block->frame_update(delta_time);
}

void rhr::stack::stack::set_collection_update_function(std::function<void()>* function_collection_update) { m_function_collection_update = function_collection_update; }
