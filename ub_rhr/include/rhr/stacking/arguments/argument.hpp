#pragma once
#include "config.h"

#include "rhr/rendering/interfaces/i_ui.hpp"
#include "rhr/rendering/vertex.hpp"

#include <cappuccino/color.hpp>
#include <cappuccino/mod/block/block.hpp>
#include <cappuccino/utils.hpp>

namespace rhr::stack::argument
{
/// Base class for visual block arguments.
class argument : public rhr::render::interfaces::i_ui
{
public:
	/// See "dev/padding.png" for information on how argument padding works.
	enum class padding_style
	{
		///
		NONE,
		SOFT,
		HARD
	};

	///
	argument(const cap::color& block_color, std::function<void()>* function_collection_update);

	/// Sets data. Data of the argument is stored as a string.
	/// \param Data to set the argument to.
	void set_data(const std::string& data);

	/// Sets mode. Mode can either be raw (rvalue) or var (lvalue).
	/// \param Argument variable mode.
	bool set_mode(cap::mod::block::block::argument::variable_mode mode);

	///
	void set_mode_restriction(cap::mod::block::block::argument::variable_mode_restriction mode_restriction);

	/// Gets mode. Mode can either be raw (rvalue) or var (lvalue).
	/// \return Argument variable mode.
	cap::mod::block::block::argument::variable_mode get_mode();

	/// Gets data. Data of the argument is stored as a string.
	/// \return Data from the argument.
	virtual const std::string& get_data();

	/// How to interpret the data.
	/// \return Type of argument.
	virtual cap::mod::block::block::argument::type get_type();

	/// Get width for surrounding argument spacing.
	/// \return Width of argument.
	virtual u32 get_width();

	/// Whether or not the argument contains data for storing.
	/// \return Has data.
	virtual bool has_data();

	///
	virtual bool drag_bounds(glm::vec<2, i32> position);

	///
	virtual padding_style get_padding_style();

	///
	static i32 padding;

protected:
	/// Sets data. Data of the argument is stored as a string.
	virtual void on_set_data();

	///
	virtual void on_set_mode(cap::mod::block::block::argument::variable_mode mode);

	/// Data of argument stored as a string.
	std::string m_data;

	/// Mode of argument.
	cap::mod::block::block::argument::variable_mode m_mode;

	///
	cap::mod::block::block::argument::variable_mode_restriction m_mode_restriction;

	///
	cap::color m_block_color;

	///
	std::function<void()>* m_function_collection_update;
};
} // namespace rhr::stack::argument
