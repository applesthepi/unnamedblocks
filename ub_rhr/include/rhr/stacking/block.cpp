#include "block.hpp"

#include "rhr/stacking/arguments/text.hpp"
#include "rhr/stacking/arguments/real.hpp"
#include "rhr/stacking/arguments/boolean.hpp"
#include "rhr/stacking/arguments/string.hpp"
#include "rhr/stacking/arguments/any.hpp"
#include "rhr/registries/block.hpp"
#include "rhr/rendering/renderer.hpp"

static void block_update(void* data)
{
	rhr::stack::block* block = (rhr::stack::block*)data;
	block->update_width();
}

rhr::stack::block::block(const std::string& unlocalized_name)
	: m_mod_block(rhr::registry::block::get_registry().get_block(unlocalized_name)->block_mod_block)
	, m_background(std::make_shared<rhr::render::object::rectangle>())
	, m_function_stack_update(nullptr)
{
	m_function_block_update = [&]()
	{
		update_width();
		(*m_function_stack_update)();
	};

	m_size = { 100, rhr::stack::block::height };
	m_mod_category = rhr::registry::block::get_registry().get_categories(m_mod_block->get_category())->category_mod_category;
	m_background->set_weak(m_background);
	m_background->set_color(m_mod_category->get_color());
	m_background->set_depth(rhr::render::renderer::depth_block);

	update_arguments();
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
i16 rhr::stack::block::height = 20;
i16 rhr::stack::block::height_content = height - (padding * 2);

void rhr::stack::block::frame_update(f64 delta_time)
{
	for (auto& arg : m_arguments)
		arg->frame_update(delta_time);
}

void rhr::stack::block::on_render()
{
	m_background->render();

	for (auto& arg : m_arguments)
		arg->render();
}

void rhr::stack::block::on_update_buffers()
{
	m_background->update_buffers();

	for (auto& arg : m_arguments)
		arg->update_buffers();
}

void rhr::stack::block::on_reload_swap_chain()
{
	m_background->reload_swap_chain();

	for (auto& arg : m_arguments)
		arg->reload_swap_chain();
}

void rhr::stack::block::post_position_update()
{
	m_background->set_super_position(m_position + m_super_position);

	for (auto& arg : m_arguments)
		arg->set_super_position(m_position + m_super_position);
}

void rhr::stack::block::update_arguments()
{
	m_arguments.clear();
	m_arguments.reserve(m_mod_block->get_arguments().size());

	std::vector<cap::mod::block::block::argument::initializer> argumentInit = m_mod_block->get_arguments();

	u32 width = rhr::stack::block::padding;
	cap::color argColor = cap::color().from_normalized(m_mod_category->get_color().get_normalized_scaled(0.25f, false));

	for (usize i = 0; i < argumentInit.size(); i++)
	{
		if (argumentInit[i].get_type() == cap::mod::block::block::argument::type::TEXT)
		{
			std::shared_ptr<rhr::stack::argument::text> arg = std::make_shared<rhr::stack::argument::text>(argColor, &m_function_block_update);
			arg->set_weak(arg);
			m_arguments.push_back(arg);

			arg->set_position({ width, rhr::stack::block::padding });
			arg->set_super_position(m_position + m_super_position);
			arg->set_data(argumentInit[i].get_default_value());
			arg->set_mode(argumentInit[i].get_mode());
			arg->set_mode_restriction(argumentInit[i].get_restriction());

			width += arg->get_width() + rhr::stack::block::padding;
		}
		else if (argumentInit[i].get_type() == cap::mod::block::block::argument::type::REAL)
		{
			std::shared_ptr<rhr::stack::argument::real> arg = std::make_shared<rhr::stack::argument::real>(argColor, &m_function_block_update);
			arg->set_weak(arg);
			m_arguments.push_back(arg);

			arg->set_position({ width, rhr::stack::block::padding });
			arg->set_super_position(m_position + m_super_position);
			arg->set_data(argumentInit[i].get_default_value());
			arg->set_mode(argumentInit[i].get_mode());
			arg->set_mode_restriction(argumentInit[i].get_restriction());

			width += arg->get_width() + rhr::stack::block::padding;
		}
		else if (argumentInit[i].get_type() == cap::mod::block::block::argument::type::STRING)
		{
			std::shared_ptr<rhr::stack::argument::string> arg = std::make_shared<rhr::stack::argument::string>(argColor, &m_function_block_update);
			arg->set_weak(arg);
			m_arguments.push_back(arg);

			arg->set_position({ width, rhr::stack::block::padding });
			arg->set_super_position(m_position + m_super_position);
			arg->set_data(argumentInit[i].get_default_value());
			arg->set_mode(argumentInit[i].get_mode());
			arg->set_mode_restriction(argumentInit[i].get_restriction());

			width += arg->get_width() + rhr::stack::block::padding;
		}
		else if (argumentInit[i].get_type() == cap::mod::block::block::argument::type::BOOL)
		{
			std::shared_ptr<rhr::stack::argument::boolean> arg = std::make_shared<rhr::stack::argument::boolean>(argColor, &m_function_block_update);
			arg->set_weak(arg);
			m_arguments.push_back(arg);

			arg->set_position({ width, rhr::stack::block::padding });
			arg->set_super_position(m_position + m_super_position);
			arg->set_data(argumentInit[i].get_default_value());
			arg->set_mode(argumentInit[i].get_mode());
			arg->set_mode_restriction(argumentInit[i].get_restriction());

			width += arg->get_width() + rhr::stack::block::padding;
		}
	}

	update_width();
}

void rhr::stack::block::update_width()
{
	m_width = rhr::stack::block::padding;

	for (usize i = 0; i < m_arguments.size(); i++)
	{
		m_arguments[i]->set_position({ m_width, rhr::stack::block::padding });

		m_width += m_arguments[i]->get_width();
		m_width += rhr::stack::block::padding;
	}

	m_size.x = m_width;

	m_background->set_size(m_size);
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
