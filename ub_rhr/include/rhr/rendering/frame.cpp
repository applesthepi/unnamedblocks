#include "frame.hpp"

#include "rhr/rendering/renderer.hpp"

static void mouse_button_caller(glm::vec<2, i32> position, f32 scroll, MouseOperation operation, MouseButton button, void* data)
{
	rhr::render::frame* frame = (rhr::render::frame*)data;
	frame->mouse_button(position, scroll, operation);
}

rhr::render::frame::frame()
	: m_plane(rhr::render::cardinal::plane::HORIZONTAL)
	, m_has_space(false)
	, m_has_frame(false)
	, m_has_content(false)
	, m_link_up(false)
	, m_link_down(false)
	, m_link_left(false)
	, m_link_right(false)
	, m_mouse_button_registered(false)
	, m_dragging(false)
	, m_dragging_bar_idx(0)
	, m_dragging_mouse_begin({ 0, 0 })
	, m_dragging_object_begin({ 0, 0 })
	, m_padding(6)
	, m_background_enabled(false)
	, m_disable_bar_movement(false)
{
	
}

void rhr::render::frame::set_padding(u8 padding)
{
	m_padding = padding;
	update_content_dimentions();
}

void rhr::render::frame::set_frame(std::shared_ptr<frame>& frame)
{
	if (m_has_content)
	{
		Logger::Error("can not set frame of frame that has content");
		return;
	}

	reset_drag();

	m_has_space = false;
	m_has_frame = true;

	m_frames.clear();
	m_frames.push_back(frame);
}

usize rhr::render::frame::add_frame(std::shared_ptr<frame>& frame, rhr::render::cardinal::local cardinal)
{
	if (m_has_content)
	{
		Logger::Error("can not add frame to frame that has content");
		return 0;
	}

	reset_drag();

	m_has_frame = true;

	if (!m_has_space)
	{
		m_has_space = true;

		if (cardinal == rhr::render::cardinal::local::UP || cardinal == rhr::render::cardinal::local::DOWN)
			m_plane = rhr::render::cardinal::plane::VERTICAL;
		else if (cardinal == rhr::render::cardinal::local::LEFT || cardinal == rhr::render::cardinal::local::RIGHT)
			m_plane = rhr::render::cardinal::plane::HORIZONTAL;
	}

	usize idx = 0;

	if (cardinal == rhr::render::cardinal::local::UP && m_plane == rhr::render::cardinal::plane::VERTICAL)
		m_frames.insert(m_frames.begin(), frame);
	else if (cardinal == rhr::render::cardinal::local::DOWN && m_plane == rhr::render::cardinal::plane::VERTICAL)
	{
		idx = m_frames.size();
		m_frames.insert(m_frames.end(), frame);
	}
	else if (cardinal == rhr::render::cardinal::local::LEFT && m_plane == rhr::render::cardinal::plane::HORIZONTAL)
		m_frames.insert(m_frames.begin(), frame);
	else if (cardinal == rhr::render::cardinal::local::RIGHT && m_plane == rhr::render::cardinal::plane::HORIZONTAL)
	{
		idx = m_frames.size();
		m_frames.insert(m_frames.end(), frame);
	}
	else
		Logger::Error("wrong direction when adding a frame to a frame");

	m_frames_enabled.push_back(true);

	if (m_frames.size() > 1)
	{
		submit_new_bar_position(cardinal);
		equalize_bars(false);
	}

	update_links();
	update_content_dimentions();

	return idx;
}

void rhr::render::frame::enable_frame(usize idx, bool enabled)
{
	if (idx >= m_frames_enabled.size())
	{
		Logger::Error("frame idx out of range");
		return;
	}

	m_frames_enabled[idx] = enabled;
	update_content_dimentions();
}

