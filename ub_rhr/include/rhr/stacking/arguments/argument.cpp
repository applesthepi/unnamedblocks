#include "argument.hpp"

#include "rhr/stacking/block.hpp"

#define ARG_SET_DATA_ERROR_MISSING(field)                     \
	("failed to set data for argument. " + std::string(field) \
	 + " does not exist in the data provided to the argument. The information is unchanged for this argument.")
#define ARG_SET_DATA_ERROR_TYPE(field)                        \
	("failed to set data for argument. " + std::string(field) \
	 + " is not the expected type. The information is unchanged for this argument.")

rhr::stack::argument::argument::argument(
	const cap::color& block_color, std::function<void()>* function_collection_update, glm::vec<2, i32>* plane_offset)
	: m_mode(cap::mod::block::block::argument::variable_mode::RAW)
	, m_block_color(block_color)
	, m_function_collection_update(function_collection_update)
	, m_mode_restriction(cap::mod::block::block::argument::variable_mode_restriction::NONE)
	, m_dirty(true)
	, m_plane_offset(plane_offset)
{}

void rhr::stack::argument::argument::set_data(const std::string& data)
{
	m_data	= data;
	m_dirty = true;
	on_set_data();
}

void rhr::stack::argument::argument::set_data_compact(const std::string& data)
{
	rapidjson::Document document;
	document.Parse(data.c_str(), data.size());

	if (document.HasParseError())
	{
		cap::logger::error(
			cap::logger::level::EDITOR, __FILE__, __LINE__, "parsing error on data provided to the argument.");
		return;
	}

	if (!document.IsObject())
	{
		cap::logger::error(
			cap::logger::level::EDITOR,
			__FILE__,
			__LINE__,
			"data provided to the argument is not an object that can be parsed.");
		return;
	}

	if (!document.HasMember("mode"))
	{
		cap::logger::error(cap::logger::level::EDITOR, __FILE__, __LINE__, ARG_SET_DATA_ERROR_MISSING("mode"));
		return;
	}

	if (!document.HasMember("data"))
	{
		cap::logger::error(cap::logger::level::EDITOR, __FILE__, __LINE__, ARG_SET_DATA_ERROR_MISSING("data"));
		return;
	}

	rapidjson::Value& v_mode = document["mode"];
	rapidjson::Value& v_data = document["data"];

	if (!v_mode.IsInt())
	{
		cap::logger::error(cap::logger::level::EDITOR, __FILE__, __LINE__, ARG_SET_DATA_ERROR_TYPE("mode"));
		return;
	}

	if (!v_data.IsString())
	{
		cap::logger::error(cap::logger::level::EDITOR, __FILE__, __LINE__, ARG_SET_DATA_ERROR_TYPE("data"));
		return;
	}

	u8 local_mode		   = static_cast<u8>(v_mode.GetInt());
	const char* local_data = v_data.GetString();

	switch (local_mode)
	{
	case 0:
		m_mode = cap::mod::block::block::argument::variable_mode::RAW;
		break;
	case 1:
		m_mode = cap::mod::block::block::argument::variable_mode::VAR;
		break;
	default:
		cap::logger::error(
			cap::logger::level::EDITOR,
			__FILE__,
			__LINE__,
			"unknown cap::mod::block::block::argument::variable_mode index; using RAW instead");
		m_mode = cap::mod::block::block::argument::variable_mode::RAW;
		break;
	}

	m_data	= std::move(std::string(local_data));
	m_dirty = true;

	on_set_data();
	on_set_mode(m_mode);
}

bool rhr::stack::argument::argument::set_mode(cap::mod::block::block::argument::variable_mode mode)
{
	if (m_mode_restriction == cap::mod::block::block::argument::variable_mode_restriction::NONE)
	{
		m_mode	= mode;
		m_dirty = true;
		on_set_mode(mode);
		return true;
	}

	return false;
}

cap::mod::block::block::argument::variable_mode rhr::stack::argument::argument::get_mode()
{
	return m_mode;
}

const std::string& rhr::stack::argument::argument::get_data()
{
	return m_data;
}

const std::string& rhr::stack::argument::argument::get_data_compact()
{
	build_data_distribute();
	return m_data_distribute;
}

cap::mod::block::block::argument::type rhr::stack::argument::argument::get_type()
{
	return cap::mod::block::block::argument::type::TEXT;
}

u32 rhr::stack::argument::argument::get_width()
{
	return 50;
}

bool rhr::stack::argument::argument::has_data()
{
	return false;
}

bool rhr::stack::argument::argument::drag_bounds(glm::vec<2, i32> position)
{
	return false;
}

rhr::stack::argument::argument::padding_style rhr::stack::argument::argument::get_padding_style()
{
	return rhr::stack::argument::argument::padding_style::HARD;
}

void rhr::stack::argument::argument::set_plane_offset(glm::vec<2, i32>* plane_offset)
{
	m_plane_offset = plane_offset;
}

i32 rhr::stack::argument::argument::padding = rhr::stack::block::padding * 4;

void rhr::stack::argument::argument::build_data_distribute()
{
	if (!m_dirty)
		return;

	rapidjson::Document document;
	document.SetObject();

	rapidjson::Value v_mode;
	rapidjson::Value v_data;

	switch (m_mode)
	{
	case cap::mod::block::block::argument::variable_mode::RAW:
		v_mode.SetInt(0);
		break;
	case cap::mod::block::block::argument::variable_mode::VAR:
		v_mode.SetInt(1);
		break;
	default:
		cap::logger::error(
			cap::logger::level::EDITOR,
			__FILE__,
			__LINE__,
			"unknown cap::mod::block::block::argument::variable_mode; using 0 instead");
		v_mode.SetInt(0);
		break;
	}

	v_data.SetString(m_data.c_str(), m_data.size());

	document.AddMember("mode", v_mode, document.GetAllocator());
	document.AddMember("data", v_data, document.GetAllocator());

	rapidjson::StringBuffer string_buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(string_buffer);
	document.Accept(writer);

	m_data_distribute = std::move(std::string(string_buffer.GetString()));
	m_dirty			  = false;
}

void rhr::stack::argument::argument::on_set_data()
{}

void rhr::stack::argument::argument::set_mode_restriction(
	cap::mod::block::block::argument::variable_mode_restriction mode_restriction)
{
	m_mode_restriction = mode_restriction;
}

void rhr::stack::argument::argument::on_set_mode(cap::mod::block::block::argument::variable_mode mode)
{}
