#include "page_media.hpp"
#include <filesystem>
#include <fstream>
#include <print>
#include <regex>
constexpr std::string_view PATH {"../medias"};

std::optional<std::string> try_get_media_and_remove_from_line(std::string& line)
{
	std::regex rg {"([\\d_]+_n)(.(png|jpg|mp4))?"};
	std::smatch rm;
	if (std::regex_search(line, rm, rg))
	{
		auto str = rm.str();
		std::println("found {}", str);
		line.erase(line.find(str), str.size()); 
		return str; 
	};
	return std::nullopt;
};
int main(int argc, const char** argv)
{
	if (argc != 2)
	{
		std::println("usage: clean_media file_path");
		return -1;
	};
	if (!std::filesystem::exists(PATH))
	{
		std::println("medias folder doesn't exist");
		return -1;
	};
	if (!std::filesystem::exists(argv[1]))
	{
		std::println("file doesn't exist");
		return -1;
	};

	std::vector<page> page_medias {fetch_page_medias(PATH)};
	
	std::ifstream f {argv[1]};

	std::vector<std::string> used_media;
	std::string line;
	// Get all media code in file
	while (std::getline(f, line))
	{
		while (true)
		{
			auto m = try_get_media_and_remove_from_line(line);
			if (!m.has_value()) break;
			used_media.emplace_back(m.value());
		};
	};

	std::filesystem::path cleaned_dir {"../cleaned_medias"};
	std::filesystem::create_directory(cleaned_dir);
	for (const auto& page: page_medias)
	{
		std::println("{}", page._pagename);
		for (const auto& files: used_media)
		{
			if (auto p {page.find_media(files)}; p.has_value())
			{
				std::filesystem::path src {std::string(PATH) + "/" + p.value()};
				std::filesystem::path dst_page_folder {cleaned_dir / page._pagename};
				std::string type = src.parent_path().filename().string();
				std::filesystem::path dst {dst_page_folder / src.parent_path().filename() / src.filename()};

				std::filesystem::create_directories(dst.parent_path());
				std::println("dst {}", dst.string());
				std::filesystem::copy(src, dst, std::filesystem::copy_options::skip_existing);
				std::println("Copying from {}", src.string());
			};
		};
	};
};
