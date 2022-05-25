#include "vertex.hpp"

vk::vertex_input_binding_description mac::vertex::info::get_binding_description()
{
	vk::vertex_input_binding_description vertex_input_binding_description {};

	vertex_input_binding_description.binding   = 0;
	vertex_input_binding_description.stride	   = stride();
	vertex_input_binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return vertex_input_binding_description;
}

std::vector<vk::vertex_input_attribute_description> mac::vertex::info::get_attribute_description()
{
	std::vector<vk::vertex_input_attribute_description> attribute_descriptions = {};

	auto vertex_formats = formats();
	auto vertex_offsets = offsets();

	for (u32 i = 0; i < vertex_offsets.size(); i++)
	{
		auto& attribute_description = attribute_descriptions.emplace_back(vk::vertex_input_attribute_description{});

		attribute_description.binding  = 0;
		attribute_description.location = i;
		attribute_description.format   = vertex_formats[i];
		attribute_description.offset   = vertex_offsets[i];
	}

	return attribute_descriptions;
}