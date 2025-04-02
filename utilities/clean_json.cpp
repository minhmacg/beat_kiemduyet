#include "../include/json.hpp"
#include <fstream>
#include <filesystem>
#include <print>
#include <iostream>
#include <set>
inline void help()
{
	std::println("Usage clean_json -i <inputfile>");
};
int main(int argc, const char** argv)
{
	if (argc != 3 || std::string_view(argv[1]) != "-i")
	{
		help();
		throw std::invalid_argument("invalid argument");
	};

	if (!std::filesystem::exists(std::string_view(argv[2])))
		throw std::invalid_argument("file doesn't exist");
	
	std::ifstream f (argv[2]);
	nlohmann::json json,rs;
	f >> json;
	const auto size = json.size();
	auto test_user = [](const std::string_view& user)
	{
		return user.empty() || user == "Mở ảnh";
	};
	for (int i {0}; i != size; i++)
	{
		std::cerr << "Processing " << i + 1 << "/" << size << std::endl;
		auto& user = json[i]["user"];
		if (test_user(user.get<std::string>()))
		{
			for (int j = i + 1; j != size; j++)
			{
				const auto& next_user = json[j]["user"];
				if (!test_user(next_user.get<std::string>()))
				{
					user = next_user;
					break;
				};
			};
		};
	};

	int dupcount {0};
	std::set<nlohmann::json> seen;
	for (const auto& j: json)
	{
		if (seen.insert(j).second)
			rs.push_back(j);
		else dupcount++;
	};
	
	std::cout << rs.dump(4) << std::endl;
	std::cerr << "Dup count : " << dupcount << std::endl;

};
