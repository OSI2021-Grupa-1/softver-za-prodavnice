#include "softver-za-prodavnice/config.hpp"
#include <iostream>
#include <stdexcept>

size_t Config::get_size() const { return paths.size(); }

std::string Config::get_path(std::string key) const {
	for (int i = 0; i < paths.size(); i++) {
		if (paths[i].first == key) {
			return (prefix/paths[i].second).generic_string();
		}
	}
	return {};
}

void Config::load_paths(const std::string& path) {
	try {
		paths = util::insert_pairs(util::get_lines((prefix/path).string()));
	} catch (std::invalid_argument e) {
		std::cout << e.what();
	}
}
