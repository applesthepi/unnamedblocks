#pragma once
#include "config.h"

#include "mocha/stacking/arguments/argument.hpp"

#include <latte/utils.hpp>

#include <espresso/block.hpp>
#include <espresso/category.hpp>

#include <macchiato/interfaces/i_ui.hpp>
#include <macchiato/window.hpp>
#include <macchiato/shapes/rectangle.hpp>
#include <macchiato/entities/shape_entity.hpp>

#define BLOCK_SERIALIZE { "un" }

namespace mocha
{
///
class block : public mac::i_ui
{
public:
	///
	block(mac::window::state* window_state, const std::string& unlocalized_name);

	///
	std::vector<mocha::argument*>& get_arguments();

	///
	u32 get_width();

	///
	esp::block* get_esp_block();

	///
	esp::category* get_esp_category();

	///
	//const esp::mod::category* get_mod_category();

	///
	bool drag_bounds(glm::vec<2, i32> position);

	///
	void update_width();

	///
	void set_stack_update_function(std::function<void()>* function_stack_update);

private:
	void ui_initialize(mac::window::state* window_state) override;
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
	void update_arguments();

	///
	static void pad_arguments(
		u32& width,
		usize i,
		mocha::argument* last_arg,
		mocha::argument* arg,
		bool last);

	///
	u32 m_width;

	///
	std::vector<mocha::argument> m_arguments;

	///
	esp::block* m_esp_block;

	///
	esp::category* m_esp_category;

	///
	mac::shape_entity m_background_entity;
	mac::shape_rectangle m_background_shape;

	///
	std::function<void()>* m_function_stack_update;

	///
	std::function<void()> m_function_block_update;

	///
	std::string m_data_compact_cache;
};
} // namespace rhr::stack