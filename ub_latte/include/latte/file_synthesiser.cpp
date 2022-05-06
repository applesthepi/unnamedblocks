#include "file_synthesiser.hpp"

latte::file_synthesiser::file_synthesiser(usize initial)
	: m_dirty(false)
{
	m_lines.reserve(initial);

	for (auto& line : m_lines)
		line.reserve(100);
}

latte::file_synthesiser::~file_synthesiser()
{
	if (m_dirty)
		latte::logger::warn(logger::level::SYSTEM, __FILE__, __LINE__, "deallocating file synthesiser that has not been written");
}

std::unique_ptr<latte::file_synthesiser> latte::file_synthesiser::read(const std::string& filepath)
{
	auto synthesiser = std::make_unique<file_synthesiser>();
	synthesiser->create_read(filepath);
	return synthesiser;
}

std::unique_ptr<latte::file_synthesiser> latte::file_synthesiser::write(const std::string& filepath)
{
	auto synthesiser = std::make_unique<file_synthesiser>();
	synthesiser->create_write(filepath);
	return synthesiser;
}

std::pair<usize, usize> latte::file_synthesiser::insert(usize idx, const std::string& text)
{
	if (idx > m_lines.size())
	{
		latte::logger::error(logger::level::SYSTEM, __FILE__, __LINE__, "failed to insert text into file");
		return std::make_pair(0, 0);
	}

	std::vector<std::string> lines;
	separate_lines(lines, text);

	usize loc_idx = lines.size() - 1;

	for (usize i = 0; i < lines.size(); i++)
	{
		usize ii = lines.size() - i - 1;
		m_lines.insert(m_lines.begin() + idx, lines[ii]);
	}

	m_dirty = true;
	return std::make_pair(loc_idx, lines.size());
}

std::pair<usize, usize> latte::file_synthesiser::push_back(const std::string& text)
{
	std::vector<std::string> lines;
	separate_lines(lines, text);

	usize loc_idx = lines.size() - 1;

	for (const auto& line : lines)
		m_lines.emplace_back(line);

	m_dirty = true;
	return std::make_pair(loc_idx, lines.size());
}

void latte::file_synthesiser::overwrite()
{
	m_stream.open(m_filepath, std::ios::out | std::ios::trunc);

	if (!m_stream.is_open())
	{
		latte::logger::error(logger::level::SYSTEM, __FILE__, __LINE__, "failed to write to file");
		return;
	}

	for (const auto& line : m_lines)
		m_stream << line << "\n";

	m_dirty = false;
	m_stream.close();
}

const std::string& latte::file_synthesiser::get_line(usize idx)
{
	if (idx >= m_lines.size())
	{
		latte::logger::error(logger::level::SYSTEM, __FILE__, __LINE__, "failed to insert text into file");
		static std::string dummy_return;
		return dummy_return;
	}

	return m_lines[idx];
}

void latte::file_synthesiser::separate_lines(std::vector<std::string>& lines, const std::string& text)
{
	auto line = &lines.emplace_back();
	usize idx = 0;

	while (idx < text.size())
	{
		char next_char = text[idx];

		if (next_char == 10 || next_char == 13)
			line = &lines.emplace_back();
		else
			*line += next_char;

		idx++;
	}
}

void latte::file_synthesiser::create_read(const std::string& filepath)
{
	m_filepath = filepath;
	m_stream.open(filepath, std::ios::in);

	if (!m_stream.is_open())
	{
		latte::logger::error(logger::level::SYSTEM, __FILE__, __LINE__, "failed read file");
		return;
	}

	m_lines.clear();
	std::string line;

	while (std::getline(m_stream, line))
		m_lines.emplace_back(line);

	m_stream.close();
}

void latte::file_synthesiser::create_write(const std::string& filepath)
{
	m_filepath = filepath;
	m_lines.clear();
}
