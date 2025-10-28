#include <Beat.hpp>
#include <regex>
#include <iostream>
#include <print>

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

vec_str get_links(std::string& input)
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
	for (auto& str: rs)
	{
		input.erase(input.find(str), str.length());
	};
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
		if (i + 1 != v.cend()) rs += ' ';
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
			std::string btv = m1.btv;
			int c {1};
			while (c < 3)
			{
				if (i - c >= 0)
				{
					auto& m2 {m[i-c]};
					btv = !m2.btv.empty() ? m2.btv : btv; 
					if (m2.timestamp == m1.timestamp && (m2.btv == m1.btv || m1.btv == "" || m2.btv == "") && m2.content.empty())
					{
						if(!m2.photo.empty())
						{
							m1.btv = btv;
							m1.photo = std::move(m2.photo);
							break;
						};
						if (!m2.video.empty())
						{
							m1.btv = btv;
							m1.video = std::move(m2.video);
							break;
						};
					};
				};
				if (i + c < m.size())
				{
					auto& m2 {m[i+c]};
					btv = !m2.btv.empty() ? m2.btv : btv; 
					if (m2.timestamp == m1.timestamp && (m2.btv == m1.btv || m1.btv == "" || m2.btv == "") && m2.content.empty())
					{
						if(!m2.photo.empty())
						{
							m1.btv = btv;
							m1.photo = std::move(m2.photo);
							break;
						};
						if (!m2.video.empty())
						{
							m1.btv = btv;
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
				return msg.btv.empty() || (msg.content.empty() && msg.photo.empty() && msg.video.empty() && msg.links.empty());
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

messvec vec_from_input(const json& js, const std::string& pagename)
{
	messvec rs;
	for (const auto& i: js)
		rs.push_back(from_json(i, pagename));
	std::string current_time;
	for (auto& i: rs)
	{
		if (!i.timestamp.empty()) current_time = i.timestamp;
		else if (!current_time.empty()) i.timestamp = current_time;
	};
	rs.erase(std::remove_if(rs.begin(), rs.end(), [](auto&& m)
			{
				return !m.has_like && m.btv != "Tuấn Dũng" && m.btv != "Minh Vũ" && m.links.empty();
			}), rs.end());
	std::ifstream f {"kd_per_page.json"};
	nlohmann::json kdv_per_page;
	f >> kdv_per_page;

	std::string kdv;
	for (const auto& [kd,page]: kdv_per_page.items())
		if (page_map.contains(pagename))
			if (std::find(page.begin(), page.end(), page_map.at(pagename)) != page.end()) kdv = kd;
	//Assign kdv to all messages
	
	for (auto& m: rs) m.kdv = kdv;
	join_messages(rs);
	return rs;
}

std::string clean_name(const std::string& name, const std::string& pagename)
{
	static std::unordered_map<std::string, std::unordered_map<std::string, std::string>> map {
   {"beatvn", {
        {"Bách", "Đức Bách"},
        {"Chinh", "Chinh Nguyệt"},
        {"Thịnh", "Nguyễn Tuấn Thịnh"},
        {"VHM Tuấn Thịnh", "Nguyễn Tuấn Thịnh"},
        {"Long", "Nguyễn Hoàng Long"},
        {"Huyền", "Nguyễn Thị Ngọc Huyền"},
        {"Triệu Tử Long", "Nguyễn Hoàng Long"},
        {"Dũng", "Tuấn Dũng"},
        {"Hoang", "Luu Viet Hoang"},
        {"Hồng Dương", "Hồng Dương Lê"},
        {"Nguyễn", "Nguyễn Đức Trọng"},
        {"Thần đồng nhạc chế", "Hồng Dương Lê"},
        {"Dương lê", "Lê Thị Thùy Dương"},
        {"Lệ", "Lệ Đỗ"},
        {"Quân", "Hồng Quân"},
        {"Phươn", "Linh Phương"},
        {"Tung", "Tung Doan"},
        {"Việt", "Đoàn Việt"}
    }},
    {"beatnow", {
        {"Bách", "Đức Bách"},
        {"Dũng", "Tuấn Dũng"},
        {"Hằng", "Minh Hằng"},
        {"Nguyễn", "Nguyễn Đức Hậu"},
        {"Viet Anh", "Nguyen Viet Anh"},
        {"Uyên", "Nguyễn Thu Uyên"},
        {"Đức", "Minh Đức"}
    }},
    {"cchn", {
        {"Bách", "Đức Bách"},
        {"Dinh Zin", "Tuan Dinh Zin"},
        {"Dũng", "Tuấn Dũng"},
        {"Giang", "Giang Uyên"},
        {"Uyên", "Giang Uyên"},
        {"Hoang", "Hoang Tuấn Anh"},
        {"Huong", "Le Thu Huong"},
        {"Linh", "Phạm Thùy Linh"},
        {"Minh", "Minh Khuê Lê"},
        {"Minh Khuê", "Minh Khuê Lê"},
        {"Trang", "Minh Trang"},
        {"Nguyễn", "Nguyễn Đức Trọng"},
        {"Hiếu", "Phạm Công Hiếu"},
        {"Hà", "Trần Thanh Hà"},
        {"vua lì đòn", "Trần Thanh Hà"} // Note: Multiple keys map to same value here
    }},
    {"ct", {
        {"Bách", "Đức Bách"},
        {"Chúi", "Văn A Chúi"},
        {"Dũng", "Dũng Nguyễn"},
        {"Huy", "Huy Ho"},
        {"Ho", "Huy Ho"},
        {"Huy Hay Hot", "Huy Ho"},
        {"Linh", "Linh Nguen"},
        {"Nhung", "Trần Thị Hồng Nhung"},
        {"Đình", "Đình Nguyên"}
    }},
    {"itb", {
        {"Anh", "Phuong Anh"},
        {"Bách", "Đức Bách"},
        {"Dũng", "Tuấn Dũng"},
        {"Giang", "Giang Uyên"},
        {"Uyên", "Giang Uyên"},
        {"Huong", "Le Thu Huong"},
        {"Minh", "Minh Vũ"},
        {"Nguyễn", "Nguyễn Đức Trọng"}
    }},
    {"kkn", {
        {"Bao-Linh", "Bao-Linh Dong"},
        {"Bách", "Đức Bách"},
        {"eel", "Vũ Thị Thu Trang"},
        {"Dũng", "Tuấn Dũng"},
        {"Huyy", "Huyy Anh"}
    }},
    {"kkn_td", {
        {"Bách", "Đức Bách"},
        {"Dũng", "Tuấn Dũng"},
        {"Huyy", "Huyy Anh"},
        {"Thu", "Thu Trang"}
    }},
    {"qc", {
        {"Bách", "Đức Bách"},
        {"Dũng", "Tuấn Dũng"},
        {"Hải", "Nguyễn Thu Hải"},
        {"Kim Ánh", "Phạm Kim Ánh"},
        {"Thu Trang", "Tô Thu Trang"},
        {"Nguyet", "Nguyễn Nguyệt Ánh"}
    }},
    {"qc2", {
        {"Bách", "Đức Bách"},
        {"Dũng", "Tuấn Dũng"},
        {"Hải", "Nguyễn Thu Hải"},
        {"Kim Ánh", "Phạm Kim Ánh"},
        {"Thu Trang", "Tô Thu Trang"},
        {"Nguyet", "Nguyễn Nguyệt Ánh"}
    }},
    {"sgn", {
        {"August", "August Tíu"},
        {"Bách", "Đức Bách"},
        {"Bánh", "Bánh Bòa"},
        {"Dũng", "Tuấn Dũng"},
        {"Minh", "Minh Vũ"},
        {"Nam", "Nam Ho Nguyen Hoang"},
        {"Ngoc Tuyen", "Le Ngoc Tuyen"},
        {"Nguyễn", "Thảo Nguyễn"},
        {"Tieu", "Quyen Tieu"},
        {"Vĩnh", "Nguyễn Thành Vĩnh"}
    }},
    {"showbeat", { // NOTE: Using the LAST definition provided in your input for this key
		{"Bách", "Đức Bách"},
		{"Dũng", "Tuấn Dũng"},
		{"Hân", "Hân Gia"},
		{"Nhơn", "Đào Hửu Nhơn"},
		{"Thanh", "Phạm Chí Thanh"},
        {"Lệ", "Lệ Đỗ"},
		{"Tung", "Tung Doan"}
    }},
    {"tt", {
        {"Bách", "Đức Bách"},
        {"Dũng", "Tuấn Dũng"},
        {"Hoang", "Luu Viet Hoang"},
        {"Huyền", "Nguyễn Huyền"},
        {"Lệ", "Lệ Đỗ"},
        {"Nguyễn", "Nguyễn Như Quỳnh"},
        {"Phương", "Linh Phương"},
        {"Thao Duyen", "Nguyen Thao Duyen"},
        {"Trà My", "Phạm Lại Trà My"}
    }},
    {"tt_btg", {
        {"An", "Nguyễn An"},
        {"Dũng", "Tuấn Dũng"},
        {"Không", "Nguyễn An"},
        {"Không Còn Gì", "Nguyễn An"}, // Note: Multiple keys map to same value here
        {"Minh", "Minh Vũ"}
    }},
    {"tt_bvw", {
        {"Minh", "Minh Vũ"},
        {"Quang", "Quang Hiếu"},
        {"Đồng Tường", "Nguyễn Đồng Tường"}
    }},
    {"tt_cchn", {
        {"Bách", "Đức Bách"},
        {"Dũng", "Tuấn Dũng"},
        {"Hiền", "Nguyễn Đức Hiền"},
        {"Hoang", "Hoang Tuấn Anh"},
        {"Minh", "Minh Trang"}
    }},
    {"tt_hh", {
        {"Son", "Son Tung Nguyen"},
        {"Trần", "Trần Hiệp"}
    }},
    {"tt_hlvn", {
        {"Bách", "Đức Bách"},
        {"Nguyễn", "Nguyễn Như Quỳnh"},
        {"Phương", "Linh Phương"},
        {"Dũng", "Ngô Tiến Dũng"}
    }},
    {"tt_htbz", {
        {"Bách", "Đức Bách"},
        {"Dương", "Dương Ng"},
        {"Lee", "Phong Lee"},
        {"Linh", "Đoàn Nhật Linh"},
        {"Minh", "Minh Vũ"},
        {"Nguyễn", "My Nguyễn"},
        {"Trần", "Trần Hiệp"}
    }},
	{"tt_showbeat", {
		{"Bách", "Đức Bách"},
		{"Dũng", "Tuấn Dũng"},
		{"Hồng", "Hà Lan Võ Hồng"},
		{"Lệ", "Lệ Đỗ"},
		{"Minh", "Minh Vũ"}
	}}};
	if (map.contains(pagename)) return map.at(pagename).contains(name) ? map.at(pagename).at(name) : name;
	else return name;
};
Messages from_json(const json& js, const std::string& pagename)
{
	Messages rs{};
	std::string sname = js.at("user");
	rs.btv = clean_name(sname, pagename);
	rs.btv = btv.contains(rs.btv) ? btv.at(rs.btv) : rs.btv;
	try
	{
		rs.content = js.at("content");
		rs.content = std::regex_replace(rs.content, std::regex{"[\t\n\r]"}, " ");
		rs.content = std::regex_replace(rs.content, std::regex{"[\\t\\n\\r]"}, " ");

		if (std::ranges::count(rs.content, '\"') % 2 != 0) rs.content += '\"';
		rs.links = get_links(rs.content);
		if (std::regex_match(rs.content, std::regex{"^http[^\\s]*$"}))
		{
			if (std::regex_match(rs.content, std::regex{"^https://drive.*"}))
			{
				rs.video.push_back(rs.content);
				rs.links.clear();
			};
			rs.content = "";
		};

		//// if this messages is reply to other message
		//auto find_tng = rs.content.find("---Tin nhắn gốc");
		//if (find_tng != std::string::npos)
		//	rs.content.insert(find_tng, " ");
	}
	catch (json::out_of_range& err) {};
	//
	try 
	{
		for(auto& p: js.at("media"))
		{
			if (p["type"] == "image")
			{
				if (p.at("src").get<std::string>().starts_with("https://scontent"))
				{
					rs.photo.push_back(media_code(p.at("src")));
				};
			};
			if (p["type"] == "video")
				rs.video.push_back(media_code(p.at("src")));
		};
	}
	catch (json::out_of_range& err) {};
	try
	{
		const auto& reacts = js.at("reactions").at("types");
		if (std::find(reacts.begin(), reacts.end(), "👍") != reacts.end()) rs.has_like = true;
	}
	catch (json::out_of_range& err) {};
	
	if (!js.at("originalRepliedMessagePreview").is_null() && js.at("originalRepliedMessagePreview").get<std::string>() != "")
	{
		rs.reply = std::string{js.at("originalRepliedMessagePreview")}.substr(17);
		rs.reply = std::regex_replace(rs.reply, std::regex{"[\\t\\n\\r]"}, " ");

		auto s = rs.reply.find("\"");
		int count {0};
		while (s != std::string::npos)
		{
			count++;
			s = rs.reply.find("\"", s+1);
		};
		if (count % 2 != 0) rs.reply += "\"";
	};
	if (js.at("timestamp") != "N/A") rs.timestamp = js.at("timestamp");
	//
	return rs;
}



messvec& join_messages(messvec& m, const std::string& pn)
{
	constexpr int WINDOW = 3;
	std::vector<std::size_t> to_erase;

	auto is_fillable = [](const auto& msg) {
		return msg.content.empty() || msg.photo.empty() || msg.video.empty() || msg.links.empty();
	};

	auto count_filled_fields = [](const auto& msg) {
		int count = 0;
		if (!msg.content.empty()) count++;
		if (!msg.photo.empty()) count++;
		if (!msg.video.empty()) count++;
		if (!msg.links.empty()) count++;
		return count;
	};

	for (std::size_t i = 0; i < m.size(); ++i)
	{
		auto& current = m[i];
		if (!is_fillable(current)) continue;

		for (int offset = -WINDOW; offset <= WINDOW; ++offset)
		{
			if (offset == 0) continue;

			std::size_t j = i + offset;
			if (offset < 0 && i < static_cast<std::size_t>(-offset)) continue;
			if (j >= m.size()) continue;

			auto& other = m[j];
			if (other.timestamp != current.timestamp) continue;
			if (!current.btv.empty() && !other.btv.empty() && current.btv != other.btv) continue;

			if (count_filled_fields(other) == 1)
			{
				if (current.content.empty() && !other.content.empty())
					current.content = std::move(other.content);
				else if (current.photo.empty() && !other.photo.empty())
					current.photo = std::move(other.photo);
				else if (current.video.empty() && !other.video.empty())
					current.video = std::move(other.video);
				else if (current.links.empty() && !other.links.empty())
					current.links = std::move(other.links);
				else
					continue; // Nothing merged

				if (current.btv.empty() && !other.btv.empty())
					current.btv = std::move(other.btv);

				to_erase.push_back(j);
			}
		}
	}

	// Sort and erase in reverse to prevent index shifting
	std::ranges::sort(to_erase);
	std::ranges::unique(to_erase);
	for (auto it = to_erase.rbegin(); it != to_erase.rend(); ++it)
		m.erase(m.begin() + *it);

	return m;
}
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

	for (auto& m: mv)
	{
		print({m.timestamp, page_map.contains(page_name) ? page_map.at(page_name) : page_name, 
				m.btv, m.content,
				vec_to_string(m.photo),
				vec_to_string(m.video),
				vec_to_string(m.links),
				m.kdv,
				m.has_like ? "L" : ""});
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

