#include "serializer.hpp"

bool rhr::handler::serializer::node::verify_data(const std::vector<std::string>& names)
{
	if (names.size() != data_names.size() || names.size() != data_values.size())
	{
		cap::logger::error(cap::logger::level::EDITOR, __FILE__, __LINE__, "failed to verify data");
		return false;
	}

	for (usize i = 0; i < names.size(); i++)
	{
		if (names[i] != data_names[i])
			return false;
	}

	return true;
}

bool rhr::handler::serializer::node::verify_children(usize count)
{
	if (children.size() != count)
	{
		cap::logger::error(cap::logger::level::EDITOR, __FILE__, __LINE__, "failed to verify children");
		return false;
	}

	return true;
}

rhr::handler::serializer::serializer(rhr::handler::serializer::node& node)
	: m_node(node)
{

}

const std::string& rhr::handler::serializer::serialize()
{
	serialize_node(m_node, m_serialized);
	return m_serialized;
}

void rhr::handler::serializer::deserialize(std::string& buffer)
{
	deserialize_node(buffer, m_node);
}

rhr::handler::serializer::node& rhr::handler::serializer::get_node()
{
	return m_node;
}

void rhr::handler::serializer::serialize_node(node& node, std::string& buffer)
{
	rapidjson::Document document;
	document.SetObject();

	// Add children.

	rapidjson::Value v_children;
	v_children.SetArray();

	std::vector<std::string> child_buffers;

	for (auto& child : node.children)
	{
		rapidjson::Value v_child_data;

		std::string& child_buffer = child_buffers.emplace_back();
		serialize_node(child, child_buffer);

		v_child_data.SetString(child_buffer.c_str(), child_buffer.size());
		v_children.PushBack(v_child_data, document.GetAllocator());
	}

	document.AddMember("ch", v_children, document.GetAllocator());

	// Add data.

	rapidjson::Value v_data_names;
	v_data_names.SetArray();

	rapidjson::Value v_data_values;
	v_data_values.SetArray();

	for (usize i = 0; i < node.data_names.size(); i++)
	{
		rapidjson::Value v_data_name;
		rapidjson::Value v_data_value;

		auto& data_name = node.data_names[i];
		auto& data_value = node.data_values[i];

		v_data_name.SetString(data_name.c_str(), data_name.size());
		v_data_value.SetString(data_value.c_str(), data_value.size());

		v_data_names.PushBack(v_data_name, document.GetAllocator());
		v_data_values.PushBack(v_data_value, document.GetAllocator());
	}

	document.AddMember("dn", v_data_names, document.GetAllocator());
	document.AddMember("dv", v_data_values, document.GetAllocator());

	// Submit.

	rapidjson::StringBuffer string_buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(string_buffer);
	document.Accept(writer);

	buffer = string_buffer.GetString();
}

void rhr::handler::serializer::deserialize_node(const std::string& buffer, rhr::handler::serializer::node& node)
{
	rapidjson::Document document;
	document.Parse(buffer.c_str(), buffer.size());

	if (!document.IsObject())
	{
		cap::logger::error(cap::logger::level::EDITOR, __FILE__, __LINE__, SERIALIZER_FAILED_PARSE);
		return;
	}

	if (!document.HasMember("ch") || !document.HasMember("dn") || !document.HasMember("dv"))
	{
		cap::logger::error(cap::logger::level::EDITOR, __FILE__, __LINE__, SERIALIZER_FAILED_PARSE);
		return;
	}

	if (!document["ch"].IsArray() || !document["dn"].IsArray() || !document["dv"].IsArray())
	{
		cap::logger::error(cap::logger::level::EDITOR, __FILE__, __LINE__, SERIALIZER_FAILED_PARSE);
		return;
	}

	auto data_names = document["dn"].GetArray();
	auto data_values = document["dv"].GetArray();
	auto children = document["ch"].GetArray();

	for (auto& data_name : data_names)
		node.data_names.emplace_back(data_name.GetString());

	for (auto& data_value : data_values)
		node.data_values.emplace_back(data_value.GetString());

	for (auto& child : children)
	{
		auto& child_node = node.children.emplace_back();
		std::string child_buffer = child.GetString();

		deserialize_node(child_buffer, child_node);
	}
}
