#include "block.hpp"

#include "rhr/stacking/arguments/text.hpp"
#include "rhr/stacking/arguments/real.hpp"
#include "rhr/stacking/arguments/boolean.hpp"
#include "rhr/stacking/arguments/string.hpp"
#include "rhr/stacking/arguments/any.hpp"
#include "rhr/registries/block.hpp"
#include "rhr/rendering/renderer.hpp"

rhr::stack::block::block(const std::string& unlocalized_name)
	: m_mod_block(rhr::registry::block::get_registry().get_block(unlocalized_name)->BlockModBlock)
	, m_background(std::make_shared<rhr::render::object::rectangle>())
{
	m_size = { 100, rhr::stack::block::height };
	m_mod_category = rhr::registry::block::get_registry().get_categories(m_mod_block->GetCategory())->CatagoryModCatagory;
	m_background->set_weak(m_background);
	m_background->set_color(m_mod_category->GetColor());
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

const ModBlock* rhr::stack::block::get_mod_block()
{
	return m_mod_block;
}

const ModCatagory* rhr::stack::block::get_mod_category()
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
	m_arguments.reserve(m_mod_block->GetArguments().size());

	std::vector<BlockArgumentInitializer> argumentInit = m_mod_block->GetArguments();

	u32 width = rhr::stack::block::padding;
	cap::color argColor = cap::color().from_normalized(m_mod_category->GetColor().get_normalized_scaled(0.25f, false));

	for (usize i = 0; i < argumentInit.size(); i++)
	{
		if (argumentInit[i].Type == BlockArgumentType::TEXT)
		{
			std::shared_ptr<rhr::stack::argument::text> arg = std::make_shared<rhr::stack::argument::text>(argColor);
			arg->set_weak(arg);
			m_arguments.push_back(arg);

			arg->set_position({ width, rhr::stack::block::padding });
			arg->set_super_position(m_position + m_super_position);
			arg->set_data(argumentInit[i].DefaultValue);

			width += arg->get_width() + rhr::stack::block::padding;
		}
		else if (argumentInit[i].Type == BlockArgumentType::REAL)
		{
			std::shared_ptr<rhr::stack::argument::real> arg = std::make_shared<rhr::stack::argument::real>(argColor);
			arg->set_weak(arg);
			m_arguments.push_back(arg);

			arg->set_position({ width, rhr::stack::block::padding });
			arg->set_super_position(m_position + m_super_position);
			arg->set_data(argumentInit[i].DefaultValue);

			width += arg->get_width() + rhr::stack::block::padding;
		}
		else if (argumentInit[i].Type == BlockArgumentType::STRING)
		{
			std::shared_ptr<rhr::stack::argument::string> arg = std::make_shared<rhr::stack::argument::string>(argColor);
			arg->set_weak(arg);
			m_arguments.push_back(arg);

			arg->set_position({ width, rhr::stack::block::padding });
			arg->set_super_position(m_position + m_super_position);
			arg->set_data(argumentInit[i].DefaultValue);

			width += arg->get_width() + rhr::stack::block::padding;
		}
		else if (argumentInit[i].Type == BlockArgumentType::BOOL)
		{
			std::shared_ptr<rhr::stack::argument::boolean> arg = std::make_shared<rhr::stack::argument::boolean>(argColor);
			arg->set_weak(arg);
			m_arguments.push_back(arg);

			arg->set_position({ width, rhr::stack::block::padding });
			arg->set_super_position(m_position + m_super_position);
			arg->set_data(argumentInit[i].DefaultValue);

			width += arg->get_width() + rhr::stack::block::padding;
		}
		/*else if (argumentInit[i].Type == BlockArgumentType::ANY)
		{
			args.push_back(new ArgumentAny());

			args.back()->setPosition(width, Global::BlockBorder);
			args.back()->SetData(argumentInit[i].DefaultValue);
			args.back()->UpdateData();

			width += args.back()->GetWidth();
		}*/
	}

	update_width();
}

void rhr::stack::block::update_width()
{
	m_width = rhr::stack::block::padding;

	for (usize i = 0; i < m_arguments.size(); i++)
	{
		m_width += m_arguments[i]->get_width();
		m_width += rhr::stack::block::padding;
	}

	m_size.x = m_width;

	m_background->set_size(m_size);
}