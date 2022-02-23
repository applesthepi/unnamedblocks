#pragma once
#include "config.h"

#include "mocha/registries/char_texture.hpp"
#include "lungo/interfaces/i_enableable.hpp"
#include "lungo/interfaces/i_positionable.hpp"
#include "lungo/interfaces/i_renderable.hpp"
#include "lungo/vertex.hpp"

#include <latte/utils.hpp>
#include <vk_mem_alloc.h>

#define RENDER_OBJECT_ALLOC_FACTOR 2

namespace rhr::render::object
{
/// Basic object class for rendering custom geometry.
class object
	: public rhr::render::interfaces::i_renderable
	, public rhr::render::interfaces::i_enableable
	, public rhr::render::interfaces::i_positionable<3, f64>
{
public:
	///
	enum class texture_type
	{
		CUSTOM,
		TEXT_SHEET
	};

	///
	object(bool ui);

	///
	object(bool ui, const std::string& texture_path);

	///
	void set_offset(glm::vec<2, i32>* offset);

	///
	void set_texture(const std::string& texture);

	///
	void set_texture_char(rhr::registry::char_texture::texture_type texture_type, u16 font_size);

	///
	void set_texture_char_size(u16 font_size);

	///
	void update_vertices(
		const std::vector<rhr::render::vertex>* vertices,
		const std::vector<u32>* indices,
		bool update_buffers_now = false);

	///
	void update_vertices(
		const rhr::render::vertex* vertices,
		u32 vertex_count,
		const u32* indices,
		u32 index_count,
		bool update_buffers_now = false);

	///
	void update_vertices(const rhr::render::vertex* vertices, u32 vertex_count, bool update_buffers_now = false);

	///
	void set_queue(u8 queue);

	/// Add draw calls to cmd buffer prebound by Renderer.
	void on_render() override;

	/// Updates mesh on cpu side.
	void on_update_buffers() override;

	/// Regenerates descriptor sets including uniforms.
	void on_reload_swap_chain() override;

	///
	void post_position_update() override;

private:
	///
	void update_uniforms(bool ui);

	///
	bool m_has_vertices;

	///
	bool m_has_indices;

	///
	usize m_vertex_count;
	usize m_index_count;

	///
	usize m_vertex_alloc_count;
	usize m_index_alloc_count;

	/// Arrays of data in ram.
	rhr::render::vertex* m_vertices;
	u32* m_indices;

	/// rhr::render::vertex buffer data.
	vk::buffer m_vertex_buffer;
	VmaAllocation m_vertex_buffer_allocation;
	vk::buffer m_vertex_staging_buffer;
	VmaAllocation m_vertex_staging_buffer_allocation;

	/// Index buffer data.
	vk::buffer m_index_buffer;
	VmaAllocation m_index_buffer_allocation;
	vk::buffer m_index_staging_buffer;
	VmaAllocation m_index_staging_buffer_allocation;

	/// Uniform buffer data.
	vk::buffer m_uniform_buffer;
	VmaAllocation m_uniform_allocation;

	/// Image buffer data.
	vk::image m_image;
	vk::image_view m_image_view;
	vk::device_memory m_image_memory;

	///
	vk::descriptor_set m_descriptor_set;

	///
	u8 m_queue;

	///
	bool m_dirty;

	///
	bool m_init_image;

	///
	bool m_ui;

	///
	bool m_has_texture;

	///
	std::string m_texture_path;

	///
	texture_type m_texture_type;

	///
	u16 m_font_size;

	///
	rhr::registry::char_texture::texture_type m_char_texture_type;

	///
	glm::vec<2, i32>* m_offset;
};
} // namespace rhr::render::object