#pragma once
#include "config.h"

#include "rhr/rendering/vertex.hpp"
#include "rhr/rendering/interfaces/i_renderable.hpp"
#include "rhr/rendering/interfaces/i_enableable.hpp"
#include "rhr/rendering/interfaces/i_positionable.hpp"

#include <cappuccino/utils.hpp>

#define RENDER_OBJECT_ALLOC_FACTOR 2

namespace rhr::render::object
{
/// Basic object class for rendering custom geometry.
class object : public rhr::render::interfaces::i_renderable, public rhr::render::interfaces::i_enableable, public rhr::render::interfaces::i_positionable<3, f64>
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
	void set_texture(const std::string& texture);

	///
	void set_texture(texture_type type);

	///
	void update_vertices(const std::vector<Vertex>* vertices, const std::vector<u32>* indices, bool update_buffers_now = false);

	///
	void update_vertices(const Vertex* vertices, u32 vertex_count, const u32* indices, u32 index_count, bool update_buffers_now = false);

	///
	void update_vertices(const Vertex* vertices, u32 vertex_count, bool update_buffers_now = false);

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
	Vertex* m_vertices;
	u32* m_indices;

	/// Vertex buffer data.
	vk::buffer m_vertex_buffer;
	vk::device_memory m_vertex_buffer_memory;
	vk::buffer m_vertex_staging_buffer;
	vk::device_memory m_vertex_staging_buffer_memory;

	/// Index buffer data.
	vk::buffer m_index_buffer;
	vk::device_memory m_index_buffer_memory;
	vk::buffer m_index_staging_buffer;
	vk::device_memory m_index_staging_buffer_memory;

	/// Uniform buffer data.
	vk::buffer m_uniform_buffer;
	vk::device_memory m_uniform_buffer_memory;

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
};
}