void rhr::render::frame::add_content(std::weak_ptr<rhr::render::interfaces::i_renderable>&& renderable,
									 std::weak_ptr<rhr::render::interfaces::i_updateable>&& updatable,
									 std::weak_ptr<rhr::render::interfaces::i_positionable<2, i32>>&& positionable,
									 std::weak_ptr<rhr::render::interfaces::i_sizeable<2, i32>>&& sizeable,
									 std::weak_ptr<rhr::render::interfaces::i_enableable>&& enableable,
									 rhr::render::cardinal::local cardinal)
{
	if (m_has_frame)
	{
		Logger::Error("can not add content to frame that has internal frames");
		return;
	}

	reset_drag();

	m_has_content = true;

	if (!m_has_space)
	{
		m_has_space = true;

		if (cardinal == rhr::render::cardinal::local::UP || cardinal == rhr::render::cardinal::local::DOWN)
			m_plane = rhr::render::cardinal::plane::VERTICAL;
		else if (cardinal == rhr::render::cardinal::local::LEFT || cardinal == rhr::render::cardinal::local::RIGHT)
			m_plane = rhr::render::cardinal::plane::HORIZONTAL;
	}

	if (auto weak = positionable.lock())
		weak->set_super_position(m_position + m_super_position);

	if (cardinal == rhr::render::cardinal::local::UP && m_plane == rhr::render::cardinal::plane::VERTICAL)
		m_content.insert(m_content.begin(), { std::move(renderable), std::move(updatable), std::move(positionable), std::move(sizeable), std::move(enableable) });
	else if (cardinal == rhr::render::cardinal::local::DOWN && m_plane == rhr::render::cardinal::plane::VERTICAL)
		m_content.insert(m_content.end(), { std::move(renderable), std::move(updatable), std::move(positionable), std::move(sizeable), std::move(enableable) });
	else if (cardinal == rhr::render::cardinal::local::LEFT && m_plane == rhr::render::cardinal::plane::HORIZONTAL)
		m_content.insert(m_content.begin(), { std::move(renderable), std::move(updatable), std::move(positionable), std::move(sizeable), std::move(enableable) });
	else if (cardinal == rhr::render::cardinal::local::RIGHT && m_plane == rhr::render::cardinal::plane::HORIZONTAL)
		m_content.insert(m_content.end(), { std::move(renderable), std::move(updatable), std::move(positionable), std::move(sizeable), std::move(enableable) });
	else
		Logger::Error("wrong direction when adding a frame to a frame");

	if (m_content.size() > 1)
	{
		submit_new_bar_position(cardinal);
		equalize_bars(true);
	}

	update_content_dimentions();
}

void rhr::render::frame::mouse_button(glm::vec<2, i32> position, f32 scroll, MouseOperation operation)
{
	if (m_disable_bar_movement)
		return;

	if (operation == MouseOperation::Press)
	{
		for (usize i = 0; i < m_bars.size(); i++)
		{
			if (position.x >= m_bars[i].absolute_position.x && position.x < m_bars[i].absolute_position.x + m_bars[i].size.x &&
				position.y >= m_bars[i].absolute_position.y && position.y < m_bars[i].absolute_position.y + m_bars[i].size.y)
			{
				m_dragging = true;
				m_dragging_bar_idx = i;
				m_dragging_mouse_begin = position;
				m_dragging_object_begin = m_bars[i].absolute_position;

				break;
			}
		}
	}
	else if (operation == MouseOperation::Release)
	{
		if (m_dragging)
		{
			m_dragging = false;
			m_dragging_bar_idx = -1;
			m_dragging_mouse_begin = { 0, 0 };
			m_dragging_object_begin = { 0, 0 };
		}
	}
	else if (operation == MouseOperation::Move)
	{
		if (m_dragging)
		{
			m_bars[m_dragging_bar_idx].absolute_position = m_dragging_object_begin + (position - m_dragging_mouse_begin);
			update_bars_from_absolute();
			update_content_dimentions();
		}
	}
}

void rhr::render::frame::link(rhr::render::cardinal::local cardinal)
{
	if (cardinal == rhr::render::cardinal::local::UP)
		m_link_up = true;
	else if (cardinal == rhr::render::cardinal::local::DOWN)
		m_link_down = true;
	else if (cardinal == rhr::render::cardinal::local::LEFT)
		m_link_left = true;
	else if (cardinal == rhr::render::cardinal::local::RIGHT)
		m_link_right = true;
}

