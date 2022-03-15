#include "block.hpp"

#include "espresso/block.hpp"
#include "espresso/registry.hpp"
#include "lungo/renderer.hpp"

#define BLOCK_SET_DATA_ERROR_MISSING(field)                \
	("failed to set data for block. " + std::string(field) \
	 + " does not exist in the data provided to the block. The information is unchanged for this block.")
#define BLOCK_SET_DATA_ERROR_TYPE(field)                   \
	("failed to set data for block. " + std::string(field) \
	 + " is not the expected type. The information is unchanged for this block.")

static void block_update(void* data)
{
	rhr::stack::block* block = (rhr::stack::block*)data;
	block->update_width();
}

rhr::stack::block::block(const std::string& unlocalized_name)
	: m_esp_block(esp::registry::get_registry()->get_block(unlocalized_name.c_str()))
	, m_background(std::make_shared<rhr::render::object::rectangle>())
	, m_function_stack_update(nullptr)
{
	m_function_block_update = [&]()
	{
		update_width();
		(*m_function_stack_update)();
	};

	m_esp_category = esp::registry::get_registry()->get_category(m_esp_block->get_category());

	m_background->set_color(m_esp_category->get_color() /*espresso::color().from_u8({ 200, 200, 200, 0 })*/);
	m_background->set_depth(rhr::render::renderer::depth_block);

	set_size_local({100, BLOCK_HEIGHT}, true);
	update_arguments();
}

/*const std::string& rhr::stack::block::get_data()
{
	rapidjson::Document document;
	document.SetObject();

	rapidjson::Value v_args;
	v_args.SetArray();

	for (auto& arg : m_arguments)
	{
		const std::string& data_compact = arg->get_data_compact();

		rapidjson::Value v_arg;
		v_arg.SetString(data_compact.c_str(), data_compact.size());

		v_args.PushBack(v_arg, document.GetAllocator());
	}

	document.AddMember("args", v_args, document.GetAllocator());

	rapidjson::StringBuffer string_buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(string_buffer);
	document.Accept(writer);

	m_data_compact_cache = std::move(std::string(string_buffer.GetString()));
	return m_data_compact_cache;
}

void rhr::stack::block::set_data(const std::string& data)
{
	rapidjson::Document document;
	document.Parse(data.c_str(), data.size());

	if (document.HasParseError())
	{
		latte::logger::error(
			latte::logger::level::EDITOR, __FILE__, __LINE__, "parsing error on data provided to the block.");
		return;
	}

	if (!document.IsObject())
	{
		latte::logger::error(
			latte::logger::level::EDITOR,
			__FILE__,
			__LINE__,
			"data provided to the block is not an object that can be parsed.");
		return;
	}

	if (!document.HasMember("args"))
	{
		latte::logger::error(latte::logger::level::EDITOR, __FILE__, __LINE__, BLOCK_SET_DATA_ERROR_MISSING("args"));
		return;
	}

	rapidjson::Value& v_args = document["args"];

	if (!v_args.IsArray())
	{
		latte::logger::error(latte::logger::level::EDITOR, __FILE__, __LINE__, BLOCK_SET_DATA_ERROR_TYPE("args"));
		return;
	}

	if (v_args.Size() != m_arguments.size())
	{
		latte::logger::error(
			latte::logger::level::EDITOR,
			__FILE__,
			__LINE__,
			"data provided to the block has a different argument count.");
		return;
	}

	for (u16 i = 0; i < v_args.Size(); i++)
	{
		rapidjson::Value& v_arg = v_args[static_cast<usize>(i)];

		if (!v_arg.IsString())
		{
			latte::logger::error(latte::logger::level::EDITOR, __FILE__, __LINE__, BLOCK_SET_DATA_ERROR_TYPE("args"));
			return;
		}

		const char* local_arg		 = v_arg.GetString();
		std::string string_local_arg = std::move(std::string(local_arg));
		m_arguments[static_cast<usize>(i)]->set_data_compact(string_local_arg);
	}

	update_width();
}*/

std::vector<rhr::stack::argument::argument>& rhr::stack::block::get_arguments()
{
	return m_arguments;
}

