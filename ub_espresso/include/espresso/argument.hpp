#pragma once
#include "config.h"

#include "espresso/reg/type.hpp"
#include "espresso/color.hpp"

#include <latte/utils.hpp>
#include <lungo/interfaces/i_ui.hpp>

namespace esp
{
/// The esp::argument is used in mods to show how the argument would react. Do not store extra data in the class, the
/// class will be used for all argument fields in the editor. Treat this class as a group of functional code rather
/// then an actual object. If you need to store data, you can set the void* to a pointer of data that you constructed
/// on the heap yourself (you can make a custom struct if you want). Dont forget to deallocate in the proper function.
class argument
{
public:
	///
	static i32 padding;

	/// Restricts the argument::mode.
	enum class mode_restriction
	{
		/// User can freely change between the argument::mode.
		NONE,

		/// User can not change the argument::mode, it is stuck on whatever it was set to by default.
		RESTRICTED
	};

	/// This is the default assignment of whether it starts as var or raw input.
	enum class mode
	{
		/// If the argument can be set to var given the argument::mode_restriction, then the argument must
		/// return a valid esp::reg::type* so that ubbs can give a physical type that stores the information.
		VAR,

		/// Static data that ubbs will feed directly into code.
		RAW
	};

	/// See "dev/padding.png" for information on how argument padding works.
	enum class padding_style
	{
		NONE,
		SOFT,
		HARD
	};

	///
	struct state
	{
		state();

		///
		rhr::render::interfaces::i_ui* parent;

		///
		glm::vec<2, i32>* plane_offset;

		///
		std::function<void()>* function_collection_update;

		///
		espresso::color block_color;

		///
		argument::mode mode;

		///
		argument::mode_restriction mode_restriction;

		///
		void* custom;
	};

	/// IMPORTANT! The custom field is a default template from esp::argument::get_initializer, NOT the
	/// instance that you should instantiate in esp::argument::create. This struct is used to initialize
	/// the esp::argument::state (other then custom).
	struct initializer
	{
		///
		argument* argument;

		///
		argument::mode mode;

		///
		argument::mode_restriction mode_restriction;

		///
		void* custom;
	};

	/// IMPORTANT! The create function will be called BEFORE the deserialization call, this means that some data
	/// will be INVALID and will change once the deserialization has been invoked. Called when an
	/// argument::state has been created and ready for the custom argument to have access to it.
	/// You can set the argument::state::custom if you want, or do any initialization needed on your end.
	/// The state's main info has already been set from the initializer except for custom, you would set that
	/// using the initializer custom.
	virtual void create(argument::state* state, argument::initializer* initializer) = 0;

	/// Don't deallocate the argument::state. If you allocated something for argument::state::custom, this is the
	/// place to deallocate that.
	virtual void destroy(argument::state* state) = 0;

	/// Name for registry storage. Convention example: "mymod_some_arg_name".
	virtual const char* get_unlocalized_name() = 0;

	/// If you want this argument in the block to store information in ubbs, then set this to a valid type from
	/// the esp::registry. Otherwise, just return nullptr.
	/// \return Type of argument.
	[[nodiscard]] virtual esp::reg::type* get_type() = 0;

	/// If you want similar arguments to cast into this block argument, then return which types you want. For
	/// example, if you have a "u8" type, you allow "real" type to cast.
	/// \param valid_types Valid casting types.
	virtual void get_valid_casts(std::vector<esp::reg::type*>& valid_types) = 0;

	/// Padding style for spacing multiple arguments within a block.
	/// \return Padding style.
	virtual argument::padding_style get_padding_style() = 0;

	/// Mode that the argument will have by default.
	/// \return Default mode.
	virtual argument::mode get_mode() = 0;

	/// Restriction of the mode. The user can not change this in the editor.
	/// \return Mode restriction.
	virtual argument::mode_restriction get_mode_restriction() = 0;

	/// Get width for surrounding argument spacing.
	/// \return Width of argument.
	virtual u32 get_width(argument::state* state) = 0;

	/// Called after any set transform related functions get called during frame update.
	virtual void ui_transform_update(argument::state* state, rhr::render::interfaces::i_ui::transform_update_spec transform_update_spec) = 0;

	///
	virtual void ui_frame_update(argument::state* state, f64 delta_time) = 0;

	///
	virtual void ui_render(argument::state* state) = 0;

	///
	virtual void ui_reload_swap_chain(argument::state* state) = 0;

	///
	virtual void ui_update_buffers(argument::state* state) = 0;

	///
	virtual void ui_chain_update_buffers(argument::state* state) = 0;

	///
	virtual void ui_static_offset_update(argument::state* state) = 0;

	///
	virtual void ui_serialize(argument::state* state, latte::serializer::node& node) = 0;

	///
	virtual void ui_deserialize(argument::state* state, latte::serializer::node& node) = 0;

	///
	virtual void on_set_mode(argument::state* state) = 0;
protected:
	///
	void toggle_mode(argument::state* state);
};
}