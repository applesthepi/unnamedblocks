#include "argument.hpp"

#include "rhr/stacking/block.hpp"

rhr::stack::argument::argument::argument(const cap::color& block_color, std::function<void()>* function_collection_update)
	: rhr::render::interfaces::i_enableable(true)
	, m_mode(cap::mod::block::block::argument::variable_mode::RAW)
	, m_block_color(block_color)
	, m_function_collection_update(function_collection_update)
	, m_mode_restriction(cap::mod::block::block::argument::variable_mode_restriction::NONE)
{
	
}

void rhr::stack::argument::argument::set_data(const std::string& data)
{
	m_data = data;
	on_set_data();
}

bool rhr::stack::argument::argument::set_mode(cap::mod::block::block::argument::variable_mode mode)
{
	if (m_mode_restriction == cap::mod::block::block::argument::variable_mode_restriction::NONE)
	{
		m_mode = mode;
		on_set_mode(mode);
		return true;
	}

	return false;
}

cap::mod::block::block::argument::variable_mode rhr::stack::argument::argument::get_mode()
{
	return m_mode;
}

const std::string& rhr::stack::argument::argument::get_data()
{
	return m_data;
}

cap::mod::block::block::argument::type rhr::stack::argument::argument::get_type()
{
	return cap::mod::block::block::argument::type::TEXT;
}

u32 rhr::stack::argument::argument::get_width()
{
	return 50;
}

bool rhr::stack::argument::argument::has_data()
{
	return false;
}

bool rhr::stack::argument::argument::drag_bounds(glm::vec<2, i32> position)
{
	return false;
}

rhr::stack::argument::argument::padding_style rhr::stack::argument::argument::get_padding_style()
{
	return rhr::stack::argument::argument::padding_style::HARD;
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

void rhr::stack::argument::argument::set_mode_restriction(cap::mod::block::block::argument::variable_mode_restriction mode_restriction)
{
	m_mode_restriction = mode_restriction;
}

void rhr::stack::argument::argument::on_set_mode(cap::mod::block::block::argument::variable_mode mode)
{

}