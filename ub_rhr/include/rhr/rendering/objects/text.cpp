#include "text.hpp"

#include "rhr/rendering/renderer.hpp"
#include "rhr/rendering/vertex.hpp"
#include "rhr/stacking/block.hpp"
#include "rhr/registries/char_texture.hpp"

rhr::render::object::text::text()
	: i_dicolorable(cap::color().from_normalized({ 0.0f, 0.0f, 0.0f, 1.0f }), cap::color().from_u8({ 25, 25, 25, 255 }))
	, i_enableable(true)
	, m_depth(10)
	, m_render_object_background(std::make_shared<rhr::render::object::object>(true))
	, m_render_object_text(std::make_shared<rhr::render::object::object>(true))
	, m_enable_background(true)
{
	m_render_object_background->set_weak(m_render_object_background);
	m_render_object_text->set_weak(m_render_object_text);
	m_render_object_text->set_texture(rhr::render::object::object::texture_type::TEXT_SHEET);
}

void rhr::render::object::text::set_text(const std::string& text)
{
	if (text.size() == 0)
	{
		m_text.clear();
		m_render_object_text->set_enabled(false);
		m_size = { 10, Block::Height - (Block::Padding * 2) };
	}
	else
	{
		m_text = text;
		m_render_object_text->set_enabled(true);
		update_size();
	}

	mark_dirty();
}

void rhr::render::object::text::set_depth(i32 depth)
{
	m_depth = depth;
	mark_dirty();
}

void rhr::render::object::text::set_padding(i32 padding)
{
	m_padding = padding;
	update_size();
	mark_dirty();
}

void rhr::render::object::text::enable_background(bool enable)
{
	m_enable_background = enable;
}

void rhr::render::object::text::update_size()
{
	i32 running_x = m_padding;

	for (usize i = 0; i < m_text.size(); i++)
		running_x += rhr::registries::char_texture::GetCharTextureCoords(m_text[i]).Advance.x >> 6;
	
	m_size = { running_x + m_padding, Block::Height - (Block::Padding * 2) };
}

void rhr::render::object::text::on_render()
{
	if (m_enabled)
	{
		if (m_enable_background)
		{
			vkCmdBindPipeline(Renderer::ActiveCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, Renderer::UIPipeline);
			m_render_object_background->render();
		}

		vkCmdBindPipeline(Renderer::ActiveCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, Renderer::UITexturePipeline);
		m_render_object_text->render();
	}
}

void rhr::render::object::text::on_update_buffers()
{
	clear_dirty();

	{
		Vertex vertices[4];
		u32 indices[6];

		vertices[0] = Vertex({ 0.0f, 0.0f, 0.0f }, m_color_secondary.get_normalized(), { 0.0f, 0.0f });
		vertices[1] = Vertex({ static_cast<float>(m_size.x), 0.0f, 0.0f }, m_color_secondary.get_normalized(), { 1.0f, 0.0f });
		vertices[2] = Vertex({ static_cast<float>(m_size.x), static_cast<float>(m_size.y), 0.0f }, m_color_secondary.get_normalized(), { 1.0f, 1.0f });
		vertices[3] = Vertex({ 0.0f, static_cast<float>(m_size.y), 0.0f }, m_color_secondary.get_normalized(), { 0.0f, 1.0f });

		indices[0] = 1;
		indices[1] = 0;
		indices[2] = 2;
		indices[3] = 2;
		indices[4] = 0;
		indices[5] = 3;

		m_render_object_background->update_vertices(vertices, 4, indices, 6, true);
	}

	if (m_render_object_text->get_enabled())
	{
		Vertex* vertices = (Vertex*)alloca(sizeof(Vertex) * m_text.size() * 4);
		u32* indices = (u32*)alloca(sizeof(u32) * m_text.size() * 6);

		int32_t running_x = m_padding;
		glm::vec<3, float> primary_color = { m_color_primary.get_normalized().r, m_color_primary.get_normalized().g, m_color_primary.get_normalized().b };

		for (size_t i = 0; i < m_text.size(); i++)
		{
			rhr::registries::char_texture::CharTextureData char_data = rhr::registries::char_texture::GetCharTextureCoords(m_text[i]);
			float y_offset = static_cast<float>(Block::HeightContent) + (-1.0f * static_cast<float>(char_data.Offset.y)) - static_cast<float>(Block::Padding);

			vertices[i * 4 + 0] = Vertex({ static_cast<float>(running_x + char_data.Offset.x), y_offset, 0.0f }, primary_color, { char_data.First.x, char_data.First.y });
			vertices[i * 4 + 1] = Vertex({ static_cast<float>(running_x + char_data.Offset.x + char_data.Size.x), y_offset, 0.0f }, primary_color, { char_data.Second.x, char_data.First.y });
			vertices[i * 4 + 2] = Vertex({ static_cast<float>(running_x + char_data.Offset.x + char_data.Size.x), static_cast<float>(char_data.Size.y) + y_offset, 0.0f }, primary_color, { char_data.Second.x, char_data.Second.y });
			vertices[i * 4 + 3] = Vertex({ static_cast<float>(running_x + char_data.Offset.x), static_cast<float>(char_data.Size.y) + y_offset, 0.0f }, primary_color, { char_data.First.x, char_data.Second.y });

			indices[i * 6 + 0] = i * 4 + 1;
			indices[i * 6 + 1] = i * 4 + 0;
			indices[i * 6 + 2] = i * 4 + 2;
			indices[i * 6 + 3] = i * 4 + 2;
			indices[i * 6 + 4] = i * 4 + 0;
			indices[i * 6 + 5] = i * 4 + 3;

			running_x += char_data.Advance.x >> 6;
		}

		m_render_object_text->update_vertices(vertices, m_text.size() * 4, indices, m_text.size() * 6, true);
		m_size = { running_x + m_padding, Block::Height - (Block::Padding * 2) };
	}
}

void rhr::render::object::text::on_reload_swap_chain()
{
	m_render_object_background->reload_swap_chain();
	m_render_object_text->reload_swap_chain();
}

void rhr::render::object::text::post_position_update()
{
	glm::vec<2, i32> position = m_position + m_super_position;
	m_render_object_background->set_super_position({static_cast<f64>(position.x), static_cast<f64>(position.y), static_cast<f64>(m_depth) });
	m_render_object_text->set_super_position({ static_cast<f64>(position.x), static_cast<f64>(position.y), static_cast<f64>(m_depth) - 0.1 });

	mark_dirty();
}

void rhr::render::object::text::post_size_update()
{
	mark_dirty();
}
