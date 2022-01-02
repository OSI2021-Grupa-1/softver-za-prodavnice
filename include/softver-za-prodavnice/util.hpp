#pragma once

#include <algorithm>
#include <exception>
#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>

namespace util {
std::vector<std::string> get_lines(const std::string& path);
std::string_view trim_whitespace(std::string_view str);
std::vector<std::pair<std::string, std::string>> insert_pairs(std::vector<std::string> file_lines);

std::string generete_receipt_file_name(std::string current_date_time);
std::string helper(int width, const std::string& str);

std::filesystem::path get_data_path(std::filesystem::path start);
} // namespace util
