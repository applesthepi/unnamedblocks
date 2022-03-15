#pragma once
#include "config.h"

#include <latte/utils.hpp>

#define SERIALIZER_FAILED_PARSE "failed to parse save"

namespace latte
{
///
class serializer
{
public:
	///
	struct node
	{
		///
		bool verify_data(const std::vector<std::string>& names);

		///
		bool verify_children(usize count);

		///
		std::vector<std::string> data_values;

		///
		std::vector<std::string> data_names;

		///
		std::vector<node> children;
	};

	explicit serializer(node& node);

	///
	const std::string& serialize();

	///
	void deserialize(std::string& buffer);

	///
	node& get_node();

private:
	///
	void serialize_node(node& node, std::string& buffer);

	///
	void deserialize_node(const std::string& buffer, node& node);

	///
	node& m_node;

	///
	std::string m_serialized;
};
}