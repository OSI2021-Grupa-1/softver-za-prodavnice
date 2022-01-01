#pragma once

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include "softver-za-prodavnice/util.hpp"
class Config {
	std::vector<std::pair<std::string, std::string>> paths{};

  public:
	size_t get_size() const;
	std::string get_path(std::string key) const;
	void load_paths(const std::string& path);
	// testna metoda, treba izbrisati kasnije
	void print() {
		for (auto x : paths) {
			std::cout << x.first << " = " << x.second << std::endl;
		}
	}
};
