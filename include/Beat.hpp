#ifndef BEAT_H
#define BEAT_H
#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <memory>
#include <json.hpp>
#include <map>
#include <regex>

const std::string SS = ", ",
		  		FS = "\t";

typedef std::vector<std::string> vec_str;
typedef std::map<std::string,int> pageid;
struct Messages
{
	struct Base{std::string id; vec_str links;};
	std::string id, page, btv, content, link, kq{"Duyệt"}, cmt;
	std::time_t time;
	
	static int counter;
	Base photo;
	Base video;
	Base source;

	std::tuple<bool,const char*> reaction {false,""};
};
Messages from_json(const nlohmann::json&);

typedef std::vector<std::shared_ptr<Messages>> messvec;
messvec vec_from_input(const nlohmann::json&);
messvec& only_reactions(messvec&, const std::string&);
messvec& join_messages(messvec&, const std::string&);

void print_to_tsv(const std::string, std::ofstream&,
		const messvec&, const std::string&);
// processing:
// [x] only liked/reacted messages
// [x] convert time
// [x] join messages
// pages name -> not now
// []

// links
// [] store links
// [] mapping
const std::map<std::string, const char*> page_map 
{
	{"beatnow","FP - BEAT NOW"},
	{"beatnw","FP - BEAT Network"},
	{"beatvn","FP - Beatvn"},
	{"cchn","FP - Chuyện của Hà Nội"},
	{"ct","FP - Cao Thủ"},
	{"dp","TT-HÓNG TO THE HEART"},
	{"hongheart","FP - Hóng to the Heart"},
	{"itb","FP - Inside the Box"},
	{"kkn","FP - Kiến không ngủ"},
	{"qc","Quảng cáo"},
	{"sbv","Giáo dục tài chính"},
	{"sgn","FP - Sài Gòn nghenn"},
	{"showbeat","FP - Showbeat"},
	{"tiktok","TT"},
	{"htbz","TT - HÀNH TINH BEATZ"},
	{"nvh","FP - Nhà Văn Hóa"},
	{"vnrec","FP - Vietnam.Recorder"},
	{"wtd","FP - What the Duck"},
};
inline std::string page_map_f(const std::string& pn, 
		const std::shared_ptr<Messages>& m)
{
	std::string rs;
	if (pn != "tiktok") return page_map.at(pn);
	if (m->btv == "Ánh"
		|| m->btv == "Luu Viet Hoang"
		|| m->btv == "Khánh Vũ") 
	{
		rs = "TT - BEATVN";
		if (m->content.find("@Linh Phương") != std::string::npos) 
			rs = "TT - HelloVietnam";
	}
	if (m->btv == "Linh Phương" || m->btv == "Ngô Tiến Dũng") rs = "TT - HelloVietnam";
	if (m->btv == "Quang Hiếu" || m->btv == "Nguyễn Đồng Tường") 
		rs = "TT - Beatvn Viral World";
	if (m->btv == "Lệ Đỗ") rs = "TT - SHOWBEAT";
	if (m->btv == "Nguyễn An") rs = "TT - BEAT the Game";
	
	return rs;
}

// beatvn 54
#endif
