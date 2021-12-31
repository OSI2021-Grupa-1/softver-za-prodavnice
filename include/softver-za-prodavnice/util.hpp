#pragma once

#include <exception>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>

namespace util {
std::vector<std::string> get_lines(const std::string& path);
std::string_view trim_whitespace(std::string_view str);
std::vector<std::pair<std::string, std::string>> insert_pairs(std::vector<std::string> file_lines);
} // namespace util
