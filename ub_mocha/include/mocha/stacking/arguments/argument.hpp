#pragma once
#include "config.h"

#include "lungo/interfaces/i_ui.hpp"
#include "lungo/vertex.hpp"

#include <espresso/color.hpp>
#include <espresso/mod/block/block.hpp>
#include <espresso/argument.hpp>
#include <latte/utils.hpp>

#define ARGUMENT_SERIALIZE { "mode" }

namespace rhr::stack::argument
{
/// Base class for visual block arguments.
class argument : public rhr::render::interfaces::i_ui
{
public:
	argument(
		const espresso::color& block_color,
		std::function<void()>* function_collection_update,
		glm::vec<2, i32>* plane_offset,
		esp::argument::initializer* esp_argument_initializer);
	argument(const argument& other);
	argument(const argument&& other);
	~argument();

	///
	u32 get_width();

	///
	esp::argument::padding_style get_padding_style();

	///
	esp::argument* get_esp_argument();

	///
	esp::argument::state* get_esp_argument_state();

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
	esp::argument::state m_esp_argument_state;

	///
	esp::argument* m_esp_argument;

	///
	esp::argument::initializer* m_esp_argument_initializer;
};
} // namespace rhr::stack::argument
