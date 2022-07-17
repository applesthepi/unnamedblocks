#pragma once
#include "config.h"

#include "macchiato/window.hpp"

#include <latte/utils.hpp>

namespace mac
{
class shape
{
public:
	shape(
		mac::window::state* window_state,
		u32 lock_vertex_size, u32 vertex_count,
		u32 lock_index_size, u32 index_count,
		bool persistent_model_matrix
	);

	///
	void update();

	///
	virtual void set_position(glm::vec<2, f32> position);
protected:
	///
	void set_object(mac::object* object);

	///
	virtual void recalculate_buffer_data() = 0;

	///
	mac::window::state* m_window_state;

	///
	void* m_vertices;
	void* m_indices;
	u32 m_vertex_count;
	u32 m_index_count;

	///
	glm::mat4 m_model_matrix;
private:
	///
	u32 m_lock_vertex_size;
	u32 m_lock_index_size;

	///
	bool m_persistent_model_matrix;

	///
	mac::object* m_object;
};
}