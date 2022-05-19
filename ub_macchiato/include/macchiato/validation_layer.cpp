#include "validation_layer.hpp"

void mac::validation_layer::simplify(const std::vector<mac::validation_layer>& in, std::vector<const char*>& out)
{
	for (const auto& validation_layer : in)
		out.emplace_back(validation_layer.name.c_str());
}
