#ifndef BEAT_DRIVE_H
#define BEAT_DRIVE_H

#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>
#include <regex>
using drivemap = std::map<std::string, std::string>;

inline std::vector<std::string> split(const std::string& input, const char delim)
{
	std::stringstream ss {input};
	std::string line;
	std::vector<std::string> rs;
	while (std::getline(ss,line,delim)) 
	{
		line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
		rs.push_back(line);
	};
	return rs;
};

inline drivemap load_drive_map(std::ifstream& f, bool store_by_filename)
{
	drivemap rs;
	std::string line;
	while (std::getline(f,line))
	{
		std::vector<std::string> line_o = split(line,'\t');
		if (line_o.size() == 2) 
			store_by_filename ? rs[line_o[0]] = line_o[1] : rs[line_o[1]] = line_o[0];
	};
	return rs;
};

inline drivemap retain_base_name_media(const drivemap& dm)
{
	drivemap rs;
	for (auto& [key,value]: dm)
	{
		std::regex rg{"((\\d|_)+_n)"};
		std::smatch rm;
		std::regex_search(key,rm,rg);
		rs[rm.str(1)] = value;
	};
	return rs;
};

#endif

