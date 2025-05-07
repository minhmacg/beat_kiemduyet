#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include "drive.h"
#include <unordered_map>
#include <print>
std::unordered_map<std::string, std::string> get_files_path(const std::filesystem::path& path)
{
	std::unordered_map<std::string, std::string> rs;
	if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path))
		throw std::invalid_argument(std::format("{} is invalid path", path.string()));
	for (const auto& file: std::filesystem::recursive_directory_iterator(path))
	{
		//std::println("{}", file.path().string());
		rs.emplace(file.path().filename().string(), file.path().string());
	};

	return rs;
};
int main(int argc, const char** argv)
{
	std::ifstream f("drivelink/drivelinks.txt");
	drivemap drive = load_drive_map(f,true);
	auto files {get_files_path("../cleaned_medias")};
	
	std::filesystem::path folder {"../copied_cleaned_medias"};
	std::filesystem::create_directory(folder);

	for (const auto& [fname, fpath]: files)
	{
		if (!drive.contains(fname))
		{
			std::filesystem::path oldpath{fpath};
			auto it {oldpath.begin()};
			std::advance(it,2);
			std::filesystem::path new_path {folder};
			for (; it != oldpath.end(); ++it)
				new_path /= *it;

			std::println("{}", fname);
			std::filesystem::create_directories(new_path.parent_path());
			if (!std::filesystem::exists(new_path)) std::filesystem::copy(oldpath, new_path);
		};
	};

};
