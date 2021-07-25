#include "collection.hpp"

#include "rhr/rendering/renderer.hpp"

rhr::stack::collection::collection()
	: rhr::render::interfaces::i_enableable(true)
	, m_background(std::make_shared<rhr::render::object::rectangle>())
	, m_display_vanity(true)
{
	m_background->set_weak(m_background);
	m_background->set_depth(rhr::render::renderer::depth_collection);
	m_stacks.reserve(5);
}

void rhr::stack::collection::add_stack(std::shared_ptr<rhr::stack::stack> stack, bool auto_size)
{
	stack->set_super_position(m_position + m_super_position);
	m_stacks.push_back(stack);

	if (auto_size)
	{
		i32 empty_space = COLLECTION_EMPTY_SPACE;

		if (!m_display_vanity)
			empty_space = 0;

		glm::vec<2, i32> size = get_size();
		glm::vec<2, i32> position = get_position();

		if (size.x < empty_space)
			size.x = empty_space;

		if (size.y < empty_space)
			size.y = empty_space;

		if (stack->get_position().x + stack->get_widest_block() > size.x - empty_space)
			size.x = stack->get_position().x + stack->get_widest_block() + empty_space;

		if (stack->get_position().x < empty_space)
		{
			i32 diff = empty_space - stack->get_position().x;
			position.x -= diff;
			size.x += diff;
			stack->set_position(stack->get_position() + glm::vec<2, i32>(diff, 0));
		}

		if (stack->get_position().y + (stack->get_blocks().size() * rhr::stack::block::height) > size.y - empty_space)
			size.y = stack->get_position().y + (stack->get_blocks().size() * rhr::stack::block::height) + empty_space;

		if (stack->get_position().y < empty_space)
		{
			i32 diff = empty_space - stack->get_position().y;
			position.y -= diff;
			size.y += diff;
			stack->set_position(stack->get_position() + glm::vec<2, i32>(0, diff));
		}

		set_size(size);
		set_position(position);
	}

	//glm::vec<2, i32> maxBounds = /*m_position + m_super_position + */m_Size;
	//glm::vec<2, i32> queryPosition = stack->get_position()/* + stack->GetSuperOffset()*/;
	//queryPosition.x += stack->get_widest_block();
	//queryPosition.y += stack->get_blocks().size() * rhr::stack::block::height;

	//if (queryPosition.x > maxBounds.x || queryPosition.y > maxBounds.y)
	//	set_size(queryPosition - m_position - m_super_position + 100);
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
		stacks[i]->set_super_position(m_position + m_super_position);
		m_stacks.push_back(stacks[i]);

		if (auto_size)
		{
			glm::vec<2, i32> size = get_size();
			glm::vec<2, i32> position = get_position();

			if (stacks[i]->get_position().x + stacks[i]->get_widest_block() > size.x - empty_space)
				size.x = stacks[i]->get_position().x + stacks[i]->get_widest_block() + empty_space;

			if (stacks[i]->get_position().x < empty_space)
			{
				i32 diff = empty_space - stacks[i]->get_position().x;
				position.x -= diff;
				size.x += diff;
			}

			if (stacks[i]->get_position().y + (stacks[i]->get_blocks().size() * rhr::stack::block::height) > size.y - empty_space)
				size.y = stacks[i]->get_position().y + (stacks[i]->get_blocks().size() * rhr::stack::block::height) + empty_space;

			if (stacks[i]->get_position().y < empty_space)
			{
				i32 diff = empty_space - stacks[i]->get_position().y;
				position.y -= diff;
				size.y += diff;
			}

			set_size(size);
			set_position(position);
		}

		//glm::vec<2, i32> maxBounds = m_position + m_super_position + m_Size;
		//glm::vec<2, i32> queryPosition = stacks[i]->get_position();
		//queryPosition.x += stacks[i]->get_widest_block();
		//queryPosition.y += stacks[i]->get_blocks().size() * rhr::stack::block::height;

		//if (queryPosition.x > maxBounds.x || queryPosition.y > maxBounds.y)
		//	set_size(queryPosition - m_position - m_super_position + 100);
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

	glm::vec<2, i32> earliest_position = m_stacks.front()->get_position();

	for (auto& stack : m_stacks)
	{
		if (stack->get_position().x < earliest_position.x)
			earliest_position.x = stack->get_position().x;

		if (stack->get_position().y < earliest_position.y)
			earliest_position.y = stack->get_position().y;
	}

	// Position

	if (offset_stacks)
	{
		glm::vec<2, i32> shift = glm::vec<2, i32>(padding, padding) - earliest_position;

		for (auto& stack : m_stacks)
		{
			stack->set_position(stack->get_position() + shift);
		}
	}
	else
	{
		glm::vec<2, i32> shift = earliest_position - glm::vec<2, i32>(padding, padding);
		set_position(get_position() + shift);

		for (auto& stack : m_stacks)
		{
			stack->set_position(stack->get_position() - shift);
		}
	}

	glm::vec<2, i32> latest_position = m_stacks.front()->get_position();

	for (auto& stack : m_stacks)
	{
		glm::vec<2, i32> stack_last_bounds = stack->get_position() + stack->get_size();

		if (stack_last_bounds.x > latest_position.x)
			latest_position.x = stack_last_bounds.x;

		if (stack_last_bounds.y > latest_position.y)
			latest_position.y = stack_last_bounds.y;
	}

	// Size

	glm::vec<2, i32> size = latest_position + glm::vec<2, i32>(padding, padding);
	set_size(size);
}

