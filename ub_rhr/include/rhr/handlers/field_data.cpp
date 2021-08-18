#include "field_data.hpp"

rhr::handler::field_data::location::location(glm::vec<2, usize> position, usize idx, u8 layer)
	: m_position(position)
	, m_idx(idx)
	, m_layer(layer)
	, m_null(false)
{

}

rhr::handler::field_data::location::location(glm::vec<2, usize> position, usize idx, u8 layer, bool null)
	: m_position(position)
	, m_idx(idx)
	, m_layer(layer)
	, m_null(null)
{

}

glm::vec<2, usize> rhr::handler::field_data::location::get_cell()
{
	return m_position;
}

usize rhr::handler::field_data::location::get_idx()
{
	return m_idx;
}

u8 rhr::handler::field_data::location::get_layer()
{
	return m_layer;
}

rhr::handler::field_data::location rhr::handler::field_data::location::none = rhr::handler::field_data::location({ 0, 0 }, 0, 0, true);

rhr::handler::field_data::data::data(usize idx, glm::vec<2, i32> position, glm::vec<2, i32> size, std::weak_ptr<rhr::render::interfaces::i_field> text_field, location location)
	: m_idx(idx)
	, m_position(position)
	, m_size(size)
	, m_text_field(text_field)
	, m_location(location)
{

}

glm::vec<2, i32> rhr::handler::field_data::data::get_position()
{
	return m_position;
}

void rhr::handler::field_data::data::set_position(glm::vec<2, i32> position)
{
	m_position = position;
}

glm::vec<2, i32> rhr::handler::field_data::data::get_size()
{
	return m_size;
}

void rhr::handler::field_data::data::set_size(glm::vec<2, i32> size)
{
	m_size = size;
}

usize rhr::handler::field_data::data::get_idx()
{
	return m_idx;
}

std::weak_ptr<rhr::render::interfaces::i_field>& rhr::handler::field_data::data::get_text_field()
{
	return m_text_field;
}

void rhr::handler::field_data::data::set_location(location location)
{
	m_location = location;
}

rhr::handler::field_data::location rhr::handler::field_data::data::get_location()
{
	return m_location;
}