void rhr::render::frame::unlink(rhr::render::cardinal::local cardinal)
{
	if (cardinal == rhr::render::cardinal::local::UP)
		m_link_up = false;
	else if (cardinal == rhr::render::cardinal::local::DOWN)
		m_link_down = false;
	else if (cardinal == rhr::render::cardinal::local::LEFT)
		m_link_left = false;
	else if (cardinal == rhr::render::cardinal::local::RIGHT)
		m_link_right = false;
}

void rhr::render::frame::update_links()
{
	if (m_has_content)
	{
		Logger::Error("can not update links of a frame that has content");
		return;
	}

	if (m_plane == rhr::render::cardinal::plane::HORIZONTAL)
	{
		if (m_frames.size() == 1)
		{
			m_frames.front()->unlink(rhr::render::cardinal::local::LEFT);
			m_frames.front()->unlink(rhr::render::cardinal::local::RIGHT);
		}
		else
		{
			for (usize i = 0; i < m_frames.size(); i++)
			{
				push_links(m_frames[i]);

				if (i == 0)
				{
					m_frames[i]->link(rhr::render::cardinal::local::RIGHT);
				}
				else if (i == m_frames.size() - 1)
				{
					m_frames[i]->link(rhr::render::cardinal::local::LEFT);
				}
				else
				{
					m_frames[i]->link(rhr::render::cardinal::local::LEFT);
					m_frames[i]->link(rhr::render::cardinal::local::RIGHT);
				}
			}
		}
	}
	else if (m_plane == rhr::render::cardinal::plane::VERTICAL)
	{
		if (m_frames.size() == 1)
		{
			m_frames.front()->unlink(rhr::render::cardinal::local::UP);
			m_frames.front()->unlink(rhr::render::cardinal::local::DOWN);
		}
		else
		{
			for (usize i = 0; i < m_frames.size(); i++)
			{
				push_links(m_frames[i]);

				if (i == 0)
				{
					m_frames[i]->link(rhr::render::cardinal::local::DOWN);
				}
				else if (i == m_frames.size() - 1)
				{
					m_frames[i]->link(rhr::render::cardinal::local::UP);
				}
				else
				{
					m_frames[i]->link(rhr::render::cardinal::local::UP);
					m_frames[i]->link(rhr::render::cardinal::local::DOWN);
				}
			}
		}
	}
}

void rhr::render::frame::set_bar(usize idx, i32 offset)
{
	m_bars[idx].offset = offset;
	update_bars_from_relative();
	update_content_dimentions();
}

void rhr::render::frame::enable_background(const cap::color& color)
{
	m_background_enabled = true;

	m_background = std::make_shared<rhr::render::object::rectangle>();
	m_background->set_weak(m_background);
	m_background->set_color(color);
	m_background->set_depth(rhr::render::renderer::depth_frame_background);

	update_background();
}

void rhr::render::frame::disable_bar_movement()
{
	m_disable_bar_movement = true;
}

void rhr::render::frame::on_render()
{
	if (m_has_frame)
	{
		for (usize i = 0; i < m_frames.size(); i++)
		{
			if (m_frames_enabled[i])
				((rhr::render::interfaces::i_renderable*)m_frames[i].get())->render();
		}
	}
	else if (m_has_content)
	{
		bool erased = false;

		for (usize i = 0; i < m_content.size(); i++)
		{
			if (erased)
			{
				erased = false;
				i--;
			}

			if (auto enabled = m_content[i].enableable.lock())
				if (!enabled->get_enabled())
					continue;

			if (auto content = m_content[i].renderable.lock())
				content->render();
			else
			{
				m_content.erase(m_content.begin() + i);
				erased = true;
			}
		}
	}

	if (m_background_enabled)
		m_background->render();
}

void rhr::render::frame::on_update_buffers()
{
	clear_dirty();
	bool erased = false;

	for (usize i = 0; i < m_content.size(); i++)
	{
		if (erased)
		{
			erased = false;
			i--;
		}

		if (auto content = m_content[i].renderable.lock())
			content->update_buffers();
		else
		{
			m_content.erase(m_content.begin() + i);
			erased = true;
		}
	}

	if (m_background_enabled)
		m_background->update_buffers();
}