void rhr::stack::collection::check_bounds()
{
	i32 empty_space = COLLECTION_EMPTY_SPACE;

	if (!m_display_vanity)
		empty_space = 0;

	for (usize i = 0; i < m_stacks.size(); i++)
	{
		glm::vec<2, i32> size = get_size();
		glm::vec<2, i32> position = get_position();

		if (m_stacks[i]->get_position().x + m_stacks[i]->get_widest_block() > size.x - empty_space)
			size.x = m_stacks[i]->get_position().x + m_stacks[i]->get_widest_block() + empty_space;

		if (m_stacks[i]->get_position().x < empty_space)
		{
			i32 diff = empty_space - m_stacks[i]->get_position().x;
			position.x -= diff;
			size.x += diff;
		}

		if (m_stacks[i]->get_position().y + (m_stacks[i]->get_blocks().size() * rhr::stack::block::height) > size.y - empty_space)
			size.y = m_stacks[i]->get_position().y + (m_stacks[i]->get_blocks().size() * rhr::stack::block::height) + empty_space;

		if (m_stacks[i]->get_position().y < empty_space)
		{
			i32 diff = empty_space - m_stacks[i]->get_position().y;
			position.y -= diff;
			size.y += diff;
		}

		set_size(size);
		set_position(position);
	}
}

void rhr::stack::collection::remove_stack(u64 idx)
{
	m_stacks.erase(m_stacks.begin() + idx);
}

void rhr::stack::collection::remove_all()
{
	m_stacks.clear();
}

void rhr::stack::collection::display_vanity(bool enabled)
{
	m_display_vanity = enabled;
}

const std::vector<std::shared_ptr<rhr::stack::stack>>& rhr::stack::collection::get_stacks()
{
	return m_stacks;
}

void rhr::stack::collection::frame_update(f64 delta_time)
{
	for (auto& stack : m_stacks)
		stack->frame_update(delta_time);
}

void rhr::stack::collection::on_render()
{
	for (auto& stack : m_stacks)
		stack->render();

	if (m_display_vanity)
		m_background->render();
}

void rhr::stack::collection::on_update_buffers()
{
	for (auto& stack : m_stacks)
		stack->update_buffers();

	m_background->update_buffers();
}

void rhr::stack::collection::on_reload_swap_chain()
{
	for (auto& stack : m_stacks)
		stack->reload_swap_chain();

	m_background->reload_swap_chain();
}

void rhr::stack::collection::post_position_update()
{
	for (auto& stack : m_stacks)
		stack->set_super_position(m_position + m_super_position);

	m_background->set_super_position(m_position + m_super_position);
}

void rhr::stack::collection::post_size_update()
{
	m_background->set_super_bounds(m_size);
	m_background->set_size_max();
}