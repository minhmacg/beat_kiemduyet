#include "page_media.hpp"
#include <fstream>
int main(int argc, const char** argv)
{
	std::filesystem::path filelist, src_folder;
	
	for (int i = 0; i != argc; i++)
	{
		if (i + 1 != argc)
		{
			if (std::string(argv[i]) == "-l") filelist = std::filesystem::path{argv[i+1]};
			if (std::string(argv[i]) == "-f") src_folder = std::filesystem::path{argv[i+1]};
		};
	};
	if (!std::filesystem::exists(filelist))
	{
		std::cout << "file doesn't exist";
		return -1;
	};
	if (!std::filesystem::exists(src_folder) || !std::filesystem::is_directory(src_folder))
	{
		std::cout << "folder doesn't exist";
		return -1;
	};

	std::vector<page> vpf {fetch_page_medias(src_folder)};

	for (auto& p: vpf)
	{
		std::filesystem::create_directories(src_folder / p.capsrc_folder_path());
	};

	std::ifstream f;
	std::string s;
	f.open(filelist);

	while (std::getline(f,s))
	{
		for (const auto& page: vpf)
		{
			for (const auto& img: page._photos)
			{
				if (img.find(s) != std::string::npos)
				{
					std::cout << "found " << src_folder / page.photo_folder_path() / img << '\n';
					if (!std::filesystem::exists(src_folder / page.capsrc_folder_path() / img))
					{
						std::filesystem::copy(src_folder / page.photo_folder_path() / img,
											src_folder / page.capsrc_folder_path() / img);
						std::cout << std::boolalpha << std::filesystem::remove(src_folder / page.photo_folder_path() / img) << '\n';
					};
				};
			}; 
		};
	};


};
