#ifndef BEAT_H
#define BEAT_H
#include <fstream>
#include <vector>
#include <json.hpp>
#include <map>

const std::string SS = ", ",
		  		FS = "\t";

typedef std::vector<std::string> vec_str;
typedef std::map<std::string,int> pageid;
struct Messages
{
	vec_str photo, video, links;
	std::string btv, content, reply, kdv, timestamp;
	bool has_like {false};
};
Messages from_json(const nlohmann::json&, const std::string& pagename);

using messvec = std::vector<Messages>;

messvec vec_from_input(const nlohmann::json&, const std::string& pagename);
messvec& only_reactions(messvec&);
messvec& join_messages(messvec&, const std::string&);

void print_to_tsv(const std::string&, std::ostream&, const messvec&, const std::string&);
// processing:
// [x] only liked/reacted messages
// [x] convert time
// [x] join messages
// pages name -> not now
// []

// links
// [] store links
// [] mapping

std::unordered_map<std::string, std::string> fetch_data(const std::filesystem::path&);
const std::unordered_map<std::string, std::string> page_map {fetch_data("pagename")};

const std::unordered_map<std::string, std::string> btv {fetch_data("btvname")};


// beatvn 54
#endif
