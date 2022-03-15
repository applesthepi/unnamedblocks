#include "argument.hpp"

#include "mocha/stacking/block.hpp"

rhr::stack::argument::argument::argument(
	const espresso::color& block_color, std::function<void()>* function_collection_update, glm::vec<2, i32>* plane_offset
	, esp::argument* esp_argument)
	: m_esp_argument(esp_argument)
{
	m_esp_argument_state.parent = static_cast<rhr::render::interfaces::i_ui*>(this);
	m_esp_argument_state.plane_offset = plane_offset;
	m_esp_argument_state.function_collection_update = function_collection_update;
	m_esp_argument_state.block_color = block_color;

	m_esp_argument->create(&m_esp_argument_state);
}

u32 rhr::stack::argument::argument::get_width()
{
	return m_esp_argument->get_width(&m_esp_argument_state);
}

esp::argument::padding_style rhr::stack::argument::argument::get_padding_style()
{
	return m_esp_argument->get_padding_style();
}

void rhr::stack::argument::argument::ui_serialize(latte::serializer::node& node)
{
	node.data_names.reserve(1);
	node.data_values.reserve(1);

	node.data_names.emplace_back("mode");

	switch (m_esp_argument_state.mode)
	{
	case esp::argument::mode::RAW:
		node.data_values.emplace_back("0");
		break;
	case esp::argument::mode::VAR:
		node.data_values.emplace_back("1");
		break;
	default:
		latte::logger::error(
			latte::logger::level::EDITOR,
			__FILE__,
			__LINE__,
			"unknown esp::argument::mode; using 0 instead");
		node.data_values.emplace_back("0");
		break;
	}

	auto& child_node = node.children.emplace_back();
	m_esp_argument->ui_serialize(&m_esp_argument_state, child_node);
}

void rhr::stack::argument::argument::ui_deserialize(latte::serializer::node& node)
{
	if (!node.verify_data(ARGUMENT_SERIALIZE) || !node.verify_children(1))
	{
		latte::logger::error(latte::logger::level::EDITOR, __FILE__, __LINE__, "failed to deserialize argument");
		return;
	}

	char variable_mode_char = node.data_values[0][0];

	switch (variable_mode_char)
	{
	case '0':
		m_esp_argument_state.mode = esp::argument::mode::RAW;
		break;
	case '1':
		m_esp_argument_state.mode = esp::argument::mode::VAR;
		break;
	default:
		latte::logger::error(
			latte::logger::level::EDITOR,
			__FILE__,
			__LINE__,
			"unknown index of espresso::mod::block::block::argument::variable_mode; using RAW instead");
		m_esp_argument_state.mode = esp::argument::mode::RAW;
		break;
	}

	m_esp_argument->ui_deserialize(&m_esp_argument_state, node.children[0]);
}

void rhr::stack::argument::argument::ui_transform_update(
	rhr::render::interfaces::i_ui::transform_update_spec transform_update_spec
)
{
	m_esp_argument->ui_transform_update(&m_esp_argument_state, transform_update_spec);
}

void rhr::stack::argument::argument::ui_frame_update(f64 delta_time)
{
	m_esp_argument->ui_frame_update(&m_esp_argument_state, delta_time);
}

void rhr::stack::argument::argument::ui_render()
{
	m_esp_argument->ui_render(&m_esp_argument_state);
}

void rhr::stack::argument::argument::ui_reload_swap_chain()
{
	m_esp_argument->ui_reload_swap_chain(&m_esp_argument_state);
}

void rhr::stack::argument::argument::ui_update_buffers()
{
	m_esp_argument->ui_update_buffers(&m_esp_argument_state);
}

void rhr::stack::argument::argument::ui_chain_update_buffers()
{
	m_esp_argument->ui_chain_update_buffers(&m_esp_argument_state);
}

void rhr::stack::argument::argument::ui_static_offset_update()
{
	m_esp_argument->ui_static_offset_update(&m_esp_argument_state);
}
