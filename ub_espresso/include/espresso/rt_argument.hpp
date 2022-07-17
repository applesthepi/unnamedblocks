#pragma once
#include "config.h"

#include "espresso/reg/type.hpp"
#include "espresso/color.hpp"
#include "espresso/argument.hpp"

#include <latte/utils.hpp>
#include <latte/serializer.hpp>

#include <macchiato/interfaces/i_ui.hpp>

namespace esp
{
/// The esp::argument is used in mods to show how the argument would react. Do not store extra data in the class, the
/// class will be used for all argument fields in the editor. Treat this class as a group of functional code rather
/// then an actual object. If you need to store data, you can set the void* to a pointer of data that you constructed
/// on the heap yourself (you can make a custom struct if you want). Dont forget to deallocate in the proper function.
class rt_argument
{
public:
	///
	static i32 padding;

	///
	struct state
	{
		state();

		///
		mac::i_ui* parent;

		///
		glm::vec<2, i32>* plane_offset;

		///
		std::function<void()>* function_collection_update;

		///
		espresso::color block_color;

		///
		esp::argument::mode mode;

		///
		esp::argument::mode_restriction mode_restriction;

		///
		void* custom;
	};

	/// IMPORTANT! The custom field is a default template from esp::argument::get_initializer, NOT the
	/// instance that you should instantiate in esp::argument::create. This struct is used to initialize
	/// the esp::argument::state (other then custom).
	struct initializer
	{
		///
		esp::argument* argument;

		///
		esp::argument::mode mode;

		///
		esp::argument::mode_restriction mode_restriction;

		///
		void* custom;
	};

	/// IMPORTANT! The create function will be called BEFORE the deserialization call, this means that some data
	/// will be INVALID and will change once the deserialization has been invoked. Called when an
	/// argument::state has been created and ready for the custom argument to have access to it.
	/// You can set the argument::state::custom if you want, or do any initialization needed on your end.
	/// The state's main info has already been set from the initializer except for custom, you would set that
	/// using the initializer custom.
	virtual void create(mac::window::state* window_state, rt_argument::state* state, rt_argument::initializer* initializer) = 0;

	/// Don't deallocate the argument::state. If you allocated something for argument::state::custom, this is the
	/// place to deallocate that.
	virtual void destroy(rt_argument::state* state) = 0;

	///
	virtual void setup_initializer(esp::rt_argument::initializer& initializer, latte::serializer::node& storage) = 0;

	/// Name for registry storage. Convention example: "mymod_some_arg_name".
	virtual const char* get_unlocalized_name() = 0;

	///
	virtual std::string get_ubbs_value(esp::rt_argument::state* state) = 0;

	/// If you want this argument in the block to store information in ubbs, then set this to a valid type from
	/// the esp::registry. Otherwise, just return nullptr.
	/// \return Type of argument.
	virtual esp::reg::type* get_type() = 0;

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
	virtual u32 get_width(rt_argument::state* state) = 0;

	///
	virtual bool capture_mouse() = 0;

	/// Called after any set transform related functions get called during frame update.
	virtual void ui_transform_update(rt_argument::state* state, mac::i_ui::transform_update_spec transform_update_spec) = 0;

	///
	virtual void ui_frame_update(rt_argument::state* state, f64 delta_time) = 0;

	///
	virtual void ui_render(rt_argument::state* state) = 0;

	///
	virtual void ui_reload_swap_chain(rt_argument::state* state) = 0;

	///
	virtual void ui_update_buffers(rt_argument::state* state) = 0;

	///
	virtual void ui_chain_update_buffers(rt_argument::state* state) = 0;

	///
	virtual void ui_static_offset_update(rt_argument::state* state) = 0;

	///
	virtual void ui_serialize(rt_argument::state* state, latte::serializer::node& node) = 0;

	///
	virtual void ui_deserialize(rt_argument::state* state, latte::serializer::node& node) = 0;

	///
	virtual void on_set_mode(rt_argument::state* state) = 0;
protected:
	///
	void toggle_mode(rt_argument::state* state);

	///
	bool check_bounds(rt_argument::state* state, glm::vec<2, i32> position);
};
}