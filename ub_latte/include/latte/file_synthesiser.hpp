#pragma once
#include "config.h"

#include <latte/utils.hpp>
#include <fstream>

namespace latte
{
class file_synthesiser
{
public:
	explicit file_synthesiser(usize initial = 1000);
	~file_synthesiser();

	///
	static std::unique_ptr<file_synthesiser> read(const std::string& filepath);

	///
	static std::unique_ptr<file_synthesiser> write(const std::string& filepath);

	///
	std::pair<usize, usize> insert(usize idx, const std::string& text);

	///
	std::pair<usize, usize> push_back(const std::string& text);

	///
	void overwrite();

	///
	const std::string& get_line(usize idx);
private:
	///
	static void separate_lines(std::vector<std::string>& lines, const std::string& text);

	///
	void create_read(const std::string& filepath);

	///
	void create_write(const std::string& filepath);

	///
	std::vector<std::string> m_lines;

	///
	std::fstream m_stream;

	///
	std::string m_filepath;

	///
	bool m_dirty;
};
}