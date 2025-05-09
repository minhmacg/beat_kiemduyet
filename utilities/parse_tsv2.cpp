#include <ranges>
#include <vector>
#include <string>
#include <regex>
#include <algorithm>
#include <print>
#include <fstream>
int main(int argc, const char** argv)
{
	//std::string a {"01/05/25 - 12:59	FP - BEAT NOW	Nguyễn Việt Anh	1 HÌNH ẢNH THẬT SỰ RẤT ĐẸP    Bắt gặp hình ảnh chú sĩ quan chạy vội lên đeo dải băng kỷ niệm cho bác cựu chiến binh - thể hiện sự trân quý vô cùng đến bác.   Nghe bảo bác cựu chiến binh ấy đã từng tham gia kháng chiến trong trận ở Bình Dương, và bác chỉ còn 1 cánh tay trái    Video: Dyne		491486815_9698525766929037_8627246504483009680_n.mp4		Dũng "};
	//std::string b {"01/05/25 - 12:59	FP - BEAT NOW	Nguyễn Đức Hậu	Đi biển búp măng non cho cả nhà đi tìm chìa khóa ôtô giấu dưới cát    Video: Ntan		https://drive.google.com/file/d/1aaF-i5cLyx4R2bjEDlYR2pzlnq_Wmt2Z/view?usp=sharing		Dũng	1	đoạn mắng mỏ dọa thằng bé	10"};
	//std::regex rg {"[^\\t\\n]*"};
	//auto i1 {std::sregex_iterator(a.begin(), a.end(), rg)};
	//auto i2 {std::sregex_iterator(b.begin(), b.end(), rg)};
	//std::println("{} {}", std::distance(i1, std::sregex_iterator{}), std::distance(i2, std::sregex_iterator{}));
	//for (auto i = i1; i != std::sregex_iterator{}; i++)
	//	std::println("{}", i->str());
	//for (auto i = i2; i != std::sregex_iterator{}; i++)
	//	std::println("{}", i->str());
	if (argc != 2)
	{
	};
	auto as = a | std::views::split('\t') | std::ranges::to<std::vector<std::string>>();
	std::println("{}", as.size());

	auto bs = b | std::views::split('\t') | std::ranges::to<std::vector<std::string>>();
	std::println("{}", bs.size());
};
