#include <vector>
#include <string>
std::vector<std::string> parse_line(const std::string_view& line);
std::vector<std::vector<std::string>> parse_tsv(const std::string& path);
