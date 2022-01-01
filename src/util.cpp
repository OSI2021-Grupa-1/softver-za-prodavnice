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
