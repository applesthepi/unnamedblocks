#include "field_data.hpp"

rhr::handler::field_data::location::location(glm::vec<2, usize> cell, usize idx, u8 layer)
	: m_cell(cell)
	, m_idx(idx)
	, m_layer(layer)
	, m_null(false)
{}

rhr::handler::field_data::location::location(glm::vec<2, usize> cell, usize idx, u8 layer, bool null)
	: m_cell(cell)
	, m_idx(idx)
	, m_layer(layer)
	, m_null(null)
{}

glm::vec<2, usize> rhr::handler::field_data::location::get_cell() const { return m_cell; }

usize rhr::handler::field_data::location::get_idx() const { return m_idx; }

u8 rhr::handler::field_data::location::get_layer() const { return m_layer; }

rhr::handler::field_data::location rhr::handler::field_data::location::none = rhr::handler::field_data::location({0, 0}, 0, 0, true);

rhr::handler::field_data::data::data(
	usize idx, glm::vec<2, i32> position, glm::vec<2, i32> size, std::weak_ptr<rhr::render::interfaces::i_field>&& text_field, location location, bool is_host)
	: m_idx(idx)
	, m_position(position)
	, m_size(size)
	, m_host_size({0, 0})
	, m_text_field(text_field)
	, m_location(location)
{
	if (is_host)
		m_other_locations = new std::vector<rhr::handler::field_data::location>();
	else
		m_other_locations = nullptr;
}

glm::vec<2, i32> rhr::handler::field_data::data::get_position() { return m_position; }

void rhr::handler::field_data::data::set_position(glm::vec<2, i32> position) { m_position = position; }

glm::vec<2, i32> rhr::handler::field_data::data::get_size() { return m_size; }

void rhr::handler::field_data::data::set_size(glm::vec<2, i32> size) { m_size = size; }

usize rhr::handler::field_data::data::get_idx() { return m_idx; }

std::weak_ptr<rhr::render::interfaces::i_field>& rhr::handler::field_data::data::get_text_field() { return m_text_field; }

void rhr::handler::field_data::data::set_location(location location) { m_location = location; }

rhr::handler::field_data::location rhr::handler::field_data::data::get_location() { return m_location; }

std::optional<std::vector<rhr::handler::field_data::location>*> rhr::handler::field_data::data::host_get_locations()
{
	if (m_other_locations == nullptr)
		return std::nullopt;
	else
		return m_other_locations;
}

void rhr::handler::field_data::data::host_add_location(const rhr::handler::field_data::location& location)
{
	if (m_other_locations == nullptr)
	{
		cap::logger::warn(cap::logger::level::SYSTEM, "tried to add a location to a non-host location object");
		return;
	}

	m_other_locations->push_back(location);
}

glm::vec<2, i32> rhr::handler::field_data::data::get_host_size() { return m_host_size; }

void rhr::handler::field_data::data::copy_to_host() { m_host_size = m_size; }
