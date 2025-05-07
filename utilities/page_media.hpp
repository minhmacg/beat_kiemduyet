#ifndef NNM_BEAT_PAGEMEDIA_H
#define NNM_BEAT_PAGEMEDIA_H

#include <iostream>
#include <vector>
#include <filesystem>
#include <set>
#include <print>
struct page
{
	std::string _pagename;
	std::set<std::string> _photos;
	std::set<std::string> _videos;
	std::string photo_folder_path() const {return _pagename + "/photos";}; 
	std::string capsrc_folder_path()const {return _pagename + "/cap_source";}; 
	std::string video_folder_path() const {return _pagename + "/videos";}; 

	std::optional<std::string> find_media(const std::string& name) const
	{
		if (auto f = std::ranges::find_if(_photos, [&](const auto& p){return p.find(name) != std::string::npos;}); f != _photos.end())
			return photo_folder_path() + "/" + *f;
		else if (auto f = std::ranges::find_if(_videos, [&](const auto& v){return v.find(name) != std::string::npos;}); f != _videos.end())
			return video_folder_path() + "/" + *f;
		else
			return std::nullopt;
	};
};

inline std::vector<std::string> medias_per_folder(const std::filesystem::path& folder)
{
	std::vector<std::string> rs;
	std::cout << folder << '\n';
	for (const auto& media: std::filesystem::directory_iterator(folder))
		if (std::filesystem::is_regular_file(media))
			rs.push_back(media.path().filename().string());
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
				{
					std::cout << img << '\n';
					current_page._photos.emplace(img);
				};
			if (std::filesystem::exists(folder / current_page.video_folder_path()))
				for (const auto& vid: medias_per_folder(folder / current_page.video_folder_path()))
					current_page._videos.emplace(vid);

			rs.push_back(current_page);
		};
	};
	return rs;
};

#endif
