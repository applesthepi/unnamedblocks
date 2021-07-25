#pragma once
#include "config.h"

#include "rhr/stacking/arguments/argument.hpp"
#include "rhr/rendering/interfaces/i_renderable.hpp"
#include "rhr/rendering/interfaces/i_updateable.hpp"
#include "rhr/rendering/interfaces/i_positionable.hpp"
#include "rhr/rendering/interfaces/i_sizeable.hpp"
#include "rhr/rendering/objects/rectangle.hpp"

#include <cappuccino/utils.hpp>
#include <cappuccino/mod_block/block.hpp>
#include <espresso/mod/category.hpp>

namespace rhr::stack
{
///
class block : public rhr::render::interfaces::i_renderable, public rhr::render::interfaces::i_updateable, public rhr::render::interfaces::i_positionable<2, i32>, public rhr::render::interfaces::i_sizeable<2, i32>
{
public:
	///
	block(const std::string& unlocalized_name);

	///
	const std::vector<std::shared_ptr<rhr::stack::argument::argument>>& get_arguments();

	///
	u32 get_width();

	///
	const ModBlock* get_mod_block();

	///
	const ModCatagory* get_mod_category();

	///
	void frame_update(f64 delta_time) override;

	///
	static i16 padding;

	///
	static i16 height;

	///
	static i16 height_content;
private:
	///
	void on_render() override;

	///
	void on_update_buffers() override;

	///
	void on_reload_swap_chain() override;

	///
	void post_position_update() override;

	///
	void update_arguments();

	///
	void update_width();

	///
	u32 m_width;

	///
	std::vector<std::shared_ptr<rhr::stack::argument::argument>> m_arguments;

	///
	const ModBlock* m_mod_block;

	///
	const ModCatagory* m_mod_category;

	///
	std::shared_ptr<rhr::render::object::rectangle> m_background;
};
}