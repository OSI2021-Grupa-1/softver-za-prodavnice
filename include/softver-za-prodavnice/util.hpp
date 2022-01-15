#pragma once

#include <algorithm>
#include <exception>
#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include "softver-za-prodavnice/item.hpp"
#include "softver-za-prodavnice/user.hpp"

namespace util {
std::vector<std::string> get_lines(const std::string& path);
std::string_view trim_whitespace(std::string_view str);
std::vector<std::pair<std::string, std::string>> insert_pairs(std::vector<std::string> file_lines);

std::string generete_receipt_file_name(std::string current_date_time);
std::string helper(int width, const std::string& str);

std::string encrypt_decrypt(std::string pw);

std::filesystem::path get_data_path(std::filesystem::path start);

Item parse_item(const std::string& line);
User parse_user(const std::string& line);
std::vector<User> read_users_from_file(const std::string path);
std::vector<Item> read_items_from_file(const std::string path);

bool greater_price(const Item& item, double price);
bool greater_quantity(const Item& item, double quantity);
bool lesser_price(const Item& item, double price);
bool lesser_quantity(const Item& item, double quantity);

bool is_valid_date(int d, int m, int y);
bool is_leap(int y);

} // namespace util
