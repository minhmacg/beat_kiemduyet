#include <Beat.hpp>
#include <regex>

using nlohmann::json;

//inline std::string convert_time(const std::time_t& t)
//{
//	std::time_t seconds = t/1000;
//	char time[15];
//	std::strftime(time,sizeof(time),"%F",std::localtime(&seconds));
//	return time;
//}

inline std::string media_code(const std::string& vs)
{
	std::regex rg {"[^\\W]*\\.(jpg|png|mp4)"};
	std::smatch rm;
	std::string rs;
	if (std::regex_search(vs, rm, rg)) rs = rm.str();
	return rs;
}

vec_str get_links(const std::string& input)
{
	vec_str rs;
	std::regex rg(R"(https?://[^\s'",<>{}[\]()\\|^]+)");
	std::smatch matches;
    std::sregex_iterator it(input.begin(), input.end(), rg);
    std::sregex_iterator end;

    while (it != end) {
        rs.push_back(it->str());
        ++it; 
    }
	return rs;
};
std::string vec_to_string(const vec_str& v)
{
	if (v.size() == 1) return *v.begin();
	if (v.size() == 0) return {};

	std::string rs {'\"'};
	auto i = v.cbegin();
	while (i != v.cend())
	{
		if (!i->empty()) rs += *i;
		if (i + 1 != v.cend()) rs += SS;
		i++;
	};
	rs += '\"';
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

//inline std::string drive_thumbnail(const std::string& link)
//{
//	std::string rs;
//	std::regex rg {"d/([^/]*)"};
//	std::smatch rm;
//	if (std::regex_search(link,rm,rg))
//		rs = "https://drive.google.com/thumbnail?id=" + rm.str(1);
//	else rs = "";
//	return rs;
//}

messvec& join_messages(messvec& m)
{
	for (int i{0}; i!= m.size(); i++)
	{
		auto& m1 {m[i]};
		if (!m1.content.empty())
		{
			int c {1};
			while (c < 3)
			{
				if (i - c >= 0)
				{
					auto& m2 {m[i-c]};
					if ((m2.btv == m1.btv) && m2.content.empty())
					{
						if(!m2.photo.empty())
						{
							m1.photo = std::move(m2.photo);
							break;
						};
						if (!m2.video.empty())
						{
							m1.video = std::move(m2.video);
							break;
						};
					};
				};
				if (i + c < m.size())
				{
					auto& m2 {m[i+c]};
					if ((m2.btv == m1.btv) && m2.content.empty())
					{
						if(!m2.photo.empty())
						{
							m1.photo = std::move(m2.photo);
							break;
						};
						if (!m2.video.empty())
						{
							m1.video = std::move(m2.video);
							break;
						};
					};
				};
				c++;
			};
		};
	};
	m.erase(std::remove_if(m.begin(), m.end(), [](const auto& msg)
			{
				return msg.content.empty() && msg.photo.empty() && msg.video.empty();
			}), m.end());

	return m;
};
//messvec& filter_message(messvec& m)
//{
//	auto [f,l] = std::ranges::remove_if(m, 
//			[](auto&& r){return !r.has_react && r.links.empty();});
//	m.erase(f,l);
//	return m;
//};
messvec vec_from_input(const json& js)
{
	messvec rs;
	for (const auto& i: js)
		rs.push_back(from_json(i));
	join_messages(rs);
	rs.erase(std::remove_if(rs.begin(), rs.end(), [](auto&& m)
			{
				return !m.has_react && m.links.empty() || ((m.btv == "Đức Bách" || m.btv == "Tuấn Dũng") && (m.photo.empty()));
			}), rs.end());
	return rs;
}

Messages from_json(const json& js)
{
	Messages rs{};
	std::string sname = js.at("user");
	rs.btv = btv.contains(sname) ? btv.at(sname) : sname;
	try
	{
		rs.content = js.at("content");
		std::ranges::replace(rs.content, '\n', ' ');
		std::ranges::replace(rs.content, '\t', '	');
		std::ranges::replace(rs.content, '\r', ' ');
		rs.links = get_links(rs.content);
	}
	catch (json::out_of_range& err) {};
	//
	try 
	{
		for(auto& p: js.at("media"))
		{
			if (p["type"] == "image")
				rs.photo.push_back(media_code(p.at("src")));
			if (p["type"] == "video")
				rs.video.push_back(media_code(p.at("src")));
		};
	}
	catch (json::out_of_range& err) {};
	try
	{
		if (js.at("react").size() > 0) rs.has_react = true;
	}
	catch (json::out_of_range& err) {};
	//
	return rs;
}



//messvec& join_messages(messvec& m, const std::string& pn)
//{
//	auto it = m.begin();
//	if (pn == "tiktok")
//	{
//		while (it + 1 != m.end())
//		{
//			auto it2 = it + 1;
//			std::time_t dt = it->time - it2->time;
//			if (it->btv == it2->btv && dt < 50000)
//			{
//				if (it->link.empty() && !it2->link.empty())
//				{
//					it2->content = it->content;
//					it = m.erase(it);
//				}
//				else if (!it->link.empty() && it2->link.empty())
//				{
//					it2->link = it->link;
//					it = m.erase(it);
//				}
//				else ++it;
//			}
//			else ++it;
//		}
//	}
//	else
//	{
//		while (it + 1 != m.end())
//		{
//			auto it2 = it + 1;
//			std::time_t dt = it->time - it2->time;
//			if (!it->reactions.empty() && !it2->reactions.empty()
//				&& it->btv == it2->btv 
//				&& dt < 50000)
//			{
//				if (it->content.empty() && !it2->content.empty())
//				{
//					if (!it->photo.empty() && it2->photo.empty())
//					{
//						it2->photo = it->photo;
//						it = m.erase(it);
//					}
//					else if (!it->video.empty() && it2->video.empty())
//					{
//						it2->video = it->video;
//						it = m.erase(it);
//					}
//					else ++it;
//				}
//				else if (!it->content.empty() && it2->content.empty())
//				{
//					if (it->photo.empty() && !it2->photo.empty())
//					{
//						it2->content = it->content;
//						it = m.erase(it);
//					}
//					else if (it->video.empty() && !it2->video.empty())
//					{
//						it2->content = it->content;
//						it = m.erase(it);
//					}
//					else ++it;
//				}
//				else ++it;
//			}
//			else ++it;
//		};
//	}
//	return m;
//}

//std::tuple<std::string, std::string> get_censor_rs(const auto& reactions)
//{
//	auto validkdv = [](const std::string& name)
//	{
//		return name == "Thành Đạt" || name == "Đức Bách" || name == "Tuấn Dũng" || name == "Phong Truong";
//	};
//	auto convertname = [](const std::string& name)
//	{
//		if (name == "Phong Truong") return "Phong";
//		if (name == "Thành Đạt") return "Đạt";
//		if (name == "Đức Bách") return "Bách";
//		return "";
//	};
//
//	std::tuple<std::string, std::string> rs = {"Duyệt", ""};
//	auto& [kq,kdv] = rs;
//	if (auto check_kdv = std::ranges::find_if(reactions, validkdv, &Messages::reaction::actor);
//			check_kdv != reactions.end())
//	{
//		kdv = convertname(check_kdv->actor);
//		if (check_kdv->react == "😮") kq = "Sửa/ xóa bài - cơ bản";
//	};
//
//	return rs;
//};


//std::string page_map_f(const std::string& pn, 
//		const Messages& m)
//{
//	std::string rs;
//	if (pn != "tiktok") return page_map.at(pn);
//	if (m.btv == "Đào Xuân Ánh"
//		|| m.btv == "Lưu Viết Hoàng"
//		|| m.btv == "Tống Bùi Vĩnh Hoàng"
//		|| m.btv == "Nguyễn Thị Kiều Khanh"
//		|| m.btv == "Vũ Văn Khánh") 
//	{
//		rs = "TT - BEATVN";
//		if (m.content.find("@Linh Phương") != std::string::npos) 
//			rs = "TT - HelloVietnam";
//	}
//	if (m.btv == "Phạm Hồ Linh Phương" || m.btv == "Ngô Tiến Dũng") rs = "TT - HelloVietnam";
//	if (m.btv == "Bùi Quang Hiếu" || m.btv == "Nguyễn Đồng Tường") 
//		rs = "TT - Beatvn Viral World";
//	if (m.btv == "Đỗ Thị Lệ") rs = "TT - SHOWBEAT";
//	if (m.btv == "Nguyễn Song An") rs = "TT - BEAT the Game";
//	
//	return rs;
//}

void print_to_tsv(const std::string& title, std::ostream& f,
		const messvec& mv,
		const std::string& page_name)
{
	auto print = [&](std::initializer_list<std::string> args)
	{
		for (auto& i: args)
			f << i << FS;
		f << '\n';
	};
	print({"time","page","btv","content","photo","video","link",
			"cap source","kdv","kq","cmt"});

	for (auto& m: mv)
	{
		print({"", page_map.at(page_name), 
				m.btv, m.content,
				vec_to_string(m.photo),
				vec_to_string(m.video),
				vec_to_string(m.links)});
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

