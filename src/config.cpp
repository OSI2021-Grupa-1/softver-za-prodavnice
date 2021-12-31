#include "softver-za-prodavnice/config.hpp"
#include <iostream>
#include <stdexcept>

size_t Config::get_size() const { return paths.size(); }

std::pair<std::string, std::string> Config::get_path(int i) const { return paths[i]; }

void Config::load_paths(const std::string& path) {
	try {

		paths = util::insert_pairs(util::get_lines(path));
	} catch (std::invalid_argument e) {
		std::cout << e.what();
	}
}
