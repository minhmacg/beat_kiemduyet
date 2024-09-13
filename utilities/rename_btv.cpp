#include <iostream>
#include <sstream>
#include <fstream>
#include <set>
#include <map>
#include <filesystem>

const std::map<std::string, std::string> btv
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

std::size_t find_nth(const std::string& str, int n, const std::string& str2)
{
	int i = 0;
	std::size_t pos = -1;
	while (i < n)
	{
		pos = str.find(str2,pos+1);
		if (pos == std::string::npos) return std::string::npos;
		else 
		{
			i++;
		};
	};
	return pos;
};

int main(int argc, const char** argv)
{
	if (argc != 2) std::cout << "bad argument\n";
	std::filesystem::path inputfile {argv[1]};
	std::ifstream f;
	std::ofstream of;
	f.open(inputfile);
	if (!f.is_open())
	{
		std::cout << "file doesn't exist";
		return -1;
	};
	of.open(std::string("./output/renamed_btv_") 
			+ inputfile.filename().string());
	if (!of.is_open()) return -1;
	std::string line;
	std::set<std::string> not_found_btv;
	while (std::getline(f,line))
	{
		if (find_nth(line, 3, "\t") != std::string::npos)
		{
			std::string btv_current {line.begin() + find_nth(line,2,"\t")+1, 
									line.begin() + find_nth(line,3,"\t")}; 
			std::cout << btv_current << '\n';
			bool found = false;
			for (auto& [k,v] : btv)
			{
				if (auto p = line.find(k); p != std::string::npos)
				{
					found = true;
					line.replace(p, k.size(), v);
					break;
				};
			};
			if (!found) not_found_btv.insert(btv_current);

			of << line << '\n';
		};
	};

	of.close();
	std::cout << "not found: ";
	for (auto& s: not_found_btv) std::cout << s << "; ";
};
