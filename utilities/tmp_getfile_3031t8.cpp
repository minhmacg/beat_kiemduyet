#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <regex>

int main(int argc, const char** argv)
{
	if (argc != 3) throw std::runtime_error("invalid number of argument");

	std::filesystem::path outfolder {argv[2]};
	std::vector<std::filesystem::path> files;
	std::ifstream f {argv[1]};

	std::regex fileregex {"([\\d_]+_n)(_\\d*.(png|jpg|mp4))?"};
	std::string line;
	while (std::getline(f, line))
	{
		std::smatch rm;
		while (std::regex_search(line, rm, fileregex))
		{
			files.push_back(rm.str());
			line = rm.suffix().str();
		};
	}

	for (const auto& filepath: files)
	{
		std::filesystem::rename(filepath, outfolder / filepath.filename());
	}
}