void rhr::render::frame::on_reload_swap_chain()
{
	if (m_has_frame)
	{
		bool erased = false;

		for (auto& frame : m_frames)
			((rhr::render::interfaces::i_renderable*)frame.get())->reload_swap_chain();
	}
	else if (m_has_content)
	{
		bool erased = false;

		for (usize i = 0; i < m_content.size(); i++)
		{
			if (erased)
			{
				erased = false;
				i--;
			}

			if (auto content = m_content[i].renderable.lock())
				content->reload_swap_chain();
			else
			{
				m_content.erase(m_content.begin() + i);
				erased = true;
			}
		}
	}

	if (m_background_enabled)
		m_background->reload_swap_chain();
}

void rhr::render::frame::on_set_weak()
{
	
}

void rhr::render::frame::post_position_update()
{
	set_size_max();
	update_content_dimentions();
}

void rhr::render::frame::post_size_update()
{
	update_content_dimentions();
}

void rhr::render::frame::update_content_dimentions()
{
	if (m_has_frame)
	{
		glm::vec<2, i32> running_position = m_position + m_super_position;
		glm::vec<2, i32> running_size;

		for (usize i = 0; i < m_frames.size(); i++)
		{
			if (!m_frames_enabled[i])
				continue;

			if (i > 0)
			{
				if (m_plane == rhr::render::cardinal::plane::HORIZONTAL)
					running_position.x = m_position.x + m_super_position.x + m_bars[i - 1].offset;
				else if (m_plane == rhr::render::cardinal::plane::VERTICAL)
					running_position.y = m_position.y + m_super_position.y + m_bars[i - 1].offset;
			}

			if (m_frames.size() > 1)
			{
				i32 offset = 0;

				if (m_plane == rhr::render::cardinal::plane::HORIZONTAL)
				{
					if (i == m_frames.size() - 1)
						offset = m_size.x - m_bars[i - 1].offset;
					else if (i == 0)
						offset = m_bars[i].offset;
					else
						offset = m_bars[i].offset - m_bars[i - 1].offset;

					running_size = { offset, m_size.y };
				}
				else if (m_plane == rhr::render::cardinal::plane::VERTICAL)
				{
					if (i == m_frames.size() - 1)
						offset = m_size.y - m_bars[i - 1].offset;
					else if (i == 0)
						offset = m_bars[i].offset;
					else
						offset = m_bars[i].offset - m_bars[i - 1].offset;

					running_size = { m_size.x - (static_cast<i32>(0) * 2), offset - static_cast<i32>(static_cast<f32>(0) * 1.5) };
				}
			}
			else
			{
				running_size = m_size - (static_cast<i32>(0) * 2);
			}

			if (i == 0)
				m_frames[i]->set_super_position(running_position + static_cast<i32>(0));
			else
			{
				if (m_plane == rhr::render::cardinal::plane::HORIZONTAL)
					m_frames[i]->set_super_position(running_position + glm::vec<2, i32>(static_cast<i32>(0) / 2, static_cast<i32>(0)));
				else if (m_plane == rhr::render::cardinal::plane::VERTICAL)
					m_frames[i]->set_super_position(running_position + glm::vec<2, i32>(static_cast<i32>(0), static_cast<i32>(0) / 2));
			}

			m_frames[i]->set_super_bounds(running_size);
		}
	}
	else if (m_has_content)
	{
		bool erased = false;

		glm::vec<2, i32> running_position = m_position + m_super_position + (static_cast<i32>(m_padding) * 2);
		glm::vec<2, i32> running_size;

		for (usize i = 0; i < m_content.size(); i++)
		{
			if (erased)
			{
				erased = false;
				i--;
			}

			if (i > 0)
			{
				if (m_plane == rhr::render::cardinal::plane::HORIZONTAL)
					running_position.x = m_position.x + m_super_position.x + m_bars[i - 1].offset + (static_cast<i32>(m_padding) * (2));
				else if (m_plane == rhr::render::cardinal::plane::VERTICAL)
					running_position.y = m_position.y + m_super_position.y + m_bars[i - 1].offset + (static_cast<i32>(m_padding) * (2));
			}
			else
			{
				if (m_plane == rhr::render::cardinal::plane::HORIZONTAL)
					running_position.x = m_position.x + m_super_position.x + (static_cast<i32>(m_padding) * 2);
				else if (m_plane == rhr::render::cardinal::plane::VERTICAL)
					running_position.y = m_position.y + m_super_position.y + (static_cast<i32>(m_padding) * 2);
			}

			if (m_content.size() > 1)
			{
				i32 offset = 0;

				if (m_plane == rhr::render::cardinal::plane::HORIZONTAL)
				{
					if (i == m_content.size() - 1)
						offset = m_size.x - m_bars[i - 1].offset;
					else if (i == 0)
						offset = m_bars[i].offset;
					else
						offset = m_bars[i].offset - m_bars[i - 1].offset;

					running_size = { offset, m_size.y };
				}
				else if (m_plane == rhr::render::cardinal::plane::VERTICAL)
				{
					if (i == m_content.size() - 1)
						offset = m_size.y - m_bars[i - 1].offset;
					else if (i == 0)
						offset = m_bars[i].offset;
					else
						offset = m_bars[i].offset - m_bars[i - 1].offset;

					running_size = { m_size.x, offset };
				}
			}
			else
			{
				running_size = m_size;
			}

			running_size -= static_cast<i32>(m_padding) * 4;

			if (auto lock = m_content[i].positionable.lock())
				lock->set_super_position(running_position);
			else
			{
				m_content.erase(m_content.begin() + i);
				erased = true;
			}

			if (auto lock = m_content[i].sizeable.lock())
				lock->set_super_bounds(running_size);
			else
			{
				m_content.erase(m_content.begin() + i);
				erased = true;
			}
		}
	}

	if (m_background_enabled)
		update_background();
}

