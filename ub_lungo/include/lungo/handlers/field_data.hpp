#pragma once
#include "config.h"

#include "lungo/interfaces/i_field.hpp"

#include "lungo/handlers/input.hpp"
#include <latte/utils.hpp>

namespace rhr::handler::field_data
{
///
class location
{
public:
	///
	location(glm::vec<2, usize> cell, usize idx, u8 layer);

	///
	location(glm::vec<2, usize> cell, usize idx, u8 layer, bool null);

	///
	glm::vec<2, usize> get_cell() const;

	///
	usize get_idx() const;

	///
	u8 get_layer() const;

	///
	static location none;

private:
	///
	glm::vec<2, usize> m_cell;

	///
	usize m_idx;

	///
	u8 m_layer;

	///
	bool m_null;
};

///
class data
{
public:
	///
	data(
		usize idx,
		glm::vec<2, i32> v_position,
		glm::vec<2, i32> p_position,
		glm::vec<2, i32> size,
		rhr::render::interfaces::i_field* text_field,
		location location,
		bool is_host);

	///
	glm::vec<2, i32> get_position_virtual();

	///
	glm::vec<2, i32> get_position_physical();

	///
	void set_position_virtual(glm::vec<2, i32> v_position);

	///
	void set_position_physical(glm::vec<2, i32> p_position);

	///
	glm::vec<2, i32> get_size();

	///
	glm::vec<2, i32> get_host_size();

	///
	void copy_to_host();

	///
	void set_size(glm::vec<2, i32> size);

	///
	usize get_idx();

	///
	rhr::render::interfaces::i_field*& get_text_field();

	///
	void set_location(location location);

	///
	location get_location();

	///
	std::optional<std::vector<rhr::handler::field_data::location>*> host_get_locations();

	///
	void host_add_location(const rhr::handler::field_data::location& location);

private:
	///
	usize m_idx;

	///
	glm::vec<2, i32> m_v_position;

	///
	glm::vec<2, i32> m_p_position;

	///
	glm::vec<2, i32> m_size;

	///
	glm::vec<2, i32> m_host_size;

	///
	rhr::render::interfaces::i_field* m_text_field;

	///
	location m_location;

	///
	std::vector<rhr::handler::field_data::location>* m_other_locations;
};
} // namespace rhr::handler::field_data