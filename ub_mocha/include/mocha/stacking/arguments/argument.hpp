#pragma once
#include "config.h"

#include <latte/utils.hpp>

#include <espresso/color.hpp>
#include <espresso/mod/block/block.hpp>
#include <espresso/rt_argument.hpp>

#include <macchiato/interfaces/i_ui.hpp>

#define ARGUMENT_SERIALIZE { "mode" }

namespace mocha
{
/// Base class for visual block arguments.
class argument : public mac::i_ui
{
public:
	argument(
		const espresso::color& block_color,
		std::function<void()>* function_collection_update,
		glm::vec<2, i32>* plane_offset,
		esp::rt_argument* esp_argument,
		esp::rt_argument::initializer* esp_argument_initializer);
	argument(const argument& other);
	argument(const argument&& other);
	~argument();

	///
	u32 get_width();

	///
	esp::argument::padding_style get_padding_style();

	///
	esp::rt_argument* get_esp_argument();

	///
	esp::rt_argument::state* get_esp_argument_state();

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
	esp::rt_argument::state m_esp_argument_state;

	///
	esp::rt_argument* m_esp_argument;

	///
	esp::rt_argument::initializer* m_esp_argument_initializer;
};
} // namespace rhr::stack::argument
