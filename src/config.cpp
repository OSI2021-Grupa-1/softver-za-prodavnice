#include "softver-za-prodavnice/config.hpp"
#include <iostream>
#include <stdexcept>

void Config::load_paths(const std::string& path) {
	try {

		paths = util::insert_pairs(util::get_lines(path));
	} catch (std::invalid_argument e) {
		std::cout << e.what();
	}
}
