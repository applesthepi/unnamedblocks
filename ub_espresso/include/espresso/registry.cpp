#include "registry.hpp"

esp::registry* esp::registry::get()
{
	return m_registry;
}

void esp::registry::set(esp::registry* reg)
{
	m_registry = reg;
}

std::unordered_map<std::string, esp::reg::type*>& esp::registry::hash_types()
{
	return m_hash_types;
}

std::unordered_map<std::string, esp::argument*>& esp::registry::hash_arguments()
{
	return m_hash_arguments;
}

std::unordered_map<std::string, esp::block*>& esp::registry::hash_blocks()
{
	return m_hash_blocks;
}

std::unordered_map<std::string, esp::category*>& esp::registry::hash_categories()
{
	return m_hash_categories;
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

std::vector<const char*>& esp::registry::definitions()
{
	return m_definitions;
}

std::vector<const char*>& esp::registry::initialization()
{
	return m_initialization;
}

std::vector<std::pair<const char*, const char*>>& esp::registry::parameters()
{
	return m_parameters;
}

std::vector<const char*>& esp::registry::libraries()
{
	return m_libraries;
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
	auto result = m_hash_arguments[unlocalized_name];

	if (result)
		return result;

	latte::logger::error(latte::logger::level::EDITOR, "failed to get argument \"" + unlocalized_name + "\"");
	return nullptr;
}

esp::rt_argument* esp::registry::get_rt_argument(const std::string& unlocalized_name)
{
	auto result = m_hash_rt_arguments[unlocalized_name];

	if (result)
		return result;

	latte::logger::error(latte::logger::level::EDITOR, "failed to get argument \"" + unlocalized_name + "\"");
	return nullptr;
}

esp::block* esp::registry::get_block(const char* unlocalized_name)
{
	auto result = m_hash_blocks[unlocalized_name];

	if (result)
		return result;

	latte::logger::error(latte::logger::level::EDITOR, "failed to get block \"" + std::string(unlocalized_name) + "\"");
	return nullptr;
}

esp::category* esp::registry::get_category(const char* unlocalized_name)
{
	auto result = m_hash_categories[unlocalized_name];

	if (result)
		return result;

	latte::logger::error(latte::logger::level::EDITOR, "failed to get category \"" + std::string(unlocalized_name) + "\"");
	return nullptr;
}

esp::registry* esp::registry::m_registry = nullptr;
