#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "../include/drive.h"
#include <filesystem>
#include <regex>
#include <algorithm>

int main(int argc, const char** argv)
{
	if (argc != 2)
	{
		std::cout << "invalid argument";
		return -1;
	};
	
	std::filesystem::path input_path {argv[1]};
	
	std::ifstream f;
	f.open(input_path);
	if (!f.is_open()) std::cout << "bad file input\n";
	
	std::vector<std::string> file_content;
	std::string line;
	while (std::getline(f,line))
	{
		file_content.push_back(line);
	};
	f.close();
	
	f.open("drivelink/drivelinks.txt");
	drivemap s = retain_base_name_media(load_drive_map(f,true));
	f.close();
	
	int i {1};
	for (auto& line: file_content)
	{
		std::cout << "line " << i << '\n';
		std::vector<std::string> matches;
		std::regex rg {"([\\d_]+_n)(_\\d*.(png|jpg|mp4))?"}; // any number of digit or '_', followed by a 'n',
												  // maybe followed by .jpg, .png or .mp4
		std::smatch rm;
		while (std::regex_search(line,rm,rg))
		{
			std::cout << "found: ";
			std::cout << rm.str() << '\n';
			line.replace(line.find(rm.str()), rm.str().size(), s[rm.str()]);
		};
		i++;
		std::cout << "................................\n";
		//auto ri_begin = std::sregex_iterator(line.begin(),line.end(), rg);
		//auto ri_end = std::sregex_iterator();
		//std::cout << "found " << std::distance(ri_begin, ri_end) << "    ........................\n";
		//for (auto& ri = ri_begin; ri != ri_end; ri++)
		//{
		//	std::cout << line.find(ri->str()) << '\n';
		//	std::cout << ri->str() << '\n';
		//	line.replace(line.find(ri->str()), ri->str().size(), s[ri->str(1)]);
		//};
		//for (auto& m: matches)
		//{
		//	line.replace(line.find(m),m.size(),s[m]);
		//};
	};
	
	std::ofstream o;
	o.open("output/drive_mapped_" + input_path.filename().string()); 
	for (auto& line: file_content)
		o << line << '\n';
	o.close();
};