void rhr::render::frame::submit_new_bar_position(rhr::render::cardinal::local cardinal)
{
	if (m_bars.size() == 0)
	{
		if (cardinal == rhr::render::cardinal::local::LEFT || cardinal == rhr::render::cardinal::local::RIGHT)
			m_bars.push_back(bar(m_size.x / 2));
		else if (cardinal == rhr::render::cardinal::local::DOWN || cardinal == rhr::render::cardinal::local::UP)
			m_bars.push_back(bar(m_size.y / 2));
	}
	else
	{
		if (cardinal == rhr::render::cardinal::local::RIGHT)
			m_bars.insert(m_bars.end(), (m_size.x - m_bars.back().offset) / 2 + m_bars.back().offset);
		else if (cardinal == rhr::render::cardinal::local::LEFT)
			m_bars.insert(m_bars.begin(), m_bars.front().offset / 2);
		else if (cardinal == rhr::render::cardinal::local::DOWN)
			m_bars.insert(m_bars.end(), (m_size.y - m_bars.back().offset) / 2 + m_bars.back().offset);
		else if (cardinal == rhr::render::cardinal::local::UP)
			m_bars.insert(m_bars.begin(), m_bars.front().offset / 2);
	}

	update_bars_from_relative();
	update_mouse_button_status(true);
}

void rhr::render::frame::equalize_bars(bool size_to_content)
{
	if (size_to_content)
	{
		i32 offset = 0;

		if (m_plane == rhr::render::cardinal::plane::HORIZONTAL)
		{
			for (usize i = 0; i < m_bars.size(); i++)
			{
				if (auto lock = m_content[i].sizeable.lock())
					offset += lock->get_size().x + static_cast<i32>(m_padding);

				m_bars[i].offset = offset;
			}
		}
		else if (m_plane == rhr::render::cardinal::plane::VERTICAL)
		{
			for (usize i = 0; i < m_bars.size(); i++)
			{
				if (auto lock = m_content[i].sizeable.lock())
					offset += lock->get_size().y + static_cast<i32>(m_padding);

				m_bars[i].offset = offset;
			}
		}
	}
	else if (m_has_content)
	{
		if (m_plane == rhr::render::cardinal::plane::HORIZONTAL)
		{
			for (usize i = 0; i < m_bars.size(); i++)
				m_bars[i].offset = (static_cast<f32>(i + 1) / static_cast<f32>(m_bars.size() + 1)) * m_size.x;
		}
		else if (m_plane == rhr::render::cardinal::plane::VERTICAL)
		{
			for (usize i = 0; i < m_bars.size(); i++)
				m_bars[i].offset = (static_cast<f32>(i + 1) / static_cast<f32>(m_bars.size() + 1)) * m_size.y;
		}
	}

	update_bars_from_relative();
}

