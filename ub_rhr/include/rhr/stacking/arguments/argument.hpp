#pragma once
#include "config.h"

#include "rhr/rendering/vertex.hpp"
#include "rhr/rendering/interfaces/i_renderable.hpp"
#include "rhr/rendering/interfaces/i_updateable.hpp"
#include "rhr/rendering/interfaces/i_positionable.hpp"

#include <cappuccino/utils.hpp>
#include <cappuccino/mod_block/block.hpp>
#include <cappuccino/color.hpp>


namespace rhr::stack::argument
{
/// Base class for visual block arguments.
class argument : public rhr::render::interfaces::i_renderable, public rhr::render::interfaces::i_updateable, public rhr::render::interfaces::i_positionable<2, i32>
{
public:
	///
	argument(const cap::color& block_color);

	/// Sets data. Data of the argument is stored as a string.
	/// \param Data to set the argument to.
	void set_data(const std::string& data);

	/// Gets data. Data of the argument is stored as a string.
	/// \return Data from the argument.
	const std::string& get_data();

	/// Sets mode. Mode can either be raw (rvalue) or var (lvalue).
	/// \param Argument variable mode.
	void set_mode(BlockArgumentVariableMode mode);

	/// Gets mode. Mode can either be raw (rvalue) or var (lvalue).
	/// \return Argument variable mode.
	BlockArgumentVariableMode get_mode();

	/// How to interpret the data.
	/// \return Type of argument.
	virtual BlockArgumentType get_type();

	/// Get width for surrounding argument spacing.
	/// \return Width of argument.
	virtual u32 get_width();

	/// Whether or not the argument contains data for storing.
	/// \return Has data.
	virtual bool has_data();

	// TODO: remove?
	/// Select the argument and enable it.
	virtual void select();

	// TODO: remove?
	/// When the focus turns off of the argument.
	virtual void unselect();

	///
	virtual bool drag_bounds(glm::vec<2, i32> position);

	///
	static i32 padding;
protected:
	/// Add draw calls to cmd buffer prebound by Renderer.
	virtual void on_render();

	/// Updates mesh on cpu side.
	virtual void on_update_buffers();

	/// Regenerates descriptor sets including uniforms.
	virtual void on_reload_swap_chain();

	/// Runs every frame before rendering.
	/// \param Seconds since last frame.
	virtual void on_frame_update(f64 delta_time);

	/// Sets data. Data of the argument is stored as a string.
	virtual void on_set_data();

	/// Data of argument stored as a string.
	std::string m_data;

	/// Mode of argument.
	BlockArgumentVariableMode m_mode;

	///
	cap::color m_block_color;
};
}
