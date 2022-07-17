#pragma once
#include "config.h"

#include "espresso/reg/type.hpp"
#include "espresso/rt_argument.hpp"
#include "espresso/block.hpp"
#include "espresso/category.hpp"

#include <latte/utils.hpp>

#define REGISTER_TYPE(name, t) auto r_t = t; esp::registry::get()->hash_types()[name] = r_t; esp::registry::get()->types().emplace_back(r_t)
#define REGISTER_ARGUMENT(name, a) auto r_a = a; esp::registry::get()->hash_arguments()[name] = r_a; esp::registry::get()->arguments().emplace_back(r_a)
#define REGISTER_RT_ARGUMENT(name, a) auto r_rta = a; esp::registry::get()->hash_rt_arguments()[name] = r_rta; esp::registry::get()->rt_arguments().emplace_back(r_rta)
#define REGISTER_BLOCK(name, b) auto r_b = b; esp::registry::get()->hash_blocks()[name] = r_b; esp::registry::get()->blocks().emplace_back(r_b)
#define REGISTER_CATEGORY(name, c) auto r_c = c; esp::registry::get()->hash_categories()[name] = r_c; esp::registry::get()->categories().emplace_back(r_c)

#define REGISTER_DEFINITION(d) esp::registry::get()->definitions().emplace_back(d)
#define REGISTER_INITIALIZATION(i) esp::registry::get()->initialization().emplace_back(i)
#define REGISTER_PARAMETER(p_sender, p_receiver) esp::registry::get()->parameters().emplace_back(std::make_pair(p_sender, p_receiver))
#define REGISTER_LIBRARY(l) esp::registry::get()->libraries().emplace_back(l)

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
	std::unordered_map<std::string, esp::reg::type*>& hash_types();

	///
	std::unordered_map<std::string, esp::argument*>& hash_arguments();

	///
	std::unordered_map<std::string, esp::rt_argument*>& hash_rt_arguments();

	///
	std::unordered_map<std::string, esp::block*>& hash_blocks();

	///
	std::unordered_map<std::string, esp::category*>& hash_categories();

	///
	std::vector<esp::reg::type*>& types();

	///
	std::vector<esp::argument*>& arguments();

	///
	std::vector<esp::rt_argument*>& rt_arguments();

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
	esp::rt_argument* get_rt_argument(const std::string& unlocalized_name);

	///
	esp::block* get_block(const char* unlocalized_name);

	///
	esp::category* get_category(const char* unlocalized_name);

private:
	///
	static registry* m_registry;

	///
	std::unordered_map<std::string, esp::reg::type*> m_hash_types;

	///
	std::unordered_map<std::string, esp::argument*> m_hash_arguments;

	///
	std::unordered_map<std::string, esp::rt_argument*> m_hash_rt_arguments;

	///
	std::unordered_map<std::string, esp::block*> m_hash_blocks;

	///
	std::unordered_map<std::string, esp::category*> m_hash_categories;

	///
	std::vector<esp::reg::type*> m_types;

	///
	std::vector<esp::argument*> m_arguments;

	///
	std::vector<esp::rt_argument*> m_rt_arguments;

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
