#include "parse_tsv.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>
std::vector<std::string> parse_line(const std::string_view& line)
{
	static auto add_quote_if_newline = [](std::string& str)
	{
		if (str.find('\n') != std::string::npos) str = "\"" + str + "\"";
	};
	std::vector<std::string> rs;
	std::string field;
	bool in_quote;

	for (auto i {0}; i != line.size(); i++)
	{
		char c = line[i];

		if (c == '"')
		{
			field += '"';
			if (i + 1 <= line.size() && line[i+1] == '"')
			{
				field += '"';
				i++;
			}
			else in_quote = !in_quote;
		}
		else if (c == '\t' && !in_quote)
		{
			rs.push_back(field);
			field.clear();
		}
		else field += c;
	};
	rs.push_back(field);
	return rs;
};
std::vector<std::vector<std::string>> parse_tsv(const std::string& path)
{
	std::ifstream f (path);
	
	std::vector<std::vector<std::string>> data;

	bool multiline {false};

	std::string line, full_line;
	while (std::getline(f, line))
	{
		const std::string smartQuotes[] = {"“", "”"};  // Unicode smart quotes
		for (const std::string& sq : smartQuotes) {
			size_t pos;
			while ((pos = line.find(sq)) != std::string::npos) {
				line.replace(pos, sq.length(), "\"\"");
			}
		}
		if (multiline) full_line += '\n' + line;
		else full_line = line;

		int quote_count {0};
		for (auto i {0}; i != full_line.size(); i++)
			if (full_line[i] == '"')
			{
				if (i < full_line.size() - 1 && full_line[i+1] == '"')
					i++;
				else quote_count++;
			};
		if (quote_count % 2 == 0 )
		{
			data.push_back(parse_line(full_line));
			multiline = false;
		}
		else multiline = true;
	};

	return data;
}
//int main(int argc, const char** argv)
//{
//	if (argc == 1)
//	{
//		std::cout << "Usage: parse_tsv [-f <field number>] input_file\n";
//		return 0;
//	};
//	std::vector<std::string> args {argv + 1, argv + argc};
//	if (!std::filesystem::exists(args.back()))
//		throw std::invalid_argument(std::format("{} doesn't exist", argv[argc-1]));
//
//	int field_to_print {-1};
//	if (auto field_to_print_it = std::ranges::find_if(args,
//				[](auto&& arg){return arg == "-f";});
//			field_to_print_it != args.end())
//	{
//		auto field = field_to_print_it + 1;
//
//		if (std::ranges::find_if(*field, [](auto&& f){return !std::isdigit(f);}) != field->end())
//			throw std::invalid_argument(std::format("field number {} invalid", *field));
//		if (field == args.end()) throw std::invalid_argument("missing field number");
//
//		field_to_print = std::stoi(*field);
//	};
//
//	auto data = parse_line(args.back());
//	if (field_to_print > 0)
//		for (const auto& rows: data)
//		{
//			std::cout << "[";
//			if (field_to_print == 0)
//				for (const auto& field: rows)
//					std::cout << field << '\t';
//			else if (field_to_print <= rows.size())
//				std::cout << rows[field_to_print - 1];
//			std::cout << "]\n";
//		};
//};
