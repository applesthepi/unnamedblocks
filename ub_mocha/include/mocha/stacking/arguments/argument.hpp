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

	///
	argument(
		const espresso::color& block_color,
		std::function<void()>* function_collection_update,
		glm::vec<2, i32>* plane_offset,
		esp::argument* esp_argument);

	///
	u32 get_width();

	///
	esp::argument::padding_style get_padding_style();

private:

	void ui_transform_update(i_ui::transform_update_spec transform_update_spec);
	void ui_frame_update(f64 delta_time);
	void ui_render();
	void ui_reload_swap_chain();
	void ui_update_buffers();
	void ui_chain_update_buffers();
	void ui_static_offset_update();
	void ui_serialize(latte::serializer::node& node) override;
	void ui_deserialize(latte::serializer::node& node) override;

	///
	esp::argument::state m_esp_argument_state;

	///
	esp::argument* m_esp_argument;
};
} // namespace rhr::stack::argument
