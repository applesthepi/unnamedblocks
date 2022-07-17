#include "block.hpp"

#include "espresso/block.hpp"
#include "espresso/registry.hpp"

#define BLOCK_SET_DATA_ERROR_MISSING(field)                \
	("failed to set data for block. " + std::string(field) \
	 + " does not exist in the data provided to the block. The information is unchanged for this block.")
#define BLOCK_SET_DATA_ERROR_TYPE(field)                   \
	("failed to set data for block. " + std::string(field) \
	 + " is not the expected type. The information is unchanged for this block.")

static void block_update(void* data)
{
	mocha::block* block = (mocha::block*)data;
	block->update_width();
}

mocha::block::block(mac::window::state* window_state, const std::string& unlocalized_name)
	: m_esp_block(esp::registry::get()->get_block(unlocalized_name.c_str()))
	, m_background_shape(window_state, { 0, 0 }, { 0, 0 }, { 0.2f, 0.0f, 0.0f, 1.0f})
	, m_background_entity(&m_background_shape)
	, m_function_stack_update(nullptr)
{
	m_function_block_update = [&]()
	{
		update_width();
		(*m_function_stack_update)();
	};

	m_esp_category = esp::registry::get()->get_category(m_esp_block->category.c_str());
}

std::vector<mocha::argument*>& mocha::block::get_arguments()
{
	return m_arguments;
}

u32 mocha::block::get_width()
{
	return m_width;
}

esp::block* mocha::block::get_esp_block()
{
	return m_esp_block;
}

esp::category* mocha::block::get_esp_category()
{
	return m_esp_category;
}

void mocha::block::ui_initialize(mac::window::state* window_state)
{
	update_child_transform(&m_background_entity, 0);
	m_background_entity.initialize(window_state);
	m_background_shape.color = m_esp_category->get_color().get_normalized() /*espresso::color().from_u8({ 200, 200, 200, 0 })*/;
	// m_background_shape (rhr::render::renderer::depth_block);

	set_size_local({ 100, BLOCK_HEIGHT }, true);
	update_arguments();
}

void mocha::block::ui_transform_update(i_ui::transform_update_spec transform_update_spec)
{
	update_child_transform(&m_background_entity, i_ui::transform_update_spec_position | i_ui::transform_update_spec_size);

	for (auto arg : m_arguments)
		update_child_transform(arg, i_ui::transform_update_spec_position);
}

void mocha::block::ui_frame_update(f64 delta_time)
{
	for (auto arg : m_arguments)
		arg->frame_update(delta_time);
}

void mocha::block::ui_render()
{
	// m_background->render();

	// for (auto arg : m_arguments)
		// arg->render();
}

void mocha::block::ui_reload_swap_chain()
{
	// m_background->reload_swap_chain();

	// for (auto& arg : m_arguments)
		// arg.reload_swap_chain();
}

void mocha::block::ui_update_buffers()
{}

void mocha::block::ui_chain_update_buffers()
{
	// m_background->update_buffers();

	// for (auto& arg : m_arguments)
		// arg.update_buffers();
}

void mocha::block::ui_static_offset_update()
{
	m_background_entity.set_static_offset(get_static_offset());

	for (auto argument : m_arguments)
		argument->set_static_offset(get_static_offset());
}

void mocha::block::ui_serialize(latte::serializer::node& node)
{
	node.data_names.reserve(1);
	node.data_values.reserve(1);
	node.children.reserve(m_arguments.size());

	// Block does not need the unlocalized name, the stack uses it to spawn it.
	node.data_names.emplace_back("un");
	node.data_values.emplace_back(m_esp_block->unlocalized_name);

	for (auto argument : m_arguments)
	{
		auto& child_node = node.children.emplace_back();
		argument->serialize(child_node);
	}
}

void mocha::block::ui_deserialize(latte::serializer::node& node)
{
	if (!node.verify_children(m_arguments.size()))
	{
		latte::logger::error(latte::logger::level::EDITOR, __FILE__, __LINE__, "failed to deserialize block; invalid argument count");
		return;
	}

	for (usize i = 0; i < m_arguments.size(); i++)
		m_arguments[i]->deserialize(node.children[i]);

	update_width();
}

