#include "../include/json.hpp"
#include <vector>
#include <fstream>
#include <filesystem>
#include <print>
#include <iostream>
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
	nlohmann::json json;
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
	
	std::cout << json.dump(4);

};
