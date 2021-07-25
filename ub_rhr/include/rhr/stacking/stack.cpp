#include "stack.hpp"

rhr::stack::stack::stack()
{
	m_blocks.reserve(10);
}

void rhr::stack::stack::add_block(std::shared_ptr<rhr::stack::block> block)
{
	block->set_position({ 0, rhr::stack::block::height * m_blocks.size() });
	block->set_super_position(m_position + m_super_position);
	m_blocks.push_back(block);

	update_size();
}

void rhr::stack::stack::add_blocks(const std::vector<std::shared_ptr<rhr::stack::block>>& blocks)
{
	if (m_blocks.size() + blocks.size() >= m_blocks.capacity())
		m_blocks.reserve((u64)std::ceil((f32)(m_blocks.size() + blocks.size()) * 1.5f + 10.0f));

	for (u64 i = 0; i < blocks.size(); i++)
	{
		blocks[i]->set_position({ 0, rhr::stack::block::height * (m_blocks.size() + i) });
		blocks[i]->set_super_position(m_position + m_super_position);
		m_blocks.push_back(blocks[i]);
	}

	update_size();
}

void rhr::stack::stack::insert_blocks(const std::vector<std::shared_ptr<rhr::stack::block>>& blocks, u64 idx)
{
	for (usize i = 0; i < blocks.size(); i++)
	{
		blocks[i]->set_position({ 0, rhr::stack::block::height * (idx + i) });
		blocks[i]->set_super_position(m_position + m_super_position);
		m_blocks.insert(m_blocks.begin() + idx + i, blocks[i]);
	}

	for (usize i = idx + blocks.size(); i < m_blocks.size(); i++)
	{
		m_blocks[i]->set_position({ 0, rhr::stack::block::height * i });
		m_blocks[i]->set_super_position(m_position + m_super_position);
	}

	update_size();
}

u64 rhr::stack::stack::get_widest_block()
{
	u64 widest = 0;

	for (u64 i = 0; i < m_blocks.size(); i++)
	{
		if (m_blocks[i]->get_width() > widest)
			widest = m_blocks[i]->get_width();
	}

	return widest;
}

void rhr::stack::stack::remove_block(u64 idx)
{
	m_blocks.erase(m_blocks.begin() + idx);

	for (usize i = 0; i < m_blocks.size(); i++)
	{
		m_blocks[i]->set_position({ 0, rhr::stack::block::height * i });
		m_blocks[i]->set_super_position(m_position + m_super_position);
	}

	update_size();
}

void rhr::stack::stack::update_size()
{
	set_size({ get_widest_block(), m_blocks.size() * rhr::stack::block::height });
}

const std::vector<std::shared_ptr<rhr::stack::block>>& rhr::stack::stack::get_blocks()
{
	return m_blocks;
}

void rhr::stack::stack::frame_update(f64 delta_time)
{
	for (auto& block : m_blocks)
		block->frame_update(delta_time);
}

void rhr::stack::stack::on_render()
{
	for (auto& block : m_blocks)
		block->render();
}

void rhr::stack::stack::on_update_buffers()
{
	for (auto& block : m_blocks)
		block->update_buffers();
}

void rhr::stack::stack::on_reload_swap_chain()
{
	for (auto& block : m_blocks)
		block->reload_swap_chain();
}

void rhr::stack::stack::post_position_update()
{
	for (usize i = 0; i < m_blocks.size(); i++)
	{
		m_blocks[i]->set_position({ 0, rhr::stack::block::height * i });
		m_blocks[i]->set_super_position(m_position + m_super_position);
	}
}
