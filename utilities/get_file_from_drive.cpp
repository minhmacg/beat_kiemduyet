#include "../include/drive.h"

int main(int argc, const char** argv)
{
	if (argc != 2) 
	{
		std::cout << "invalid argument\n";
		return -1;
	};

	std::ifstream f;
	f.open("drivelink/drivelinks.txt");
	drivemap dmr {load_drive_map(f, true)};
	f.close();
	std::string line;
	std::vector<std::string> file;
	f.open(argv[1]);
	if (!f.is_open()) std::cout << "bad file input\n";
	
	while (std::getline(f,line))
	{
		file.push_back(line);
	};

	std::ofstream o;
	o.open("output/file_from_drive");
	for (auto& l: file)
	{
		l = dmr[l];
		o << l << '\n';
		std::cout << l << '\n';
	};
};

