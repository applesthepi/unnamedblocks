#include "shape.hpp"

mac::shape::shape(
	mac::window::state* window_state,
	u32 lock_vertex_size, u32 vertex_count,
	u32 lock_index_size, u32 index_count,
	bool persistent_model_matrix
)
	: m_window_state(window_state)
	, m_vertices(nullptr)
	, m_indices(nullptr)
	, m_vertex_count(vertex_count)
	, m_index_count(index_count)
	, m_model_matrix(glm::mat4(1.0f))
	, m_lock_vertex_size(lock_vertex_size)
	, m_lock_index_size(lock_index_size)
	, m_persistent_model_matrix(persistent_model_matrix)
	, m_object(nullptr)
{
	if (lock_vertex_size)
		m_vertices = malloc(lock_vertex_size);
	if (lock_index_size)
		m_indices = malloc(lock_index_size);
}

void mac::shape::update()
{
	if (m_lock_vertex_size)
		memset(reinterpret_cast<u8*>(m_vertices), 0, m_lock_vertex_size);
	if (m_lock_index_size)
		memset(reinterpret_cast<u8*>(m_indices), 0, m_lock_index_size);
	if (!m_persistent_model_matrix)
		m_model_matrix = glm::mat4(1.0f);

	recalculate_buffer_data();
	m_object->set_data(m_vertices, m_vertex_count, m_indices, m_index_count);
}

void mac::shape::set_object(mac::object* object)
{
	m_object = object;
}
