#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <filesystem>

using path = std::filesystem::path;
// given a list of filename. Make a directory in each file's parent directory with name "cap source", and move file into that folder

struct page
{
	path folder_path, photos_folder_path, cap_source_folder;
	std::vector<path> cap_source_org, img;
};

int main() 
{
	std::fstream f;
	std::string s;
	std::filesystem::path list, page_folder;
	std::cout << "file list to copy: ";
	std::cin >> list;
	std::cout << "inbox folder: ";
	std::cin >> page_folder;
	std::vector<page> vpf;
	if (!std::filesystem::exists(list))
	{
		std::cout << "file doesn't exist";
		return -1;
	};
	if (!std::filesystem::exists(page_folder) || !std::filesystem::is_directory(page_folder))
	{
		std::cout << "folder doesn't exist";
		return -1;
	};
	for (auto& p: std::filesystem::directory_iterator(page_folder))
		if (std::filesystem::is_directory(p))
		{
			std::cout << p.path() << '\n';
			std::filesystem::path cap_source {p.path() / "cap_source"};
			if (!std::filesystem::create_directory(cap_source)) std::cout << "existed folder\n";
			std::filesystem::path photos {p.path() / "photos"};

			if (std::filesystem::exists(photos))
			{
				page pf {.folder_path = p.path(),
					.photos_folder_path = photos,
					.cap_source_folder = cap_source};
				for (auto& img: std::filesystem::directory_iterator(photos))
				{
					if (img.is_regular_file())
					{
						pf.img.push_back(img.path());
						std::cout << img.path() << '\n';
					};
				};
				vpf.push_back(pf);
			};
		};
	for (auto& pf: vpf)
	{
		std::cout << "---------------------------------\n" 
			<< pf.folder_path << " " << pf.photos_folder_path << '\n';
		for (auto& i: pf.img)
			std::cout << i << ", ";
		std::cout << '\n';
	}
	std::vector<std::string> imgs_to_copy;
	f.open(list);
	while (std::getline(f,s))
	{
		if (!s.empty()) imgs_to_copy.push_back(s);
	}
	f.close();
	
	std::cout << "\n\n\n...............finding images..............................\n"
		<< ".....................found:\n";
	std::vector<std::string> notfound_image;
	for (auto& i: imgs_to_copy)
	{
		for (auto& page: vpf)
		{
			if (auto findimg = std::find_if
								(page.img.begin(),page.img.end(),
									[&](path& p){return p.string().find(i) != std::string::npos; });
						findimg != page.img.end())
			{
				std::cout << *findimg << '\t';
				page.cap_source_org.push_back(findimg->string());
			}
			else {notfound_image.push_back(i);};
		};
		std::cout << '\n';
	};

	std::cout << "not found: \n";
	for (auto& i: notfound_image) std::cout << i << '\n';

	std::cout << "...............................\n";
	for (auto page: vpf)
	{
		std::cout << page.folder_path << '\n';
		for (auto& cap: page.cap_source_org)
		{
			std::cout << cap << '\n';
			path dst = page.cap_source_folder / cap.filename();
			std::filesystem::copy(cap, dst);
		}
	}
};
