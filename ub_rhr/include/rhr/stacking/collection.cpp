#include "collection.hpp"

#include "rhr/rendering/renderer.hpp"

rhr::stack::collection::collection()
	: m_background(std::make_shared<rhr::render::object::rectangle>())
	, m_display_vanity(true)
{
	m_function_collection_update = [&]() { check_bounds(); };

	m_background->set_weak(m_background);
	m_background->set_depth(rhr::render::renderer::depth_collection);
	m_background->set_color(cap::color().from_u8({25, 25, 30, 255}));
	m_stacks.reserve(5);
}

void rhr::stack::collection::add_stack(std::shared_ptr<rhr::stack::stack> stack, bool auto_size)
{
	update_child_transform(stack, i_ui::transform_update_spec_position | i_ui::transform_update_spec_size);
	stack->set_collection_update_function(&m_function_collection_update);
	m_stacks.push_back(stack);

	if (auto_size)
	{
		i32 empty_space = COLLECTION_EMPTY_SPACE;

		if (!m_display_vanity)
			empty_space = 0;

		glm::vec<2, i32> size	  = get_size_local();
		glm::vec<2, i32> position = get_position_local_physical();

		if (size.x < empty_space)
			size.x = empty_space;

		if (size.y < empty_space)
			size.y = empty_space;

		if (stack->get_position_local_physical().x + stack->get_widest_block() > size.x - empty_space)
			size.x = stack->get_position_local_physical().x + static_cast<i32>(stack->get_widest_block()) + empty_space;

		if (stack->get_position_local_physical().x < empty_space)
		{
			i32 diff = empty_space - stack->get_position_local_physical().x;
			position.x -= diff;
			size.x += diff;
			stack->set_position_local_physical(stack->get_position_local_physical() + glm::vec<2, i32>(diff, 0), true);
		}

		if (stack->get_position_local_physical().y + (stack->get_blocks().size() * rhr::stack::block::height) > size.y - empty_space)
			size.y = stack->get_position_local_physical().y + (static_cast<i32>(stack->get_blocks().size()) * static_cast<i32>(rhr::stack::block::height)) + empty_space;

		if (stack->get_position_local_physical().y < empty_space)
		{
			i32 diff = empty_space - stack->get_position_local_physical().y;
			position.y -= diff;
			size.y += diff;
			stack->set_position_local_physical(stack->get_position_local_physical() + glm::vec<2, i32>(0, diff), true);
		}

		set_size_local(size, true);
		set_position_local_physical(position, true);
	}
}

void rhr::stack::collection::add_stacks(const std::vector<std::shared_ptr<rhr::stack::stack>>& stacks, bool auto_size)
{
	if (m_stacks.size() + stacks.size() >= m_stacks.capacity())
		m_stacks.reserve((u64)std::ceil((f32)(m_stacks.size() + stacks.size()) * 1.5f + 10.0f));

	i32 empty_space = COLLECTION_EMPTY_SPACE;

	if (!m_display_vanity)
		empty_space = 0;

	for (u64 i = 0; i < stacks.size(); i++)
	{
		update_child_transform(stacks[i], true);
		m_stacks.push_back(stacks[i]);

		if (auto_size)
		{
			glm::vec<2, i32> size	  = get_size_local();
			glm::vec<2, i32> position = get_position_local_physical();

			if (stacks[i]->get_position_local_physical().x + stacks[i]->get_widest_block() > size.x - empty_space)
				size.x = stacks[i]->get_position_local_physical().x + static_cast<i32>(stacks[i]->get_widest_block()) + empty_space;

			if (stacks[i]->get_position_local_physical().x < empty_space)
			{
				i32 diff = empty_space - stacks[i]->get_position_local_physical().x;
				position.x -= diff;
				size.x += diff;
			}

			if (stacks[i]->get_position_local_physical().y + (stacks[i]->get_blocks().size() * rhr::stack::block::height) > size.y - empty_space)
				size.y =
					stacks[i]->get_position_local_physical().y + (static_cast<i32>(stacks[i]->get_blocks().size()) * static_cast<i32>(rhr::stack::block::height)) + empty_space;

			if (stacks[i]->get_position_local_physical().y < empty_space)
			{
				i32 diff = empty_space - stacks[i]->get_position_local_physical().y;
				position.y -= diff;
				size.y += diff;
			}

			set_size_local(size, true);
			set_position_local_physical(position, true);
		}
	}
}

