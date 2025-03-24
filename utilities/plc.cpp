#include "parse_tsv.hpp"
#include <iostream>
#include <regex>
#include <array>
#include <fstream>
#include <filesystem>
#include <format>
#include <algorithm>
std::array plc
{
	// FB,
	"HÀNH VI PHẠM TỘI VÀ BẠO LỰC",
	"Bạo lực và khích nộ",
	"Cá nhân và tổ chức nguy hiểm",
	"Cấu kết gây hại và cổ xúy tội ác",
	"Hàng hóa và dịch vụ bị hạn chế",
	"Gian lận và lừa đảo",
	"AN TOÀN",
	"Tự tử, tự gây thương tích và chứng rối loạn ăn uống",
	"Ảnh khỏa thân, lạm dụng và bóc lột tình dục trẻ em",
	"Bóc lột tình dục người lớn",
	"Bắt nạt và quấy rối",
	"Bóc lột con người",
	"Vi phạm quyền riêng tư",
	"NỘI DUNG PHẢN CẢM",
	"Ngôn từ gây thù ghét",
	"Nội dung bạo lực và phản cảm",
	"Ảnh khỏa thân người lớn và hoạt động tình dục",
	"Hành vi gạ gẫm tình dục người lớn và ngôn ngữ khiêu dâm",
	"TÍNH TOÀN VẸN VÀ TÍNH XÁC THỰC",
	"Danh tính thực và tính toàn vẹn của tài khoản",
	"Spam",
	"An ninh mạng",
	"Hành vi gian dối",
	"Thông tin sai lệch",
	"Tưởng nhớ",
	"TÔN TRỌNG QUYỀN SỞ HỮU TRÍ TUỆ",
	"Quyền sở hữu trí tuệ",
	//TIKTOK
	"An toàn và văn minh",
	"Hành vi bạo lực và hoạt động tội phạm",
	"Phát ngôn thù địch và hành vi thù địch",
	"Tổ chức và cá nhân bạo lực và thù địch",
	"Bóc lột và xâm hại trẻ vị thành niên",
	"Bóc lột tình dục và bạo lực trên cơ sở giới",
	"Bóc lột con người",
	"Quấy rối và bắt nạt",
	"Sức khỏe tâm thần và hành vi",
	"Tự tử và tự làm hại bản thân",
	"Rối loạn ăn uống và tự cảm nhận ngoại hình cơ thể",
	"Hoạt động và thử thách nguy hiểm",
	"Chủ đề nhạy cảm và dành cho người trưởng thành",
	"Hoạt động và dịch vụ tình dục",
	"Khỏa thân và phơi bày cơ thể",
	"Nội dung gợi dục",
	"Nội dung gây sốc và ghê rợn",
	"Ngược đãi động vật",
	"TÌNH TOÀN DIỆN VÀ CHÂN THỰC",
	"Thông tin sai lệch",
	"Tính liêm chính trong bầu cử và dân sự",
	"Nội dung đa phương tiện tổng hợp và bị thao túng",
	"Tương tác giả",
	"Nội dung không nguyên bản và mã QR",
	"Spam và hành vi lừa đảo bằng tài khoản",
	"HÀNG HÓA VÀ HOẠT ĐỘNG THƯƠNG MẠI BỊ KIỂM SOÁT",
	"Đánh bạc",
	"Rượu, thuốc lá và ma túy",
	"Súng và vũ khí nguy hiểm",
	"Buôn bán hàng hóa và dịch vụ bị kiểm soát",
	"Minh bạch thương mại và quảng cáo trả phí",
	"Gian lận và lừa đảo",
	"QUYỀN RIÊNG TƯ VÀ BẢO MẬT",
	"Thông tin cá nhân",
	"Bảo mật nền tảng",
	"Nguồn tin",
	"An ninh chính trị",
	"Tôn giáo",
	"Đối tác khách hàng",
};

std::array btv
{
	"Phong", "Bách", "Dũng",
};
std::array stt
{
	"Duyệt", "Sửa/ xoá bài - cơ bản", "Sửa/ xóa bài - nâng cao" 
};
int main(int argc, const char** argv)
{
	if (argc != 2) 
	{
		std::cout << "bad input\n";
		return -1;
	};

	std::filesystem::path file {argv[1]};
	std::ifstream f;
	f.open(argv[1]);
	if (!f.is_open()) std::cout << "file doesn't exist\n";
	
	auto data {parse_tsv(argv[1])};

	for (auto& rows: data)
	{
		auto& rbtv {rows[8]};
		auto& rstt {rows[9]};
		auto& rplc {rows[11]};

		if (!rbtv.empty()) rbtv = btv.at(std::stoi(rbtv)); 
		if (!rstt.empty()) rstt = stt.at(std::stoi(rstt));
		else rstt = stt.front();
		if (!rplc.empty()) rplc = plc.at(std::stoi(rplc)); 

		for (const auto& field: rows)
			std::cout << field << '\t';
		std::cout << '\n';
	};
	// a tab followed by any number of digit, and any number of whitespace till the end of line
	//std::regex plc_rg {"\t(\\d+)$"};
	//if (std::regex_search(line,rm,plc_rg))
	//{
	//	std::cout << rm.str() << '\n';
	//	line.replace(line.find_last_of(rm.str(1)) - rm.str(1).size() + 1, 
	//			rm.str(1).size(), 
	//			std::format("\"{}\"", plc.at(std::stoi(rm.str(1)))));
	//};
		
};
