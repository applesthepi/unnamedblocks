#pragma once
#include "config.h"

#include "espresso/reg/type.hpp"
#include "espresso/argument.hpp"
#include "espresso/block.hpp"
#include "espresso/category.hpp"

#include <latte/utils.hpp>

#define REGISTER_TYPE(t) registry->types().emplace_back(new esp::reg::type(t))
#define REGISTER_ARGUMENT(a) registry->arguments().emplace_back(reinterpret_cast<esp::argument*>(a))
#define REGISTER_BLOCK(b) registry->blocks().emplace_back(reinterpret_cast<esp::block*>(b))->mod_unlocalized_name = MOD_UNLOCALIZED
#define REGISTER_CATEGORY(c) registry->categories().emplace_back(reinterpret_cast<esp::category*>(c))

#define REGISTER_DEFINITION(d) registry->definitions().emplace_back(d)
#define REGISTER_INITIALIZATION(i) registry->initialization().emplace_back(i)
#define REGISTER_PARAMETER(p_sender, p_receiver) registry->parameters().emplace_back(std::make_pair(p_sender, p_receiver))
#define REGISTER_LIBRARY(l) registry->libraries().emplace_back(l)

namespace esp
{
/// Registry for modded classes to be used in the editor.
class registry
{
public:
	///
	static registry* get();

	///
	static void set(registry* reg);

	///
	std::vector<esp::reg::type*>& types();

	///
	std::vector<esp::argument*>& arguments();

	///
	std::vector<esp::block*>& blocks();

	///
	std::vector<esp::category*>& categories();

	///
	std::vector<const char*>& definitions();

	///
	std::vector<const char*>& initialization();

	///
	std::vector<std::pair<const char*, const char*>>& parameters();

	///
	std::vector<const char*>& libraries();

	///
	esp::reg::type* get_type(const char* unlocalized_name);

	///
	void get_types(const char** unlocalized_names, usize count, std::vector<esp::reg::type*>& types);

	///
	void get_types(const char* unlocalized_name, std::vector<esp::reg::type*>& types);

	///
	esp::argument* get_argument(const std::string& unlocalized_name);

	///
	esp::block* get_block(const char* unlocalized_name);

	///
	esp::category* get_category(const char* unlocalized_name);

private:
	///
	static registry* m_registry;

	///
	std::vector<esp::reg::type*> m_types;

	///
	std::vector<esp::argument*> m_arguments;

	///
	std::vector<esp::block*> m_blocks;

	///
	std::vector<esp::category*> m_categories;

	/// Declared at the top of the rust file. Usually used for defining structs to instantiate for initialization.
	std::vector<const char*> m_definitions;

	/// Put in the main method of the rust code before the main stack is called. Used for either initializing definitions
	/// or setting up other external libraries.
	std::vector<const char*> m_initialization;

	/// Carried to all stacks; Expected variable to exist in the main method from initialization.
	std::vector<std::pair<const char*, const char*>> m_parameters;

	/// Libraries added to the Cargo.toml of the rust program. Add the imports ("use") in imports.
	std::vector<const char*> m_libraries;
};
}
