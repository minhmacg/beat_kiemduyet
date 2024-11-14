#include <Beat.hpp>
#include <ranges>
#include <regex>

using nlohmann::json;

inline std::string convert_time(const std::time_t& t)
{
	std::time_t seconds = t/1000;
	char time[15];
	std::strftime(time,sizeof(time),"%F",std::localtime(&seconds));
	return time;
}

inline vec_str media_code(const vec_str& vs)
{
	vec_str rs;
	std::regex rg {"([^/]*)$"};
	std::smatch rm;
	for (auto& str: vs)
		if (std::regex_search(str,rm,rg)) rs.push_back(rm.str(1));
	return rs;
}

inline std::string vec_to_string(const vec_str& v)
{
	std::string rs {'\''};
	auto i = v.cbegin();
	while (i != v.cend())
	{
		if (!i->empty()) rs += *i;
		if (i + 1 != v.cend()) rs += SS;
		i++;
	};
	rs += '\'';
	std::ranges::replace(rs, ',', '\n');
	return rs;
}
//inline vec_str map_link(const vec_str& vs, const drivemap& dm)
//{
//	vec_str rs;
//	for (auto& str: vs) 
//		dm.find(str) != dm.end() ?
//			rs.push_back(dm.at(str)) :
//			rs.push_back("");
//	return rs;
//}

inline std::string drive_thumbnail(const std::string& link)
{
	std::string rs;
	std::regex rg {"d/([^/]*)"};
	std::smatch rm;
	if (std::regex_search(link,rm,rg))
		rs = "https://drive.google.com/thumbnail?id=" + rm.str(1);
	else rs = "";
	return rs;
}

messvec vec_from_input(const json& js)
{
	messvec rs;
	for (const auto& i: js)
		rs.push_back(from_json(i));
	return rs;
}

Messages from_json(const json& js)
{
	Messages rs{};
	rs.time = js.at("timestamp_ms");
	rs.page = "";
	std::string sname = js.at("sender_name");
	rs.btv = btv.contains(sname) ? btv.at(sname) : sname;
	try
	{
		rs.content = js.at("content");
		std::ranges::replace(rs.content, '\n', ' ');
		std::ranges::replace(rs.content, '\t', ' ');
		std::ranges::replace(rs.content, '\r', ' ');
	}
	catch (json::out_of_range& err) {};
	//
	try 
	{
		for(auto& p: js.at("photos"))
			rs.photo.links.push_back(p.at("uri"));
	}
	catch (json::out_of_range& err) {};
	//
	try 
	{
		for (auto& v: js.at("videos"))
			rs.video.links.push_back(v.at("uri"));
	}
	catch (json::out_of_range& err) {};
	//
	try {rs.link = js.at("share").at("link");}
	catch (json::out_of_range& err) {};
	//
	try 
	{
		for (auto& react: js.at("reactions")) 
			rs.reactions.push_back({react["reaction"],react["actor"]});
	}
	catch (json::out_of_range& err) {};
	
	return rs;
}


messvec& only_reactions(messvec& m)
{
	auto [f,l] = std::ranges::remove_if(m, 
			[](auto&& r){return r.empty();},
			&Messages::reactions);
	m.erase(f,l);
	return m;
};

messvec& join_messages(messvec& m, const std::string& pn)
{
	auto it = m.begin();
	if (pn == "tiktok")
	{
		while (it + 1 != m.end())
		{
			auto it2 = it + 1;
			std::time_t dt = it->time - it2->time;
			if (it->btv == it2->btv && dt < 50000)
			{
				if (it->link.empty() && !it2->link.empty())
				{
					it2->content = it->content;
					it = m.erase(it);
				}
				else if (!it->link.empty() && it2->link.empty())
				{
					it2->link = it->link;
					it = m.erase(it);
				}
				else ++it;
			}
			else ++it;
		}
	}
	else
	{
		while (it + 1 != m.end())
		{
			auto it2 = it + 1;
			std::time_t dt = it->time - it2->time;
			if (!it->reactions.empty() && !it2->reactions.empty()
				&& it->btv == it2->btv 
				&& dt < 50000)
			{
				if (it->content.empty() && !it2->content.empty())
				{
					if (!it->photo.links.empty() && it2->photo.links.empty())
					{
						it2->photo = it->photo;
						it = m.erase(it);
					}
					else if (!it->video.links.empty() && it2->video.links.empty())
					{
						it2->video = it->video;
						it = m.erase(it);
					}
					else ++it;
				}
				else if (!it->content.empty() && it2->content.empty())
				{
					if (it->photo.links.empty() && !it2->photo.links.empty())
					{
						it2->content = it->content;
						it = m.erase(it);
					}
					else if (it->video.links.empty() && !it2->video.links.empty())
					{
						it2->content = it->content;
						it = m.erase(it);
					}
					else ++it;
				}
				else ++it;
			}
			else ++it;
		};
	}
	return m;
}