void rhr::stack::collection::size_to_stacks(bool offset_stacks, bool use_padding)
{
	// Config

	if (m_stacks.size() == 0)
		return;

	i32 padding = 0;

	if (use_padding && m_display_vanity)
		padding = COLLECTION_EMPTY_SPACE;

	glm::vec<2, i32> earliest_position = m_stacks.front()->get_position_local_physical();

	for (auto& stack : m_stacks)
	{
		if (stack->get_position_local_physical().x < earliest_position.x)
			earliest_position.x = stack->get_position_local_physical().x;

		if (stack->get_position_local_physical().y < earliest_position.y)
			earliest_position.y = stack->get_position_local_physical().y;
	}

	// Position

	if (offset_stacks)
	{
		glm::vec<2, i32> shift = glm::vec<2, i32>(padding, padding) - earliest_position;

		for (auto& stack : m_stacks)
		{
			stack->set_position_local_physical(stack->get_position_local_physical() + shift, true);
		}
	}
	else
	{
		glm::vec<2, i32> shift = earliest_position - glm::vec<2, i32>(padding, padding);
		set_position_local_physical(get_position_local_physical() + shift, true);

		for (auto& stack : m_stacks)
		{
			stack->set_position_local_physical(stack->get_position_local_physical() - shift, true);
		}
	}

	glm::vec<2, i32> latest_position = m_stacks.front()->get_position_local_physical();

	for (auto& stack : m_stacks)
	{
		glm::vec<2, i32> stack_last_bounds = stack->get_position_local_physical() + stack->get_size_local();

		if (stack_last_bounds.x > latest_position.x)
			latest_position.x = stack_last_bounds.x;

		if (stack_last_bounds.y > latest_position.y)
			latest_position.y = stack_last_bounds.y;
	}

	// Size

	glm::vec<2, i32> size = latest_position + glm::vec<2, i32>(padding, padding);
	set_size_local(size, true);
}

void rhr::stack::collection::check_bounds()
{
	i32 empty_space = COLLECTION_EMPTY_SPACE;

	if (!m_display_vanity)
		empty_space = 0;

	for (usize i = 0; i < m_stacks.size(); i++)
	{
		glm::vec<2, i32> size	  = get_size_local();
		glm::vec<2, i32> position = get_position_local_physical();

		if (m_stacks[i]->get_position_local_physical().x + m_stacks[i]->get_widest_block() > size.x - empty_space)
			size.x = m_stacks[i]->get_position_local_physical().x + static_cast<i32>(m_stacks[i]->get_widest_block()) + empty_space;

		if (m_stacks[i]->get_position_local_physical().x < empty_space)
		{
			i32 diff = empty_space - m_stacks[i]->get_position_local_physical().x;
			position.x -= diff;
			size.x += diff;
		}

		if (m_stacks[i]->get_position_local_physical().y + (m_stacks[i]->get_blocks().size() * rhr::stack::block::height) > size.y - empty_space)
			size.y =
				m_stacks[i]->get_position_local_physical().y + (static_cast<i32>(m_stacks[i]->get_blocks().size()) * static_cast<i32>(rhr::stack::block::height)) + empty_space;

		if (m_stacks[i]->get_position_local_physical().y < empty_space)
		{
			i32 diff = empty_space - m_stacks[i]->get_position_local_physical().y;
			position.y -= diff;
			size.y += diff;
		}

		set_size_local(size, true);
		set_position_local_physical(position, true);
	}
}

void rhr::stack::collection::remove_stack(u64 idx, u64 block_offset)
{
	if (block_offset == 0)
		m_stacks.erase(m_stacks.begin() + static_cast<i64>(idx));
	else
		m_stacks[static_cast<usize>(idx)]->remove_blocks_end(block_offset);
}

void rhr::stack::collection::remove_all() { m_stacks.clear(); }

void rhr::stack::collection::display_vanity(bool enabled) { m_display_vanity = enabled; }

const std::vector<std::shared_ptr<rhr::stack::stack>>& rhr::stack::collection::get_stacks() { return m_stacks; }

void rhr::stack::collection::ui_transform_update(i_ui::transform_update_spec transform_update_spec)
{
	update_child_transform(m_background, i_ui::transform_update_spec_position);
	m_background->set_size_max(true);

	for (auto& stack : m_stacks)
		update_child_transform(stack, true);
}

void rhr::stack::collection::ui_render()
{
	if (m_display_vanity)
		m_background->render();

	for (auto& stack : m_stacks)
		stack->render();
}

void rhr::stack::collection::ui_reload_swap_chain()
{
	m_background->reload_swap_chain();

	for (auto& stack : m_stacks)
		stack->reload_swap_chain();
}

void rhr::stack::collection::ui_update_buffers()
{
	m_background->update_buffers();
}

void rhr::stack::collection::ui_chain_update_buffers()
{
	for (auto& stack : m_stacks)
		stack->update_buffers();
}

void rhr::stack::collection::ui_frame_update(f64 delta_time)
{
	for (auto& stack : m_stacks)
		stack->frame_update(delta_time);
}
