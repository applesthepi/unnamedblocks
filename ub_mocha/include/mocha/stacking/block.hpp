#pragma once
#include "config.h"

// #include "lungo/interfaces/i_ui.hpp"
// #include "lungo/objects/rectangle.hpp"
#include "mocha/stacking/arguments/argument.hpp"

#include <espresso/block.hpp>
#include <espresso/category.hpp>
#include <latte/utils.hpp>

#define BLOCK_SERIALIZE { "un" }

namespace rhr::stack
{
///
class block : public rhr::render::interfaces::i_ui
{
public:
	///
	block(const std::string& unlocalized_name);

	///
	std::vector<rhr::stack::argument::argument>& get_arguments();

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
	void ui_initialize() override;
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
		rhr::stack::argument::argument* last_arg,
		rhr::stack::argument::argument* arg,
		bool last);

	///
	u32 m_width;

	///
	std::vector<rhr::stack::argument::argument> m_arguments;

	///
	esp::block* m_esp_block;

	///
	esp::category* m_esp_category;

	///
	std::shared_ptr<rhr::render::object::rectangle> m_background;

	///
	std::function<void()>* m_function_stack_update;

	///
	std::function<void()> m_function_block_update;

	///
	std::string m_data_compact_cache;
};
} // namespace rhr::stack