std::tuple<std::string, std::string> get_censor_rs(const auto& reactions)
{
	auto validkdv = [](const std::string& name)
	{
		return name == "Thành Đạt" || name == "Đức Bách" || name == "Tuấn Dũng";
	};
	auto convertname = [](const std::string& name)
	{
		if (name == "Thành Đạt") return "Đạt";
		if (name == "Đức Bách") return "Bách";
		return "";
	};

	std::tuple<std::string, std::string> rs = {"Duyệt", ""};
	auto& [kq,kdv] = rs;
	if (auto check_kdv = std::ranges::find_if(reactions, validkdv, &Messages::reaction::actor);
			check_kdv != reactions.end())
	{
		kdv = convertname(check_kdv->actor);
		if (check_kdv->react == "😮") kq = "Sửa/ xóa bài - cơ bản";
	};

	return rs;
};


std::string page_map_f(const std::string& pn, 
		const Messages& m)
{
	std::string rs;
	if (pn != "tiktok") return page_map.at(pn);
	if (m.btv == "Đào Xuân Ánh"
		|| m.btv == "Lưu Viết Hoàng"
		|| m.btv == "Tống Bùi Vĩnh Hoàng"
		|| m.btv == "Nguyễn Thị Kiều Khanh"
		|| m.btv == "Vũ Văn Khánh") 
	{
		rs = "TT - BEATVN";
		if (m.content.find("@Linh Phương") != std::string::npos) 
			rs = "TT - HelloVietnam";
	}
	if (m.btv == "Phạm Hồ Linh Phương" || m.btv == "Ngô Tiến Dũng") rs = "TT - HelloVietnam";
	if (m.btv == "Bùi Quang Hiếu" || m.btv == "Nguyễn Đồng Tường") 
		rs = "TT - Beatvn Viral World";
	if (m.btv == "Đỗ Thị Lệ") rs = "TT - SHOWBEAT";
	if (m.btv == "Nguyễn Song An") rs = "TT - BEAT the Game";
	
	return rs;
}

void print_to_tsv(const std::string& title, std::ofstream& f,
		const messvec& mv,
		const std::string& page_name)
{
	auto print = [&](std::initializer_list<std::string> args)
	{
		for (auto& i: args)
			f << i << FS;
		f << '\n';
	};
	f.open(title);
	print({"time","page","btv","content","photo","video","link",
			"cap source","kdv","kq","cmt"});
	for (auto& m: mv)
	{
		auto [kq, kdv] = get_censor_rs(m.reactions); 
		print({ convert_time(m.time), page_map_f(page_name,m), 
				m.btv, m.content,
				vec_to_string(media_code(m.photo.links)),
				vec_to_string(media_code(m.video.links)),
				m.link, "", kdv, kq, ""});
	}
}


std::unordered_map<std::string, std::string> fetch_data(const std::filesystem::path& path)
{
	std::ifstream f {path};
	std::string line;
	
	std::unordered_map<std::string, std::string> rs;
	while (std::getline(f,line))
	{
		if (auto commapos = line.find(','); commapos != std::string::npos)
		{
			std::string fbname = line.substr(0, commapos);
			std::string realname = line.substr(commapos + 1);

			rs[fbname] = realname;
		};
	};

	return rs;
};