u32 rhr::stack::block::get_width()
{
	return m_width;
}

esp::block* rhr::stack::block::get_esp_block()
{
	return m_esp_block;
}

esp::category* rhr::stack::block::get_esp_category()
{
	return m_esp_category;
}

//const esp::argument* rhr::stack::block::get_mod_block()
//{
//	return m_mod_block;
//}
//
//const esp::mod::category* rhr::stack::block::get_mod_category()
//{
//	return m_mod_category;
//}

void rhr::stack::block::ui_transform_update(i_ui::transform_update_spec transform_update_spec)
{
	update_child_transform(m_background, true);

	for (auto& arg : m_arguments)
		update_child_transform(&arg, true);
}

void rhr::stack::block::ui_frame_update(f64 delta_time)
{
	for (auto& arg : m_arguments)
		arg.frame_update(delta_time);
}

void rhr::stack::block::ui_render()
{
	m_background->render();

	for (auto& arg : m_arguments)
		arg.render();
}

void rhr::stack::block::ui_reload_swap_chain()
{
	m_background->reload_swap_chain();

	for (auto& arg : m_arguments)
		arg.reload_swap_chain();
}

void rhr::stack::block::ui_update_buffers()
{}

void rhr::stack::block::ui_chain_update_buffers()
{
	m_background->update_buffers();

	for (auto& arg : m_arguments)
		arg.update_buffers();
}

void rhr::stack::block::ui_static_offset_update()
{
	m_background->set_offset(get_static_offset());

	for (auto& argument : m_arguments)
		argument.set_static_offset(get_static_offset());
}

void rhr::stack::block::ui_serialize(latte::serializer::node& node)
{
	node.data_names.reserve(1);
	node.data_values.reserve(1);
	node.children.reserve(m_arguments.size());

	// Block does not need the unlocalized name, the stack uses it to spawn it.
	node.data_names.emplace_back("un");
	node.data_values.emplace_back(m_esp_block->get_unlocalized_name());

	for (auto& argument : m_arguments)
	{
		auto& child_node = node.children.emplace_back();
		argument.serialize(child_node);
	}
}

void rhr::stack::block::ui_deserialize(latte::serializer::node& node)
{
	if (!node.verify_children(m_arguments.size()))
	{
		latte::logger::error(latte::logger::level::EDITOR, __FILE__, __LINE__, "failed to deserialize block; invalid argument count");
		return;
	}

	for (usize i = 0; i < m_arguments.size(); i++)
		m_arguments[i].deserialize(node.children[i]);

	update_width();
}

void rhr::stack::block::update_arguments()
{
	auto argument_inits = m_esp_block->get_arguments();
	espresso::color block_color = m_esp_category->get_color();

	m_arguments.clear();
	m_arguments.reserve(argument_inits.size());

	rhr::stack::argument::argument* last_arg = nullptr;
	u32 width = 0;

	for (usize i = 0; i < argument_inits.size(); i++)
	{
		auto& arg = m_arguments.emplace_back(rhr::stack::argument::argument(block_color, &m_function_block_update, get_static_offset(), argument_inits[i].argument));

		pad_arguments(width, i, last_arg, &arg, i == argument_inits.size() - 1);
		last_arg = &arg;

		update_child_transform(&arg, i_ui::transform_update_spec_position | i_ui::transform_update_spec_size);
		arg.set_position_local_physical({ width, BLOCK_PADDING }, true);
	}

	update_width();
}

void rhr::stack::block::update_width()
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
	m_background->set_size_local(get_size_local(), true);
}

bool rhr::stack::block::drag_bounds(glm::vec<2, i32> position)
{
	for (auto& arg : m_arguments)
	{
		auto& arg_pos = arg.get_position_virtual_absolute();

		if (position.x > arg_pos.x && position.x < arg_pos.x + arg.get_width() &&
			position.y > arg_pos.y && position.y < arg_pos.y + BLOCK_HEIGHT_CONTENT)
			return true;
	}

	return false;
}

void rhr::stack::block::set_stack_update_function(std::function<void()>* function_stack_update)
{
	m_function_stack_update = function_stack_update;
}

void rhr::stack::block::pad_arguments(
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
