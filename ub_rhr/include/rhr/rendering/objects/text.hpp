#pragma once
#include "config.h"

#include "rhr/handlers/field.hpp"
#include "rhr/rendering/interfaces/i_dicolorable.hpp"
#include "rhr/rendering/interfaces/i_field.hpp"
#include "rhr/rendering/interfaces/i_ui.hpp"
#include "rhr/rendering/objects/object.hpp"

#include <cappuccino/utils.hpp>

namespace rhr::render::object
{
///
class text
	: public rhr::render::interfaces::i_ui
	, public rhr::render::interfaces::i_dicolorable
	, public rhr::render::interfaces::i_field
{
public:
	text(rhr::registry::char_texture::texture_type texture_type, u16 font_size, bool read_only, bool force_register);
	~text();

	///
	void set_update_function(std::function<void()>* function_update);

	///
	void set_weak_field(std::weak_ptr<rhr::render::interfaces::i_field>&& weak_field);

	/// Sets text.
	/// \param Text.
	void set_text(const std::string& text);

	///
	const std::string& get_text();

	/// Sets Z depth.
	/// \param Z depth.
	void set_depth(i32 depth);

	///
	void set_padding(i32 padding);

	///
	void set_font_size(u16 font_size);

	///
	void enable_background(bool enable);

	///
	void set_mouse_button(std::function<void(glm::vec<2, i32> position, f32 scroll, MouseOperation operation, MouseButton button)>& mouse_button);

	///
	std::optional<usize> pick_index(glm::vec<2, i32> position, bool ignore_y) override;

	///
	std::optional<glm::vec<2, i32>> get_index_position(usize idx) override;

	///
	usize get_index_count() override;

	///
	void insert_char(char charactor, usize idx) override;

	///
	void insert_string(const std::string& string, usize idx) override;

	///
	bool remove_char(usize idx) override;

	///
	bool remove_string(usize idx, usize size) override;

	///
	void mouse_button(glm::vec<2, i32> position, f32 scroll, MouseOperation operation, MouseButton button) override;

private:
	///
	void update_size();

	///
	void ui_transform_update(i_ui::transform_update_spec transform_update_spec) override;

	///
	void ui_render() override;

	///
	void ui_reload_swap_chain() override;

	///
	void ui_update_buffers() override;

	///
	void ui_chain_update_buffers() override;

	///
	void ui_frame_update(f64 delta_time) override;

	///
	void post_color_update() override;

	///
	void register_field();

	///
	void unregister_field();

	/// Object to render the background rectangle.
	std::shared_ptr<rhr::render::object::object> m_render_object_background;

	/// Object to render the actual text.
	std::shared_ptr<rhr::render::object::object> m_render_object_text;

	///
	std::string m_text;

	///
	std::vector<i16> m_char_contacts;

	///
	std::vector<i16> m_char_offsets;

	///
	i32 m_depth;

	///
	i32 m_padding;

	///
	bool m_enable_background;

	///
	std::function<void()>* m_function_update;

	///
	std::optional<rhr::handler::field_data::location> m_location;

	///
	bool m_read_only;

	///
	std::function<void(glm::vec<2, i32> position, f32 scroll, MouseOperation operation, MouseButton button)> m_mouse_button;

	///
	u16 m_font_size;

	///
	bool m_registered;

	///
	std::weak_ptr<rhr::render::interfaces::i_field> m_weak_field;

	///
	bool m_force_register;

	///
	rhr::registry::char_texture::texture_type m_texture_type;
};
} // namespace rhr::render::object