void mocha::block::update_arguments()
{
	// auto argument_inits = m_esp_block->argument_types;
	espresso::color block_color = m_esp_category->get_color();

	m_arguments.clear();
	m_arguments.reserve(m_esp_block->argument_types.size());

	esp::rt_argument* last_arg = nullptr;
	u32 width = 0;

	for (usize i = 0; i < m_esp_block->argument_types.size(); i++)
	{
		auto& arg = m_arguments.emplace_back(
			block_color, &m_function_block_update, get_static_offset(),
			esp::registry::get()->get_rt_argument(m_esp_block->argument_types[i]),
			
		);

		// auto arg = m_arguments.emplace_back(esp::registry::get()->get_rt_argument(m_esp_block->argument_types[i]));

		pad_arguments(width, i, last_arg, &arg, i == m_esp_block->argument_types.size() - 1);
		last_arg = arg;

		update_child_transform(arg, 0);
		arg.initialize();
		arg.set_position_local_physical({ width, BLOCK_PADDING }, false);
		arg.update_transform(i_ui::transform_update_spec_position | i_ui::transform_update_spec_size);
	}

	update_width();
}

void mocha::block::update_width()
{
	m_width = 0;
	rhr::stack::argument::argument* last_arg;

	for (usize i = 0; i < m_arguments.size(); i++)
	{
		rhr::stack::argument::argument* arg = &(m_arguments[i]);

		pad_arguments(m_width, i, last_arg, arg, i == m_arguments.size() - 1);
		last_arg = arg;

		arg->set_position_local_physical({ m_width, BLOCK_PADDING }, true);
		m_width += m_arguments[i].get_width();
	}

	pad_arguments(m_width, 0, last_arg, last_arg, true);

	set_size_local(glm::vec<2, i32>(m_width, get_size_local().y), true);
	m_background_shape.size = get_size_local();
	// m_background->set_size_local(get_size_local(), true);
}

bool mocha::block::drag_bounds(glm::vec<2, i32> position)
{
	for (auto& arg : m_arguments)
	{
		auto arg_pos = arg.get_position_virtual_absolute();

		if (position.x > arg_pos.x && position.x < arg_pos.x + arg.get_width() &&
			position.y > arg_pos.y && position.y < arg_pos.y + BLOCK_HEIGHT_CONTENT)
			return arg.get_esp_argument()->capture_mouse();
	}

	return false;
}

void mocha::block::set_stack_update_function(std::function<void()>* function_stack_update)
{
	m_function_stack_update = function_stack_update;
}

void mocha::block::pad_arguments(
	u32& width,
	usize i,
	rhr::stack::argument::argument* last_arg,
	rhr::stack::argument::argument* arg,
	bool last)
{
	static const u32 full_padding = esp::argument::padding;
	static const u32 no_padding	  = BLOCK_PADDING;

	if (last)
	{
		switch (arg->get_padding_style())
		{
		case esp::argument::padding_style::HARD:
			width += no_padding;
			break;
		case esp::argument::padding_style::SOFT:
			width += full_padding;
			break;
		case esp::argument::padding_style::NONE:
			width += no_padding;
			break;
		}

		return;
	}

	switch (arg->get_padding_style())
	{
	case esp::argument::padding_style::HARD:
	{
		if (i == 0)
			width += no_padding;
		else
			width += full_padding;

		break;
	}
	case esp::argument::padding_style::SOFT:
	{
		if (i == 0)
			width += full_padding;
		else
		{
			switch (last_arg->get_padding_style())
			{
			case esp::argument::padding_style::HARD:
			{
				width += full_padding;
				break;
			}
			case esp::argument::padding_style::SOFT:
			{
				width += full_padding;
				break;
			}
			case esp::argument::padding_style::NONE:
			{
				width += no_padding;
				break;
			}
			}
		}

		break;
	}
	case esp::argument::padding_style::NONE:
	{
		if (i == 0)
			width += no_padding;
		else
		{
			switch (last_arg->get_padding_style())
			{
			case esp::argument::padding_style::HARD:
			{
				width += full_padding;
				break;
			}
			case esp::argument::padding_style::SOFT:
			{
				width += no_padding;
				break;
			}
			case esp::argument::padding_style::NONE:
			{
				width += no_padding;
				break;
			}
			}
		}

		break;
	}
	}
}
