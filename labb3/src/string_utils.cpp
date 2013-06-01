#include <string>
#include <vector>
#include <iostream>

std::string trim(std::string s) {
	size_t begin_str = s.find_first_not_of(" \t\n\r");
	if(begin_str != std::string::npos) {
		size_t last = s.find_last_not_of(" \t\n\r");
		if(last >= begin_str) {
			std::string trimmed = s.substr(begin_str, last - begin_str + 1);
			return trimmed;
		} else {
			return "";
		}
	} else {
		return "";
	}
}
void erase_word(std::string &str, size_t pos, size_t len) {
	str.erase(pos, str.find(" ", pos + len) - pos);
	str = trim(str);
}

std::vector<std::string> split(const std::string &str, const std::string &search, bool keep) {
	size_t pos = 0;
	std::vector<std::string> res;
	size_t p;
	while((p = str.find_first_of(search, pos)) != std::string::npos) {
		if(p != pos) {
			std::string s = trim(str.substr(pos, (p - pos) + (keep ? 1 : 0)));
			if(!s.empty()) res.push_back(s);
		}
		pos = p + 1;
	}
	if(pos < str.length()) {
		std::string s = trim(str.substr(pos));
		if(!s.empty()) res.push_back(s);
	}
	return res;
}

std::ostream cnull(0);
