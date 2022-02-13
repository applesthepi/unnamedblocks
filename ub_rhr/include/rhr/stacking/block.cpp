#include "block.hpp"

#include "rhr/registries/block.hpp"
#include "rhr/rendering/renderer.hpp"
#include "rhr/stacking/arguments/any.hpp"
#include "rhr/stacking/arguments/boolean.hpp"
#include "rhr/stacking/arguments/real.hpp"
#include "rhr/stacking/arguments/string.hpp"
#include "rhr/stacking/arguments/text.hpp"

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

rhr::stack::block::block(const std::string& unlocalized_name, glm::vec<2, i32>* plane_offset)
	: m_mod_block(rhr::registry::block::get_registry().get_block(unlocalized_name)->block_mod_block)
	, m_background(std::make_shared<rhr::render::object::rectangle>())
	, m_function_stack_update(nullptr)
	, m_plane_offset(plane_offset)
{
	m_function_block_update = [&]()
	{
		update_width();
		(*m_function_stack_update)();
	};

	m_mod_category =
		rhr::registry::block::get_registry().get_categories(m_mod_block->get_category())->category_mod_category;
	m_background->set_offset(plane_offset);
	m_background->set_color(m_mod_category->get_color() /*cap::color().from_u8({ 200, 200, 200, 0 })*/);
	m_background->set_depth(rhr::render::renderer::depth_block);

	set_size_local({100, rhr::stack::block::height}, true);
	update_arguments();
}

void rhr::stack::block::set_plane_offset(glm::vec<2, i32>* plane_offset)
{
	m_plane_offset = plane_offset;

	m_background->set_offset(plane_offset);

	for (auto& argument : m_arguments)
		argument->set_plane_offset(plane_offset);
}

glm::vec<2, i32>* rhr::stack::block::get_plane_offset()
{
	return m_plane_offset;
}

const std::string& rhr::stack::block::get_data()
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
		cap::logger::error(
			cap::logger::level::EDITOR, __FILE__, __LINE__, "parsing error on data provided to the block.");
		return;
	}

	if (!document.IsObject())
	{
		cap::logger::error(
			cap::logger::level::EDITOR,
			__FILE__,
			__LINE__,
			"data provided to the block is not an object that can be parsed.");
		return;
	}

	if (!document.HasMember("args"))
	{
		cap::logger::error(cap::logger::level::EDITOR, __FILE__, __LINE__, BLOCK_SET_DATA_ERROR_MISSING("args"));
		return;
	}

	rapidjson::Value& v_args = document["args"];

	if (!v_args.IsArray())
	{
		cap::logger::error(cap::logger::level::EDITOR, __FILE__, __LINE__, BLOCK_SET_DATA_ERROR_TYPE("args"));
		return;
	}

	if (v_args.Size() != m_arguments.size())
	{
		cap::logger::error(
			cap::logger::level::EDITOR,
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
			cap::logger::error(cap::logger::level::EDITOR, __FILE__, __LINE__, BLOCK_SET_DATA_ERROR_TYPE("args"));
			return;
		}

		const char* local_arg		 = v_arg.GetString();
		std::string string_local_arg = std::move(std::string(local_arg));
		m_arguments[static_cast<usize>(i)]->set_data_compact(string_local_arg);
	}

	update_width();
}

const std::vector<std::shared_ptr<rhr::stack::argument::argument>>& rhr::stack::block::get_arguments()
{
	return m_arguments;
}

u32 rhr::stack::block::get_width()
{
	return m_width;
}

const cap::mod::block::block* rhr::stack::block::get_mod_block()
{
	return m_mod_block;
}

const esp::mod::category* rhr::stack::block::get_mod_category()
{
	return m_mod_category;
}
i16 rhr::stack::block::padding = 2;
i16 rhr::stack::block::height  = 20;

i16 rhr::stack::block::height_content = height - (padding * 2);

void rhr::stack::block::ui_transform_update(i_ui::transform_update_spec transform_update_spec)
{
	update_child_transform(m_background, true);

	for (auto& arg : m_arguments)
		update_child_transform(arg, true);
}

