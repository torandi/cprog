#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP

#include <string>
#include <vector>

void erase_word(std::string &str, size_t pos, size_t len);
std::string trim(std::string s);
std::vector<std::string> split(const std::string &str, const std::string &search, bool keep = false);

#endif
