#include "argument.hpp"

#include "rhr/stacking/block.hpp"

rhr::stack::argument::argument::argument(const cap::color& block_color)
	: m_mode(BlockArgumentVariableMode::RAW)
	, m_block_color(block_color)
{
	
}

void rhr::stack::argument::argument::set_data(const std::string& data)
{
	m_data = data;
	on_set_data();
}

const std::string& rhr::stack::argument::argument::get_data()
{
	return m_data;
}

void rhr::stack::argument::argument::set_mode(BlockArgumentVariableMode mode)
{
	m_mode = mode;
}

BlockArgumentVariableMode rhr::stack::argument::argument::get_mode()
{
	return m_mode;
}

BlockArgumentType rhr::stack::argument::argument::get_type()
{
	return BlockArgumentType::TEXT;
}

u32 rhr::stack::argument::argument::get_width()
{
	return 50;
}

bool rhr::stack::argument::argument::has_data()
{
	return false;
}

void rhr::stack::argument::argument::select()
{

}

void rhr::stack::argument::argument::unselect()
{

}

bool rhr::stack::argument::argument::drag_bounds(glm::vec<2, i32> position)
{
	return false;
}

i32 rhr::stack::argument::argument::padding = rhr::stack::block::padding * 4;

void rhr::stack::argument::argument::on_render()
{

}

void rhr::stack::argument::argument::on_update_buffers()
{

}

void rhr::stack::argument::argument::on_reload_swap_chain()
{

}

void rhr::stack::argument::argument::on_frame_update(f64 delta_time)
{

}

void rhr::stack::argument::argument::on_set_data()
{

}