void rhr::stack::block::ui_frame_update(f64 delta_time)
{
	for (auto& arg : m_arguments)
		arg->frame_update(delta_time);
}

void rhr::stack::block::ui_render()
{
	m_background->render();

	for (auto& arg : m_arguments)
		arg->render();
}

void rhr::stack::block::ui_reload_swap_chain()
{
	m_background->reload_swap_chain();

	for (auto& arg : m_arguments)
		arg->reload_swap_chain();
}

void rhr::stack::block::ui_update_buffers()
{}

void rhr::stack::block::ui_chain_update_buffers()
{
	m_background->update_buffers();

	for (auto& arg : m_arguments)
		arg->update_buffers();
}

void rhr::stack::block::ui_serialize(rhr::handler::serializer::node& node)
{
	node.data_names.reserve(1);
	node.data_values.reserve(1);
	node.children.reserve(m_arguments.size());

	// Block does not need the unlocalized name, the stack uses it to spawn it.
	node.data_names.emplace_back("un");
	node.data_values.emplace_back(m_mod_block->get_unlocalized_name());

	for (auto& argument : m_arguments)
	{
		auto& child_node = node.children.emplace_back();
		argument->serialize(child_node);
	}
}

void rhr::stack::block::ui_deserialize(rhr::handler::serializer::node& node)
{
	if (!node.verify_children(m_arguments.size()))
	{
		cap::logger::error(cap::logger::level::EDITOR, __FILE__, __LINE__, "failed to deserialize block; invalid argument count");
		return;
	}

	for (usize i = 0; i < m_arguments.size(); i++)
		m_arguments[i]->deserialize(node.children[i]);

	update_width();
}

void rhr::stack::block::update_arguments()
{
	m_arguments.clear();
	m_arguments.reserve(m_mod_block->get_arguments().size());

	std::vector<cap::mod::block::block::argument::initializer> argument_init = m_mod_block->get_arguments();

	u32 width = 0;
	cap::color arg_color =
		cap::color().from_normalized(m_mod_category->get_color().get_normalized_scaled(0.25f, false));

	std::shared_ptr<rhr::stack::argument::argument> last_arg;

	for (usize i = 0; i < argument_init.size(); i++)
	{
		if (argument_init[i].get_type() == cap::mod::block::block::argument::type::TEXT)
		{
			std::shared_ptr<rhr::stack::argument::text> arg =
				std::make_shared<rhr::stack::argument::text>(arg_color, &m_function_block_update, m_plane_offset);

			m_arguments.push_back(arg);
			last_arg = arg;

			pad_arguments(width, i, last_arg, arg);

			update_child_transform(arg, i_ui::transform_update_spec_position | i_ui::transform_update_spec_size);
			arg->set_position_local_physical({width, rhr::stack::block::padding}, true);
			arg->set_data(argument_init[i].get_default_value());
			arg->set_mode(argument_init[i].get_mode());
			arg->set_mode_restriction(argument_init[i].get_restriction());
		}
		else if (argument_init[i].get_type() == cap::mod::block::block::argument::type::REAL)
		{
			std::shared_ptr<rhr::stack::argument::real> arg =
				std::make_shared<rhr::stack::argument::real>(arg_color, &m_function_block_update, m_plane_offset);

			m_arguments.push_back(arg);
			last_arg = arg;

			pad_arguments(width, i, last_arg, arg);

			update_child_transform(arg, i_ui::transform_update_spec_position | i_ui::transform_update_spec_size);
			arg->set_position_local_physical({width, rhr::stack::block::padding}, true);
			arg->set_data(argument_init[i].get_default_value());
			arg->set_mode(argument_init[i].get_mode());
			arg->set_mode_restriction(argument_init[i].get_restriction());
		}
		else if (argument_init[i].get_type() == cap::mod::block::block::argument::type::STRING)
		{
			std::shared_ptr<rhr::stack::argument::string> arg =
				std::make_shared<rhr::stack::argument::string>(arg_color, &m_function_block_update, m_plane_offset);

			m_arguments.push_back(arg);
			last_arg = arg;

			pad_arguments(width, i, last_arg, arg);

			update_child_transform(arg, i_ui::transform_update_spec_position | i_ui::transform_update_spec_size);
			arg->set_position_local_physical({width, rhr::stack::block::padding}, true);
			arg->set_data(argument_init[i].get_default_value());
			arg->set_mode(argument_init[i].get_mode());
			arg->set_mode_restriction(argument_init[i].get_restriction());
		}
		else if (argument_init[i].get_type() == cap::mod::block::block::argument::type::BOOL)
		{
			std::shared_ptr<rhr::stack::argument::boolean> arg =
				std::make_shared<rhr::stack::argument::boolean>(arg_color, &m_function_block_update, m_plane_offset);

			m_arguments.push_back(arg);

			pad_arguments(width, i, last_arg, arg);
			last_arg = arg;

			update_child_transform(arg, i_ui::transform_update_spec_position | i_ui::transform_update_spec_size);
			arg->set_position_local_physical({width, rhr::stack::block::padding}, true);
			arg->set_data(argument_init[i].get_default_value());
			arg->set_mode(argument_init[i].get_mode());
			arg->set_mode_restriction(argument_init[i].get_restriction());
		}
	}

	update_width();
}

