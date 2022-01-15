#include "softver-za-prodavnice/util.hpp"
#include <list>
#include <stdexcept>

std::vector<std::string> util::get_lines(const std::string& path) {
	std::vector<std::string> ret;
	std::string temp_line;
	if (auto file = std::ifstream(path)) {
		while (std::getline(file, temp_line)) {
			ret.push_back(temp_line);
		}
	} else {
		throw std::invalid_argument("File couldn't be opened");
	}
	return ret;
}

std::vector<std::pair<std::string, std::string>>
util::insert_pairs(std::vector<std::string> file_lines) {
	std::vector<std::pair<std::string, std::string>> key_value_pairs;
	std::size_t line_number = 0;
	for (const auto& str : file_lines) {
		++line_number;
		if (auto delimeter_pos = str.find('='); delimeter_pos != std::string::npos) {
			std::string const key = (str.substr(0, delimeter_pos));
			if (key.empty()) {
				throw std::invalid_argument("Key is empty");
			} else {
				std::string const value = (str.substr(delimeter_pos + 1));
				if (key[0] != '#') {
					key_value_pairs.push_back({std::string(key), std::string(value)});
				}
			}
		}
	}
	return key_value_pairs;
}

std::string_view util::trim_whitespace(std::string_view str) {
	if (str.empty()) {
		return {};
	}
	std::size_t start = 0;
	std::size_t finish = str.size();
	while (std::isspace(str[start])) {
		++start;
	}
	while (std::isspace(str[finish - 1])) {
		--finish;
	}
	if (finish == start) {
		return str;
	}
	return str.substr(start, finish - start);
}

std::string util::generete_receipt_file_name(std::string current_date_time) {
	std::string file_name;
	std::replace(current_date_time.begin(), current_date_time.end(), ':', '-');
	std::replace(current_date_time.begin(), current_date_time.end(), '.', '-');

	file_name = current_date_time + ".txt";
	return file_name;
}

std::string util::helper(int width, const std::string& str) {
	int len = str.length();
	if (width < len - 2) {
		return " " + str.substr(0, 45) + "\n" + helper(width, str.substr(46, str.length()));
	}

	int diff = width - len;
	int pad1 = diff / 2;
	int pad2 = diff - pad1;
	return std::string(pad1, ' ') + str + std::string(pad2, ' ');
}

std::filesystem::path util::get_data_path(std::filesystem::path start) {
	start = std::filesystem::absolute(start);
	static constexpr std::string_view target = "data";
	while (!start.empty() && (start.parent_path() != start.root_directory())) {
		auto ret = start / target;
		if (std::filesystem::is_directory(ret)) {
			return ret;
		}
		start = start.parent_path();
	}
	return {};
}

std::string util::encrypt_decrypt(std::string pw) {
	char key = 'x';
	std::string to_process = pw;
	for (int i = 0; i < pw.size(); i++) to_process[i] = to_process[i] ^ key;

	return to_process;
}

std::vector<User> util::read_users_from_file(const std::string path) {
	std::vector<User> ret{};
	std::string temp_line;
	if (auto file = std::ifstream(path)) {
		while (std::getline(file, temp_line)) {
			User user = util::parse_user(temp_line);
			ret.push_back(user);
		}
		return ret;
	} else
		throw "File couldn't be opened";
}

User util::parse_user(const std::string& line) {
	std::vector<std::string> data;
	auto divider = line.find('#');
	data.push_back(line.substr(0, divider));
	std::string temp_line = line.substr(divider + 1);
	divider = temp_line.find('#');
	data.push_back(temp_line.substr(0, divider));
	std::string temp_line2 = temp_line.substr(divider + 1);
	divider = temp_line2.find('#');
	data.push_back(temp_line2.substr(0, divider));
	data.push_back(temp_line2.substr(divider + 1));
	User ret(data[0], data[1], data[2], std::stoi(data[3]));
	return ret;
}

std::vector<Item> util::read_items_from_file(const std::string path) {
	std::vector<Item> ret{};
	std::string temp_line;
	if (auto file = std::ifstream(path)) {
		while (std::getline(file, temp_line)) {
			Item item = util::parse_item(temp_line);
			ret.push_back(item);
		}
		return ret;
	} else
		throw "File couldn't be opened\n";
}

Item util::parse_item(const std::string& line) {
	std::vector<std::string> data;
	auto divider = line.find('#');
	data.push_back(line.substr(0, divider));
	std::string temp_line = line.substr(divider + 1);
	divider = temp_line.find('#');
	data.push_back(temp_line.substr(0, divider));
	std::string temp_line2 = temp_line.substr(divider + 1);
	divider = temp_line2.find('#');
	data.push_back(temp_line2.substr(0, divider));
	data.push_back(temp_line2.substr(divider + 1));
	Item ret(data[0], data[1], std::stod(data[2]), std::stod(data[3]));
	return ret;
}

bool util::greater_price(const Item& item, double price) {
	if (item.get_price() > price) return true;
	return false;
}

bool util::greater_quantity(const Item& item, double quantity) {
	if (item.get_quantity() > quantity) return true;
	return false;
}

bool util::lesser_price(const Item& item, double price) {
	if (item.get_price() < price) return true;
	return false;
}

bool util::lesser_quantity(const Item& item, double quantity) {
	if (item.get_quantity() < quantity) return true;
	return false;
}


bool util::is_leap(int year) { return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0)); }

bool util::is_valid_date(int d, int m, int y) {
	const int MAX_VALID_YR = 9999;
	const int MIN_VALID_YR = 1800;

	if (y > MAX_VALID_YR || y < MIN_VALID_YR) return false;
	if (m < 1 || m > 12) return false;
	if (d < 1 || d > 31) return false;

	if (m == 2) {
		if (is_leap(y)) return (d <= 29);
		else
			return (d <= 28);
	}

	if (m == 4 || m == 6 || m == 9 || m == 11) return (d <= 30);

	return true;
}
