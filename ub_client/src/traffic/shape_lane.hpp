#pragma once
#include "config.h"

#include "latte/logger.hpp"
#include "utils.hpp"
#include "macchiato/shapes/shape.hpp"
#include "macchiato/vertices/vertex_color.hpp"
#include "macchiato/vertices/vertex_texture.hpp"
#include "macchiato/objects/object_color.hpp"
#include "macchiato/objects/object_texture.hpp"

#include <latte/utils.hpp>
#include <thread>

namespace traffic
{
class shape_lane : public mac::shape
{
public:
	static constexpr u32 points = 5;
	static constexpr f32 width = 20.0f;

	static constexpr u32 vertex_count = points * 2;
	static constexpr u32 index_count = 6 * (points - 1);
	static constexpr u32 vertices_size = mac::vertex_color::info::stride_s * vertex_count;
	static constexpr u32 indices_size = sizeof(u32) * index_count;

	static constexpr glm::vec<4, f32> i_color = { 1.0f, 1.0f, 1.0f, 1.0f };

	shape_lane(
		mac::window::state* window_state,
		glm::vec<2, f32> p1_,
		glm::vec<2, f32> p2_,
		glm::vec<2, f32> p3_,
		glm::vec<2, f32> p4_
	)
		: mac::shape(window_state, vertices_size, vertex_count, indices_size, index_count, false)
		, p1(p1_)
		, p2(p2_)
		, p3(p3_)
		, p4(p4_)
		, m_object_color(nullptr)
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

	glm::vec<2, f32> p1, p2, p3, p4;
private:
	///
	mac::object_color* m_object_color;
protected:
	void recalculate_buffer_data() override
	{
		// VERTICES
		
		auto vertices = reinterpret_cast<mac::vertex_color::vertex*>(m_vertices);
		memset(reinterpret_cast<u8*>(vertices), 0, vertices_size);

		// INDICES

		auto indices = reinterpret_cast<u32*>(m_indices);
        memset(reinterpret_cast<u8*>(indices), 0, indices_size);

		// SIDE MATRIX
#if 0
		auto matrix = glm::lookAt(glm::vec<3, f32>(p1, 0.0f), glm::vec<3, f32>(p2, 0.0f), { 0.0f, 0.0f, 1.0f });
		matrix = glm::translate(matrix, { 10.0f, 0.0f, 0.0f });

		glm::vec<4, f32> v4_side_offset = matrix * glm::vec<4, f32>(p1, 0.0f, 1.0f);
		glm::vec<2, f32> v_side_offset  = { v4_side_offset.x, v4_side_offset.z };

		LOG_DEBUG_VEC2(p1);
		LOG_DEBUG_VEC2(v_side_offset);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
#endif
		// POINTS

		std::vector<glm::vec<2, f32>> bp_left;
		bp_left.reserve(points);
		bezier_sample_points(p1, p2, p3, p4, points, bp_left);

		auto function_left_point = [](glm::vec<2, f32> base, glm::vec<2, f32> delta)
		{
			auto m_rot = glm::lookAt(
				glm::vec<3, f32>(0.0f),
				glm::normalize(glm::vec<3, f32>(delta, 0.0f)),
				{ 0.0f, 0.0f, -1.0f }
			);
			auto m_trans_a = glm::translate(glm::mat4(1.0f), { base.y, 0.0f, base.x });
			auto m_trans_b = glm::translate(glm::mat4(1.0f), { -width, 0.0f, 0.0f });

			glm::vec<4, f32> v4_side_offset = m_trans_a * m_rot * m_trans_b * glm::vec<4, f32>(0.0f, 0.0f, 0.0f, 1.0f);
			return glm::vec<2, f32>(v4_side_offset.z, v4_side_offset.x);
		};

		std::vector<glm::vec<2, f32>> bp_right;

		//bp_right.emplace_back(function_left_point(bp_left[0], bp_left[1] - bp_left[0]));
		bp_right.emplace_back(function_left_point(bp_left[0], p2 - p1));
		for (u16 i = 1; i < bp_left.size() - 1; i++)
			bp_right.emplace_back(function_left_point(bp_left[i], bp_left[i] - bp_left[i - 1]));
		bp_right.emplace_back(function_left_point(bp_left.back(), p4 - p3));

		//LOG_DEBUG_VEC2((bp_left[bp_left.size() - 1] - bp_left[bp_left.size() - 2]));
		//LOG_DEBUG_VEC2(bp_right[bp_right.size() - 2]);
		//LOG_DEBUG_VEC2(bp_right[bp_right.size() - 1]);
#if 1
		std::vector<std::string> messages;

		messages.emplace_back("LEFT:");

		for (auto& bp : bp_left)
			messages.emplace_back(std::to_string(bp.x) + ", " + std::to_string(bp.y));

		messages.emplace_back("RIGHT:");

		for (auto& bp : bp_right)
			messages.emplace_back(std::to_string(bp.x) + ", " + std::to_string(bp.y));

		latte::logger::info(latte::logger::level::SYSTEM, messages);
#endif	


		// VERTICES

		for (u16 i = 0; i < static_cast<u16>(points); i++)
		{
			vertices[i * 2].color    = { 1.0f, 1.0f, 1.0f, 1.0f };
			vertices[i * 2].position = glm::vec<3, f32>(bp_left[i].x, bp_left[i].y, 0.0f);
		}
		
		for (u16 i = 0; i < static_cast<u16>(points); i++)
		{
			vertices[i * 2 + 1].color    = { 1.0f, 1.0f, 1.0f, 1.0f };
			vertices[i * 2 + 1].position = glm::vec<3, f32>(bp_right[i].x, bp_right[i].y, 0.0f);
		}
		
		//vertices[points * 2].color    = { 1.0f, 1.0f, 1.0f, 1.0f };
		//vertices[points * 2].position = glm::vec<3, f32>(bp_right.back().x, bp_right.back().y, 0.0f);

		// INDICES

		for (u16 i = 0; i < static_cast<u16>(points) - 1; i++)
		{
			indices[i * 6 + 0] = i * 2 + 0;
			indices[i * 6 + 1] = i * 2 + 2;
			indices[i * 6 + 2] = i * 2 + 3;
			indices[i * 6 + 3] = i * 2 + 0;
			indices[i * 6 + 4] = i * 2 + 3;
			indices[i * 6 + 5] = i * 2 + 1;
		}

		//indices[(points - 1) * 6 + 0] = (points - 2) * 2 + 2;
		//indices[(points - 1) * 6 + 1] = (points - 2) * 2 + 4;
		//indices[(points - 1) * 6 + 2] = (points - 2) * 2 + 3;

		// MODEL MATRIX
        
		m_model_matrix = glm::translate(m_model_matrix, glm::vec<3, f32>(0.0f));

		/// UBOs

		m_object_color->ubo_obj().model_matrix = m_model_matrix;
		m_object_color->ubo_obj().color = i_color;
	}
};
}
