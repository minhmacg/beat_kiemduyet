#ifndef NNM_BEAT_PAGEMEDIA_H
#define NNM_BEAT_PAGEMEDIA_H

#include <iostream>
#include <vector>
#include <filesystem>

struct page
{
	std::string _pagename;
	std::vector<std::string> _medias;
	std::string photo_folder_path() const {return _pagename + "/photos";}; 
	std::string capsrc_folder_path()const {return _pagename + "/cap_source";}; 
	std::string video_folder_path() const {return _pagename + "/videos";}; 
};

inline std::vector<std::string> medias_per_folder(const std::filesystem::path& folder)
{
	std::vector<std::string> rs;
	std::cout << folder << '\n';
	for (const auto& media: std::filesystem::directory_iterator(folder))
	{
		if (std::filesystem::is_regular_file(media))
			rs.push_back(media.path().filename());
	};
	return rs;
};

inline std::vector<page> fetch_page_medias(const std::filesystem::path& folder)
{
	std::vector<page> rs;
	for (const auto& p: std::filesystem::directory_iterator(folder))
	{
		if (std::filesystem::is_directory(p))
		{
			page current_page {p.path().stem()};
			std::cout<< current_page._pagename << '\n';
			if (std::filesystem::exists(folder / current_page.photo_folder_path()))
				for (const auto& img: medias_per_folder(folder / current_page.photo_folder_path()))
					current_page._medias.push_back(img);
			if (std::filesystem::exists(folder / current_page.video_folder_path()))
				for (const auto& vid: medias_per_folder(folder / current_page.video_folder_path()))
					current_page._medias.push_back(vid);

			rs.push_back(current_page);
		};
	};
	return rs;
};

#endif
