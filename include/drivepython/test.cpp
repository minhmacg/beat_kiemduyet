#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <vector>

std::vector<std::string> split(const std::string& input, const char delim)
{
	std::stringstream ss {input};
	std::string line;
	std::vector<std::string> rs;
	while (std::getline(ss,line,delim))
	{
		rs.push_back(line);
	};
	return rs;
};
int main()
{
	std::ifstream f;
	std::ofstream o;
	f.open("drive.csv");
	o.open("drive_links.txt");
	std::string s;
	while (std::getline(f,s))
	{
		std::size_t p = s.find('\t');
		std::string x,y;
		std::vector<std::string> output = split(s,'\t');
		for (const auto& i: output)
		{
			std::cout << i << 'n';
		};
		std::cout << '\n';
		x = s.substr(0,p);
		y = s.substr(p+1);
		std::cout << "name " << x << " ; link " << y << '\n';
		o << x << '\t' << y << '\n';
	};
};
