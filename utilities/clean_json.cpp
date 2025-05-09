#include "../include/json.hpp"
#include <fstream>
#include <filesystem>
#include <print>
#include <iostream>
#include <set>
#include <regex>
inline void help()
{
	std::println("Usage clean_json -i <inputfile> [--filter]");
};
int main(int argc, const char** argv)
{
	if (argc < 3 || std::string_view(argv[1]) != "-i")
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
	int dupcount {0};
	std::set<nlohmann::json> seen;
	for (const auto& j: json)
	{
		const auto user = j["user"].get<std::string>();
		if (seen.insert(j).second)
		{
			if ((argc == 4 && std::string_view{argv[3]} == "--filter") && user == "Tuấn Dũng" || user == "Đức Bách" || user == "Minh Vũ" || j.at("reactions").contains("👍"))
				rs.push_back(j);
			rs.push_back(j);
		}
		else dupcount++;
	};

	std::cerr << "clear " << dupcount << "duplicates\n";
	std::string kdv {""};	
	for (int i {0}; i != rs.size(); i++)
	{
		std::cerr << "Processing " << i + 1 << "/" << size << std::endl;
		auto user = rs[i]["user"];
		if (user.is_null() || user == "User N/A") rs[i]["user"] = "";
		if (argc == 4 && std::string_view{argv[3]} == "--filter")
			if (user == "Tuấn Dũng" || user == "Đức Bách" || user == "Minh Vũ")
			{
				//for (int j = i + 1; j != size; j++)
				//{
				//	const auto& next_user = json[j]["user"];
				//	if (!test_user(next_user.get<std::string>()))
				//	{
				//		user = next_user;
				//		break;
				//	};
				//};

				kdv = user;
				if (i > 0) rs[i-1]["kdv"] = kdv;
			};
			rs[i]["kdv"] = kdv;
			rs[i]["user"] = std::regex_replace(rs[i]["user"].get<std::string>(), std::regex{" đã gửi \\d* ảnh"},"");
			rs[i]["user"] = std::regex_replace(rs[i]["user"].get<std::string>(), std::regex{"Icon for this message.*"},"");
	};

	std::cout << rs.dump(4) << std::endl;
	std::cerr << "Dup count : " << dupcount << std::endl;

};
