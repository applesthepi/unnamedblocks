#pragma once
#include "config.h"

#include "espresso/type.hpp"
#include "espresso/argument.hpp"
#include "espresso/block.hpp"
#include "espresso/category.hpp"
#include "espresso/registry.hpp"

#include <latte/utils.hpp>
#include <lungo/registries/char_texture.hpp>
#include <lungo/renderer.hpp>

namespace esp
{
///
class mod_data
{
public:
	///
	const std::string& get_mod_display_name();

	///
	const std::string& get_mod_unlocalized_name();

	///
	const std::string& get_version();

	///
	latte::logger::stream* get_stream_system();

	///
	latte::logger::stream* get_stream_editor();

	///
	latte::logger::stream* get_stream_runtime();

	///
	esp::registry* get_registry_esp();

	///
	rhr::registry::char_texture* get_registry_char_texture();

	///
	rhr::render::renderer* get_renderer();

	///
	void set_mod_display_name(const std::string& display_name);

	///
	void set_mod_unlocalized_name(const std::string& unlocalized_name);

	///
	void set_version(const std::string& version);

	///
	void set_logger_linkage(
		latte::logger::stream* stream_system, latte::logger::stream* stream_editor, latte::logger::stream* stream_runtime);

	///
	void set_registry_esp(esp::registry* registry);

	///
	void set_registry_char_texture(rhr::registry::char_texture* registry);

	///
	void set_renderer(rhr::render::renderer* renderer);
private:
	///
	std::string m_mod_display_name;

	///
	std::string m_mod_unlocalized_name;

	///
	std::string m_version;

	///
	latte::logger::stream* m_stream_system;

	///
	latte::logger::stream* m_stream_editor;

	///
	latte::logger::stream* m_stream_runtime;

	///
	esp::registry* m_registry_esp;

	///
	rhr::registry::char_texture* m_registry_char_texture;

	///
	rhr::render::renderer* m_renderer;
};
}