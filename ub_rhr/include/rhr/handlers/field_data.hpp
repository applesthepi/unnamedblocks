#pragma once
#include "config.h"

#include "rhr/rendering/interfaces/i_field.hpp"

#include <cappuccino/utils.hpp>
#include <espresso/input_handler.hpp>

namespace rhr::handler::field_data
{
///
class location
{
public:
	///
	location(glm::vec<2, usize> position, usize idx, u8 layer);

	///
	location(glm::vec<2, usize> position, usize idx, u8 layer, bool null);

	///
	glm::vec<2, usize> get_cell();

	///
	usize get_idx();

	///
	u8 get_layer();

	///
	static location none;
private:
	///
	glm::vec<2, usize> m_position;

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
	data(usize idx, glm::vec<2, i32> position, glm::vec<2, i32> size, std::weak_ptr<rhr::render::interfaces::i_field> text_field, location location);

	///
	glm::vec<2, i32> get_position();

	///
	void set_position(glm::vec<2, i32> position);

	///
	glm::vec<2, i32> get_size();

	///
	void set_size(glm::vec<2, i32> size);

	///
	usize get_idx();

	///
	std::weak_ptr<rhr::render::interfaces::i_field>& get_text_field();

	///
	void set_location(location location);

	///
	location get_location();
private:
	///
	usize m_idx;

	///
	glm::vec<2, i32> m_position;

	///
	glm::vec<2, i32> m_size;

	///
	std::weak_ptr<rhr::render::interfaces::i_field> m_text_field;

	///
	location m_location;
};
}