#pragma once
#include "config.h"

#include "shape.hpp"
#include "macchiato/vertices/vertex_color.hpp"
#include "macchiato/vertices/vertex_texture.hpp"
#include "macchiato/objects/object_color.hpp"
#include "macchiato/objects/object_texture.hpp"

#include <latte/utils.hpp>

namespace mac
{
class shape_rectangle : public mac::shape
{
public:
	static constexpr u32 vertex_count = 4;
	static constexpr u32 index_count = 6;
	static constexpr u32 vertices_size = mac::vertex_color::info::stride_s * vertex_count;
	static constexpr u32 indices_size = sizeof(u32) * index_count;

	static constexpr glm::vec<2, i32> i_position = { 0, 0 };
	static constexpr glm::vec<2, i32> i_size = { 100, 100 };
	static constexpr glm::vec<4, f32> i_color = { 1.0f, 1.0f, 1.0f, 1.0f };

	shape_rectangle(
		mac::window::state* window_state
	)
		: mac::shape(window_state, vertices_size, vertex_count, indices_size, index_count, false)
		, position(i_position)
		, size(i_size)
		, color(i_color)
		, m_has_image(false)
		, m_image_view(nullptr)
		, m_object_color(nullptr)
		, m_object_texture(nullptr)
	{
		setup_object_color();
	}

	shape_rectangle(
		mac::window::state* window_state,
		const std::string& image_registry_name
	)
		: mac::shape(window_state, vertices_size, vertex_count, indices_size, index_count, false)
		, position(i_position)
		, size(i_size)
		, color(i_color)
		, m_has_image(true)
		, m_image_view(nullptr)
		, m_object_color(nullptr)
		, m_object_texture(nullptr)
	{
		m_image_view = mac::window::get_image(window_state, image_registry_name);

		if (m_image_view != nullptr)
			setup_object_texture();
		else
			setup_object_color();
	}

	shape_rectangle(
		mac::window::state* window_state,
		glm::vec<2, i32> position_,
		glm::vec<2, i32> size_,
		glm::vec<4, f32> color_
	)
		: mac::shape(window_state, vertices_size, vertex_count, indices_size, index_count, false)
		, position(position_)
		, size(size_)
		, color(color_)
		, m_has_image(false)
		, m_image_view(nullptr)
		, m_object_color(nullptr)
		, m_object_texture(nullptr)
	{
		setup_object_color();
	}

	shape_rectangle(
		mac::window::state* window_state,
		const std::string& image_registry_name,
		glm::vec<2, i32> position_,
		glm::vec<2, i32> size_,
		glm::vec<4, f32> color_
	)
		: mac::shape(window_state, vertices_size, vertex_count, indices_size, index_count, false)
		, position(position_)
		, size(size_)
		, color(color_)
		, m_has_image(false)
		, m_image_view(nullptr)
		, m_object_color(nullptr)
		, m_object_texture(nullptr)
	{
		m_image_view = mac::window::get_image(window_state, image_registry_name);

		if (m_image_view != nullptr)
			setup_object_texture();
		else
			setup_object_color();
	}

	glm::vec<2, i32> position;
	glm::vec<2, i32> size;
	glm::vec<4, f32> color;
private:
	///
	void setup_object_color()
	{
		m_object_color = new mac::object_color(m_window_state);
		set_object(m_object_color);

		auto pipeline_bucket_color = mac::window::get_pipeline_bucket(m_window_state, "color");

		{
			std::unique_lock lock(m_window_state->spawn_objects_mutex);
			m_window_state->spawn_objects.emplace_back(m_object_color);
		}

		{
			std::unique_lock lock(pipeline_bucket_color->shared_mutex);
			pipeline_bucket_color->objects.emplace_back(m_object_color);
		}
	}

	///
	void setup_object_texture()
	{
		m_object_texture = new mac::object_texture(m_window_state, m_image_view);
		set_object(m_object_texture);

		auto pipeline_bucket_texture = mac::window::get_pipeline_bucket(m_window_state, "texture");

		{
			std::unique_lock lock(m_window_state->spawn_objects_mutex);
			m_window_state->spawn_objects.emplace_back(m_object_texture);
		}

		{
			std::unique_lock lock(pipeline_bucket_texture->shared_mutex);
			pipeline_bucket_texture->objects.emplace_back(m_object_texture);
		}
	}

	///
	bool m_has_image;

	///
	vk::image_view m_image_view;

	///
	mac::object_color* m_object_color;

	///
	mac::object_texture* m_object_texture;
protected:
	void recalculate_buffer_data() override
	{
		// VERTICES

		auto vertices = reinterpret_cast<mac::vertex_color::vertex*>(m_vertices);
		memset(reinterpret_cast<u8*>(vertices), 0, vertices_size);

		vertices[0].color = i_color;
		vertices[1].color = i_color;
		vertices[2].color = i_color;
		vertices[3].color = i_color;

		vertices[1].position.x = static_cast<f32>(size.x);
		vertices[2].position.y = static_cast<f32>(size.y);
		vertices[3].position.x = static_cast<f32>(size.x);
		vertices[3].position.y = static_cast<f32>(size.y);

		// INDICES

		auto indices = reinterpret_cast<u32*>(m_indices);
		memset(reinterpret_cast<u8*>(indices), 0, indices_size);

		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
		indices[3] = 1;
		indices[4] = 2;
		indices[5] = 3;

		// MODEL MATRIX

		m_model_matrix = glm::translate(m_model_matrix, glm::vec<3, f32>(position, 0.0f));

		/// UBOs

		if (m_has_image)
		{
			m_object_texture->ubo_obj().model_matrix = m_model_matrix;
			m_object_texture->ubo_obj().color = color;
		}
		else
		{
			m_object_color->ubo_obj().model_matrix = m_model_matrix;
			m_object_color->ubo_obj().color = color;
		}
	}
};
}
