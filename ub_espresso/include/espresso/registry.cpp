#include "registry.hpp"

esp::registry* esp::registry::get()
{
	return m_registry;
}

void esp::registry::set(esp::registry* reg)
{
	m_registry = reg;
}

std::vector<esp::reg::type*>& esp::registry::types()
{
	return m_types;
}

std::vector<esp::argument*>& esp::registry::arguments()
{
	return m_arguments;
}

std::vector<esp::block*>& esp::registry::blocks()
{
	return m_blocks;
}

std::vector<esp::category*>& esp::registry::categories()
{
	return m_categories;
}

esp::reg::type* esp::registry::get_type(const char* unlocalized_name)
{
	for (auto type : m_types)
	{
		if (type->get_unlocalized_name() == unlocalized_name)
			return type;
	}

	latte::logger::error(latte::logger::level::EDITOR, "failed to get type \"" + std::string(unlocalized_name) + "\"");
	return nullptr;
}

void esp::registry::get_types(const char** unlocalized_names, usize count, std::vector<esp::reg::type*>& types)
{
	if (types.empty())
		types.reserve(count);

	for (usize i = 0; i < count; i++)
	{
		const char* unlocalized_name = unlocalized_names[i];

		for (auto type : m_types)
		{
			if (type->get_unlocalized_name() == unlocalized_name)
				types.emplace_back(type);
		}
	}
}

void esp::registry::get_types(const char* unlocalized_name, std::vector<esp::reg::type*>& types)
{
	if (types.empty())
		types.reserve(1);

	for (auto type : m_types)
	{
		if (type->get_unlocalized_name() == unlocalized_name)
		{
			types.emplace_back(type);
			return;
		}
	}
}

esp::argument* esp::registry::get_argument(const std::string& unlocalized_name)
{
	for (auto argument : m_arguments)
	{
		if (argument->get_unlocalized_name() == unlocalized_name)
			return argument;
	}

	latte::logger::error(latte::logger::level::EDITOR, "failed to get argument \"" + unlocalized_name + "\"");
	return nullptr;
}

esp::block* esp::registry::get_block(const char* unlocalized_name)
{
	for (auto block : m_blocks)
	{
		if (!strcmp(block->get_unlocalized_name(), unlocalized_name))
			return block;
	}

	latte::logger::error(latte::logger::level::EDITOR, "failed to get block \"" + std::string(unlocalized_name) + "\"");
	return nullptr;
}

esp::category* esp::registry::get_category(const char* unlocalized_name)
{
	for (auto category : m_categories)
	{
		if (category->get_unlocalized_name() == unlocalized_name)
			return category;
	}

	latte::logger::error(latte::logger::level::EDITOR, "failed to get category \"" + std::string(unlocalized_name) + "\"");
	return nullptr;
}

esp::registry* esp::registry::m_registry = nullptr;