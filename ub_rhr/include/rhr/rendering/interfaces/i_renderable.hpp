#pragma once
#include "config.h"

#include <cappuccino/utils.hpp>

namespace rhr::render::interfaces
{
/// Interface for renderable objects.
class i_renderable
{
public:
	///
	i_renderable();

	/// Required to operate.
	void set_weak(std::weak_ptr<i_renderable>&& weak);

	/// Add draw calls to cmd buffer prebound by Renderer.
	void render();

	/// Updates mesh on cpu side.
	void update_buffers();

	/// Regenerates descriptor sets including uniforms.
	void reload_swap_chain();

	/// Gets the weak reference.
	/// \return Weak reference to itself.
	std::weak_ptr<i_renderable>& get_weak();

	/// If i_renderable::set_weak() was run then this returns true.
	bool is_weak();

protected:
	/// Add draw calls to cmd buffer prebound by Renderer.
	virtual void on_render();

	/// Updates mesh on cpu side.
	virtual void on_update_buffers();

	/// Regenerates descriptor sets including uniforms.
	virtual void on_reload_swap_chain();

	/// Called when i_renderable::set_weak is called.
	virtual void on_set_weak();

	/// Called when i_renderable::mark_dirty is called.
	virtual void on_mark_dirty();

	/// Marks i_renderable as dirty. UpdateBuffers will be called next frame.
	void mark_dirty();

	/// Clears i_renderable's dirty tag.
	void clear_dirty();

	/// Guarantees only one i_renderable::mark_dirty(); call.
	bool m_dirty;

	/// Flag if weak is set.
	bool m_weak_set;

	/// Weak reference to itself.
	std::weak_ptr<i_renderable> m_weak;
};
} // namespace rhr::render::interfaces