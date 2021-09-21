#include "mod.hpp"

// system

#include "blocks/system/main.hpp"

// utility

// graphics

// booleans

#include "blocks/booleans/and.hpp"
#include "blocks/booleans/flip.hpp"
#include "blocks/booleans/log.hpp"
#include "blocks/booleans/not.hpp"
#include "blocks/booleans/or.hpp"
#include "blocks/booleans/set.hpp"

// reals

#include "blocks/reals/abs.hpp"
#include "blocks/reals/ceil.hpp"
#include "blocks/reals/comp_eq.hpp"
#include "blocks/reals/comp_gr.hpp"
#include "blocks/reals/comp_gr_eq.hpp"
#include "blocks/reals/comp_ls.hpp"
#include "blocks/reals/comp_ls_eq.hpp"
#include "blocks/reals/cos.hpp"
#include "blocks/reals/divide.hpp"
#include "blocks/reals/eulers.hpp"
#include "blocks/reals/floor.hpp"
#include "blocks/reals/from_bool.hpp"
#include "blocks/reals/from_string.hpp"
#include "blocks/reals/log.hpp"
#include "blocks/reals/logarithmic.hpp"
#include "blocks/reals/minus.hpp"
#include "blocks/reals/mod.hpp"
#include "blocks/reals/multiply.hpp"
#include "blocks/reals/pi.hpp"
#include "blocks/reals/plus.hpp"
#include "blocks/reals/pow.hpp"
#include "blocks/reals/random.hpp"
#include "blocks/reals/round.hpp"
#include "blocks/reals/set.hpp"
#include "blocks/reals/sin.hpp"
#include "blocks/reals/tan.hpp"

// strings

#include "blocks/strings/cat_from.hpp"
#include "blocks/strings/char_at.hpp"
#include "blocks/strings/comp_eq.hpp"
#include "blocks/strings/find.hpp"
#include "blocks/strings/from_bool.hpp"
#include "blocks/strings/from_real.hpp"
#include "blocks/strings/from_real_int.hpp"
#include "blocks/strings/length.hpp"
#include "blocks/strings/log.hpp"
#include "blocks/strings/set.hpp"
#include "blocks/strings/sub_string.hpp"

UB_EXPORT void Initialization(esp::mod::data* data)
{
	/////////////////////////////////////////////////////////////////
	// mod information
	/////////////////////////////////////////////////////////////////

	data->set_mod_display_name("Vanilla Mod");
	data->set_mod_unlocalized_name("mod_vin");
	data->set_version("NULL"); // TODO: version

	/////////////////////////////////////////////////////////////////
	// categories
	/////////////////////////////////////////////////////////////////

	data->register_category(new CategorySystem(data->get_mod_unlocalized_name(), data->get_mod_display_name()));
	data->register_category(new CategoryUtility(data->get_mod_unlocalized_name(), data->get_mod_display_name()));
	data->register_category(new CategoryGraphics(data->get_mod_unlocalized_name(), data->get_mod_display_name()));
	data->register_category(new CategoryReals(data->get_mod_unlocalized_name(), data->get_mod_display_name()));
	data->register_category(new CategoryStrings(data->get_mod_unlocalized_name(), data->get_mod_display_name()));
	data->register_category(new CategoryBooleans(data->get_mod_unlocalized_name(), data->get_mod_display_name()));

	/////////////////////////////////////////////////////////////////
	// Blocks
	/////////////////////////////////////////////////////////////////

	// system

	data->register_block(new mod_vin::block::system::main());

	// utility

	// graphics

	// booleans

	data->register_block(new mod_vin::block::boolean::and_());
	data->register_block(new mod_vin::block::boolean::flip());
	data->register_block(new mod_vin::block::boolean::log());
	data->register_block(new mod_vin::block::boolean::not_());
	data->register_block(new mod_vin::block::boolean::or_());
	data->register_block(new mod_vin::block::boolean::set());

	// reals

	data->register_block(new mod_vin::block::real::abs());
	data->register_block(new mod_vin::block::real::ceil());
	data->register_block(new mod_vin::block::real::comp_eq());
	data->register_block(new mod_vin::block::real::comp_gr());
	data->register_block(new mod_vin::block::real::comp_gr_eq());
	data->register_block(new mod_vin::block::real::comp_ls());
	data->register_block(new mod_vin::block::real::comp_ls_eq());
	data->register_block(new mod_vin::block::real::cos());
	data->register_block(new mod_vin::block::real::divide());
	data->register_block(new mod_vin::block::real::eulers());
	data->register_block(new mod_vin::block::real::floor());
	data->register_block(new mod_vin::block::real::from_bool());
	data->register_block(new mod_vin::block::real::from_string());
	data->register_block(new mod_vin::block::real::log());
	data->register_block(new mod_vin::block::real::logarithmic());
	data->register_block(new mod_vin::block::real::minus());
	data->register_block(new mod_vin::block::real::mod());
	data->register_block(new mod_vin::block::real::multiply());
	data->register_block(new mod_vin::block::real::pi());
	data->register_block(new mod_vin::block::real::plus());
	data->register_block(new mod_vin::block::real::pow());
	data->register_block(new mod_vin::block::real::random());
	data->register_block(new mod_vin::block::real::round());
	data->register_block(new mod_vin::block::real::set());
	data->register_block(new mod_vin::block::real::sin());
	data->register_block(new mod_vin::block::real::tan());

	// strings

	data->register_block(new mod_vin::block::string::cat_from());
	data->register_block(new mod_vin::block::string::char_at());
	data->register_block(new mod_vin::block::string::comp_eq());
	data->register_block(new mod_vin::block::string::find());
	data->register_block(new mod_vin::block::string::from_bool());
	data->register_block(new mod_vin::block::string::from_real());
	data->register_block(new mod_vin::block::string::from_real_int());
	data->register_block(new mod_vin::block::string::length());
	data->register_block(new mod_vin::block::string::log());
	data->register_block(new mod_vin::block::string::set());
	data->register_block(new mod_vin::block::string::sub_string());
}
