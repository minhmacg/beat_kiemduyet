#ifndef BEAT_H
#define BEAT_H
#include <iostream>
#include <concepts>
#include <fstream>
#include <vector>
#include <tuple>
#include <memory>
#include <json.hpp>
#include <map>
#include <regex>

template<typename T>
concept cc = std::integral<T>;

const std::string SS = ", ",
		  		FS = "\t";

typedef std::vector<std::string> vec_str;
typedef std::map<std::string,int> pageid;
struct Messages
{
	struct Base{std::string id; vec_str links;};
	std::string id, page, btv, content, link;
	std::time_t time;
	
	static int counter;
	Base photo;
	Base video;
	Base source;
	struct reaction
	{
		std::string react;
		std::string actor;
	};
	std::vector<reaction> reactions;
};
Messages from_json(const nlohmann::json&);

typedef std::vector<std::shared_ptr<Messages>> messvec;
messvec vec_from_input(const nlohmann::json&);
messvec& only_reactions(messvec&);
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
const std::unordered_map<std::string, const char*> page_map 
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


const std::unordered_map<std::string, std::string> btv
{
	{"Ánh" , "Đào Xuân Ánh"},
	{"Bánh Bòa" , "Nguyễn Hồ Thanh Thảo"},
	{"Thảo Nguyễn","Nguyễn Hồ Thanh Thảo"},
	{"Bùi Quỳnh Tranq." , "Bùi Quỳnh Trang"},
	{"Bao-Linh Dong" , "Đồng Thị Bảo Linh"},
	{"Châm Châm" , "Nguyễn Đặng Mai Trâm"},
	{"Măng Cụtt", "Nguyễn Đoàn Vĩnh Xuyên"},
	{"Danh Nam Bùi" , "Bùi Danh Nam"},
	{"Đào Hửu Nhơn" , "Đào Hửu Nhơn"},
	{"Đoàn Việt" , "Đoàn Quốc Việt"},
	{"Dũng Nguyễn" , "Nguyễn Bá Dũng"},
	{"Dương Lê" , "Lê Thị Thùy Dương"},
	{"Giang Uyên" , "Lê Giang Uyên"},
	{"Hân Gia" , "Lưu Gia Hân"},
	{"Hoàng Khánh Duy" , "Hoàng Khánh Duy"},
	{"Hoang Tuấn Anh" , "Hoàng Tuấn Anh"},
	{"Hồng Quân" , "Đậu Hồng Quân"},
	{"Hồng Thư Thư" , "Võ Thị Hồng Thư"},
	{"Hồng Dương Lê" , "Lê Hồng Dương"},
	{"Huy Ho" , "Hồ Quang Huy"},
	{"Huy Tran" , "Trần Quốc Huy"},
	{"Huyy Anh" , "Tạ Anh Huy"},
	{"Khánh Bùi" , "Bùi Thị Khánh "},
	{"Khanh Huyen" , "Phạm Khánh Huyền"},
	{"Khánh Ly" , "Nguyễn Thị Khánh Ly"},
	{"Khánh Vũ" , "Vũ Văn Khánh"},
	{"Lệ Đỗ" , "Đỗ Thị Lệ"},
	{"Le Ngoc Tuyen" , "Lê Ngọc Tuyền"},
	{"Linh Phương" , "Phạm Hồ Linh Phương"},
	{"Luu Viet Hoang" , "Lưu Viết Hoàng"},
	{"Minh Đức" , "Phạm Minh Đức"},
	{"MinhQuang Luu" , "Lưu Minh Quang"},
	{"Minh Quang Luu" , "Lưu Minh Quang"},
	{"Nguyễn An" , "Nguyễn Song An"},
	{"Nguyễn Đồng Tường" , "Nguyễn Đồng Tường"},
	{"Nguyễn Đức Hậu" , "Nguyễn Đức Hậu"},
	{"Ngo Duc Anh" , "Ngô Đức Anh"},
	{"Nguyễn Đức Trọng" , "Nguyễn Đức Trọng"},
	{"Nguyễn Huyền" , "Nguyễn Thị Ngọc Huyền "},
	{"Nguyen Ngoc Huyen" , "Nguyễn Thị Ngọc Huyền "},
	{"Tuan Anh Nguyen","Nguyễn Tuấn Anh"},
	{"Phuong Anh","Đặng Phương Anh"},
	{"Trần Hiệp","Trần Văn Hiệp"},
	{"Nguyễn Long" , "Nguyễn Hoàng Long"},
	{"Nguyễn Thành Vĩnh" , "Nguyễn Thành Vĩnh"},
	{"Nguyễn Thị Thuý" , "Nguyễn Thị Thuý"},
	{"Nguyễn Trung Kiên" , "Nguyễn Trung Kiên"},
	{"Nguyễn Tuấn Thịnh" , "Nguyễn Tuấn Thịnh"},
	{"Nguyen Viet Anh" , "Nguyễn Việt Anh"},
	{"Phạm Hoàng My" , "Phạm Hoàng My"},
	{"Phuong Thao" , "Lương Phương Thảo"},
	{"Phuong Thao Luong" , "Lương Phương Thảo"},
	{"Quang Hiếu" , "Bùi Quang Hiếu"},
	{"Quyen Tieu" , "Đỗ Tiểu Quyên"},
	{"Quỳnh Như" , "Nguyễn Như Quỳnh"},
	{"Son Tung Nguyen" , "Nguyễn Sơn Tùng"},
	{"Thao Thu Giap" , "Giáp Thị Thu Thảo"},
	{"Thu Trang" , "Vũ Thị Thu Trang"},
	{"Tran Anh" , "Trần Diệu Tú Anh"},
	{"Tống Bùi Vĩnh Hoàng" , "Tống Bùi Vĩnh Hoàng"},
	{"Tuan Dinh Zin" , "Đinh Tiến Tuấn"},
	{"Văn A Chúi" , "Hồ Quang Huy"},
};


// beatvn 54
#endif
