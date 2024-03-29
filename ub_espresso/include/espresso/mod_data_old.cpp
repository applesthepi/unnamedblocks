#include "mod_data.hpp"

const std::string& esp::mod_data::get_mod_display_name()
{
	return m_mod_display_name;
}

const std::string& esp::mod_data::get_mod_unlocalized_name()
{
	return m_mod_unlocalized_name;
}

const std::string& esp::mod_data::get_version()
{
	return m_version;
}

latte::logger::stream* esp::mod_data::get_stream_system()
{
	return m_stream_system;
}

latte::logger::stream* esp::mod_data::get_stream_editor()
{
	return m_stream_editor;
}

latte::logger::stream* esp::mod_data::get_stream_runtime()
{
	return m_stream_runtime;
}

rhr::handler::input::stream* esp::mod_data::get_stream_input()
{
	return m_stream_input;
}

esp::registry* esp::mod_data::get_registry_esp()
{
	return m_registry_esp;
}

rhr::registry::char_texture* esp::mod_data::get_registry_char_texture()
{
	return m_registry_char_texture;
}

rhr::render::renderer* esp::mod_data::get_renderer()
{
	return m_renderer;
}

std::shared_ptr<rhr::stack::plane>* esp::mod_data::get_primary_plane()
{
	return m_primary_plane;
}

std::shared_ptr<rhr::stack::plane>* esp::mod_data::get_toolbar_plane()
{
	return m_toolbar_plane;
}

void esp::mod_data::set_mod_display_name(const std::string& display_name)
{
	m_mod_display_name = display_name;
}

void esp::mod_data::set_mod_unlocalized_name(const std::string& unlocalized_name)
{
	m_mod_unlocalized_name = unlocalized_name;
}

void esp::mod_data::set_version(const std::string& version)
{
	m_version = version;
}

void esp::mod_data::set_logger_linkage(
	latte::logger::stream* stream_system, latte::logger::stream* stream_editor, latte::logger::stream* stream_runtime)
{
	m_stream_system	 = stream_system;
	m_stream_editor	 = stream_editor;
	m_stream_runtime = stream_runtime;
}

void esp::mod_data::set_input_stream(rhr::handler::input::stream* input_stream)
{
	m_stream_input = input_stream;
}

void esp::mod_data::set_registry_esp(esp::registry* registry)
{
	m_registry_esp = registry;
}

void esp::mod_data::set_registry_char_texture(rhr::registry::char_texture* registry)
{
	m_registry_char_texture = registry;
}

void esp::mod_data::set_renderer(rhr::render::renderer* renderer)
{
	m_renderer = renderer;
}

void esp::mod_data::set_planes(std::shared_ptr<rhr::stack::plane>* primary_plane, std::shared_ptr<rhr::stack::plane>* toolbar_plane)
{
	m_primary_plane = primary_plane;
	m_toolbar_plane = toolbar_plane;
}