void rhr::stack::block::update_width()
{
	m_width = 0;
	std::shared_ptr<argument::argument> last_arg;

	for (usize i = 0; i < m_arguments.size(); i++)
	{
		std::shared_ptr<argument::argument>& arg = m_arguments[i];

		pad_arguments(m_width, i, last_arg, arg);
		last_arg = arg;

		arg->set_position_local_physical({m_width, rhr::stack::block::padding}, true);
		m_width += m_arguments[i]->get_width();
	}

	pad_arguments(m_width, 0, last_arg, last_arg, true);

	set_size_local(glm::vec<2, i32>(m_width, get_size_local().y), true);
	m_background->set_size_local(get_size_local(), true);
}

bool rhr::stack::block::drag_bounds(glm::vec<2, i32> position)
{
	for (auto arg : m_arguments)
	{
		if (arg->drag_bounds(position))
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
	const std::shared_ptr<rhr::stack::argument::argument>& last_arg,
	const std::shared_ptr<rhr::stack::argument::argument>& arg,
	bool last)
{
	static const u32 full_padding = rhr::stack::argument::argument::padding;
	static const u32 no_padding	  = rhr::stack::block::padding;

	if (last)
	{
		switch (arg->get_padding_style())
		{
		case rhr::stack::argument::argument::padding_style::HARD:
		{
			width += no_padding;
			break;
		}
		case rhr::stack::argument::argument::padding_style::SOFT:
		{
			width += full_padding;
			break;
		}
		case rhr::stack::argument::argument::padding_style::NONE:
		{
			width += no_padding;
			break;
		}
		}

		return;
	}

	switch (arg->get_padding_style())
	{
	case rhr::stack::argument::argument::padding_style::HARD:
	{
		if (i == 0)
			width += no_padding;
		else
			width += full_padding;

		break;
	}
	case rhr::stack::argument::argument::padding_style::SOFT:
	{
		if (i == 0)
			width += full_padding;
		else
		{
			switch (last_arg->get_padding_style())
			{
			case rhr::stack::argument::argument::padding_style::HARD:
			{
				width += full_padding;
				break;
			}
			case rhr::stack::argument::argument::padding_style::SOFT:
			{
				width += full_padding;
				break;
			}
			case rhr::stack::argument::argument::padding_style::NONE:
			{
				width += no_padding;
				break;
			}
			}
		}

		break;
	}
	case rhr::stack::argument::argument::padding_style::NONE:
	{
		if (i == 0)
			width += no_padding;
		else
		{
			switch (last_arg->get_padding_style())
			{
			case rhr::stack::argument::argument::padding_style::HARD:
			{
				width += full_padding;
				break;
			}
			case rhr::stack::argument::argument::padding_style::SOFT:
			{
				width += no_padding;
				break;
			}
			case rhr::stack::argument::argument::padding_style::NONE:
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
