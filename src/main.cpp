#include <Beat.hpp>
#include <regex>
#include <iostream>
#include <filesystem>
inline std::string vec_to_string(const vec_str& v)
{
	std::string rs {'\"'};
	auto i = v.cbegin();
	while (i != v.cend())
	{
		if (!(*i).empty()) rs += *i + '\n';
		i++;
	};
	rs += '\"';
	return rs;
}
int main(int argc, const char** argv)
{
	std::ifstream f;
	
	using nlohmann::json;
	std::filesystem::path json_path {"json_src"};
	for (auto& file: std::filesystem::directory_iterator{json_path})
	{
		//std::cout << "test\n";
		std::string fp = file.path();
		f.open(file.path());
		json input_data;
		f >> input_data;
		f.close();

		std::string page_name;
		std::regex r_fname {"(\\w*).json"};
		std::smatch rm_fname;
		std::regex_search(fp,rm_fname,r_fname) ? 
			page_name = rm_fname.str(1) : page_name = "";

		std::string output_fmt {"output_tsv/output_" + page_name + ".tsv"};
		std::filesystem::create_directory("output_tsv");
		
		//std::cout << output_fmt << '\n';

		messvec rs {vec_from_input(input_data)};
		//for (auto& m: rs)
		//{
		//	std::cout << page_name << '\t' 
		//		<< m.btv << '\t' 
		//		<< m.content.substr(0,30) << '\n'
		//		<< vec_to_string((m.photo.links))
		//		<< "--------------------\n";
		//};
		std::ofstream fo;
		print_to_tsv(output_fmt, std::cout, rs, page_name);
		
		//Messages m = from_json(rs[0]);
		//std::cout << rs << "\n";
		
		//convert_time(rs);
		//join_messages(rs);
		//pages_name(rs);

		//std::time_t t {1715736514107/1000};
		//std::array<char,15> a1;
		//std::strftime(a1.data(),a1.size(),"%Y/%m/%d %Z",std::localtime(&t));
		//std::cout << a1.data();
		// Process json data
		// Export to csv, FS = "\t"
	}
}
