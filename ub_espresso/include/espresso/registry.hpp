#pragma once
#include "config.h"

#include "espresso/reg/type.hpp"
#include "espresso/argument.hpp"
#include "espresso/block.hpp"
#include "espresso/category.hpp"

#include <latte/utils.hpp>

#define REGISTER_TYPE(t) registry->types().emplace_back(reinterpret_cast<esp::reg::type*>(t))
#define REGISTER_ARGUMENT(a) registry->arguments().emplace_back(reinterpret_cast<esp::argument*>(a))
#define REGISTER_BLOCK(b) registry->blocks().emplace_back(reinterpret_cast<esp::block*>(b))
#define REGISTER_CATEGORY(c) registry->categories().emplace_back(reinterpret_cast<esp::category*>(c))

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
};
}