void rhr::render::frame::push_links(std::shared_ptr<frame>& frame)
{
	if (m_link_up)
		frame->link(rhr::render::cardinal::local::UP);
	if (m_link_down)
		frame->link(rhr::render::cardinal::local::DOWN);
	if (m_link_left)
		frame->link(rhr::render::cardinal::local::LEFT);
	if (m_link_right)
		frame->link(rhr::render::cardinal::local::RIGHT);
}

void rhr::render::frame::update_mouse_button_status(bool enabled)
{
	if (!is_weak())
		return;

	if (enabled && !m_mouse_button_registered)
	{
		m_mouse_button_registered = true;
		InputHandler::RegisterMouseCallback(mouse_button_caller, this);
	}
	else if (!enabled && m_mouse_button_registered)
	{
		m_mouse_button_registered = false;
		InputHandler::UnregisterMouseCallback(mouse_button_caller);
	}
}

void rhr::render::frame::update_bars_from_relative()
{
	for (auto& bar : m_bars)
	{
		bar.absolute_position = m_position + m_super_position;

		if (m_plane == rhr::render::cardinal::plane::HORIZONTAL)
		{
			bar.absolute_position.x += bar.offset;
			bar.absolute_position.x -= static_cast<i32>(m_padding) / 2;

			bar.size = { static_cast<i32>(m_padding), m_size.y };

			if (m_link_up)
				bar.size.y -= static_cast<i32>(m_padding) / 2;
			else
				bar.size.y -= static_cast<i32>(m_padding);

			if (m_link_down)
				bar.size.y -= static_cast<i32>(m_padding) / 2;
			else
				bar.size.y -= static_cast<i32>(m_padding);

			//std::cout << bar.absolute_position.x << ", " << bar.absolute_position.y << " | " << bar.size.x << "x" << bar.size.y << std::endl;
		}
		else if (m_plane == rhr::render::cardinal::plane::VERTICAL)
		{
			bar.absolute_position.y += bar.offset;
			bar.absolute_position.y -= static_cast<i32>(m_padding) / 2;

			bar.size = { m_size.x, static_cast<i32>(m_padding) };

			if (m_link_left)
				bar.size.x -= static_cast<i32>(m_padding) / 2;
			else
				bar.size.x -= static_cast<i32>(m_padding);

			if (m_link_right)
				bar.size.x -= static_cast<i32>(m_padding) / 2;
			else
				bar.size.x -= static_cast<i32>(m_padding);
		}
	}
}

void rhr::render::frame::update_bars_from_absolute()
{
	for (auto& bar : m_bars)
	{
		if (m_plane == rhr::render::cardinal::plane::HORIZONTAL)
			bar.offset = bar.absolute_position.x - m_position.x - m_super_position.x + (static_cast<i32>(m_padding) / 2);
		else if (m_plane == rhr::render::cardinal::plane::VERTICAL)
			bar.offset = bar.absolute_position.y - m_position.y - m_super_position.y + (static_cast<i32>(m_padding) / 2);
	}
}

void rhr::render::frame::reset_drag()
{
	m_dragging = false;
	m_dragging_bar_idx = -1;
	m_dragging_mouse_begin = { 0 ,0 };
	m_dragging_object_begin = { 0, 0 };
}

void rhr::render::frame::update_background()
{
	m_background->set_super_position(m_position + m_super_position + static_cast<i32>(m_padding));
	m_background->set_size(m_size - (static_cast<i32>(m_padding) * 2));
}