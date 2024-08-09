#include <Beat.hpp>
using nlohmann::json;

inline std::string convert_time(std::time_t& t)
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
	std::string rs {'\"'};
	auto i = v.cbegin();
	while (i != v.cend())
	{
		if (!(*i).empty()) rs += *i;
		if (i + 1 != v.cend()) rs += '\n';
		i++;
	};
	rs += '\"';
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
	{
		rs.push_back(std::make_shared<Messages>(from_json(i)));
	};
	return rs;
}

Messages from_json(const json& js)
{
	Messages rs{};
	rs.time = js.at("timestamp_ms");
	rs.page = "";
	rs.btv = js.at("sender_name");
	try
	{
		rs.content = js.at("content");
		std::replace(rs.content.begin(),rs.content.end(),'\n',' ');
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
			if (react["actor"] == "Thành Đạt") rs.reaction = {true,"Đạt"};
			else if (react["actor"] == "Đức Bách") rs.reaction = {true,"Bách"};
			else rs.reaction = {true,""};
	}
	catch (json::out_of_range& err) {};
	rs.kq = "Duyệt";
	rs.cmt = "";

	return rs;
}


messvec& only_reactions(messvec& m, const std::string& pn)
{
	auto it = m.begin();
	while (it != m.end())
	{
		if (!std::get<bool>((*it)->reaction)) it = m.erase(it);
		else ++it;
	}
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
			std::time_t dt = (*it)->time - (*it2)->time;
			if ((*it)->btv == (*it2)->btv
				&&	dt < 50000)
			{
				if ((*it)->link.empty() 
					&& !(*it2)->link.empty())
				{
					(*it2)->content = (*it)->content;
					it = m.erase(it);
				}
				else if (!(*it)->link.empty()
					&& (*it2)->link.empty())
				{
					(*it2)->link = (*it)->link;
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
			std::time_t dt = (*it)->time - (*it2)->time;
			if ((*it)->btv == (*it2)->btv
				&&	dt < 50000)
			{
				if ((*it)->content.empty() 
					&& !(*it2)->content.empty())
				{
					if (!(*it)->photo.links.empty()
						&& (*it2)->photo.links.empty())
					{
						(*it2)->photo = (*it)->photo;
						it = m.erase(it);
					}
					else if (!(*it)->video.links.empty()
						&& (*it2)->video.links.empty())
					{
						(*it2)->video = (*it)->video;
						it = m.erase(it);
					}
					else ++it;
				}
				else if (!(*it)->content.empty() 
					&& (*it2)->content.empty())
				{
					if ((*it)->photo.links.empty()
						&& !(*it2)->photo.links.empty())
					{
						(*it2)->content = (*it)->content;
						it = m.erase(it);
					}
					else if ((*it)->video.links.empty()
						&& !(*it2)->video.links.empty())
					{
						(*it2)->content = (*it)->content;
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

void print_to_tsv(const std::string title, std::ofstream& f,
		const messvec& mv,
		const std::string& page_name)
{
	auto print = [&f](std::initializer_list<std::string> args)
	{
		for (auto& i: args)
			f << i << FS;
		f << '\n';
	};
	f.open(title);
	print({"time","page","btv","content","photo","video","link",
			"cap source","thumbnail drive","kdv","kq","cmt"});
	for (auto& m: mv)
	{
		print({ convert_time(m->time), page_map_f(page_name,m), 
				m->btv, m->content,
				vec_to_string(media_code(m->photo.links)),
				vec_to_string(media_code(m->video.links)),
				m->link, "", drive_thumbnail(m->link),
				std::get<1>(m->reaction), m->kq, m->cmt
			});
	}
	f.close();
}


