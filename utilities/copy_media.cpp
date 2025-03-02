#include <fstream>
#include <algorithm>

#include "page_media.hpp"
using path = std::filesystem::path;
// given a filelist of filename. Make a directory in each file's parent directory with name "cap source", and move file into that folder


int main(int argc, const char** argv) 
{
	std::filesystem::path filelist, src_folder, output_folder;
	
	for (int i = 0; i != argc; i++)
	{
		if (i + 1 != argc)
		{
			if (std::string(argv[i]) == "-l") filelist = std::filesystem::path{argv[i+1]};
			if (std::string(argv[i]) == "-f") src_folder = std::filesystem::path{argv[i+1]};
			if (std::string(argv[i]) == "-t") output_folder = std::filesystem::path{argv[i+1]};
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
		std::filesystem::create_directories(output_folder / p.photo_folder_path());
		std::filesystem::create_directories(output_folder / p.video_folder_path());
		std::filesystem::create_directories(output_folder / p.capsrc_folder_path());
	};
	//for (auto& pf: vpf)
	//{
	//	std::cout << "---------------------------------\n" 
	//		<< pf.folder_path << " " << pf.photos_folder_path << '\n';
	//	for (auto& i: pf.img)
	//		std::cout << i << ", ";
	//	std::cout << '\n';
	//}
	
	std::fstream f;
	std::string s;
	std::vector<std::string> imgs_to_copy;
	std::vector<std::string> notfound_image;

	f.open(filelist);
	while (std::getline(f,s)) if (!s.empty()) imgs_to_copy.push_back(s);
	
	std::cout << "\n\n\n...............finding images..............................\n"
		<< ".....................found:\n";
	for (auto& i: imgs_to_copy)
	{
		for (auto& page: vpf)
		{
			if (auto findimg = std::find_if
								(page._medias.begin(),page._medias.end(), [&](auto&& p){return p == i;});
						findimg!= page._medias.end())
			{
				std::filesystem::path from, to;
				if (findimg->find(".mp4") != std::string::npos)
				{
					from = src_folder / page.video_folder_path() / *findimg;
					to = output_folder / page.video_folder_path() / *findimg;
				}
				else
				{
					from = src_folder / page.photo_folder_path() / *findimg;
					to = output_folder / page.photo_folder_path() / *findimg;
				};
				
				std::cout << from << '\n';
				std::filesystem::rename(from, to);
				//std::string newimg {findimg->string()};
				//std::string to_replace {"/tmp_t8t9/inbox/"};
				//newimg.replace(newimg.find(to_replace), to_replace.size(), "/t8/");
				//std::cout << *findimg << '\n';
				//std::cout << newimg << '\n';
				//page.cap_source_org.push_back(findimg->string());
			}
			else {notfound_image.push_back(i);};
		};
	};

	//std::cout << "not found: \n";
	//for (auto& i: notfound_image) std::cout << i << '\n';

	//std::cout << "...............................\n";
	//for (auto page: vpf)
	//{
	//	std::cout << page.folder_path << '\n';
	//	for (auto& cap: page.cap_source_org)
	//	{
	//		std::cout << cap << '\n';
	//		path dst = page.cap_source_folder / cap.filename();
	//		std::filesystem::copy(cap, dst);
	//	}
	//}
};
