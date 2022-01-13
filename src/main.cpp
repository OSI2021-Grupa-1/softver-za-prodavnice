#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include "softver-za-prodavnice/config.hpp"
#include "softver-za-prodavnice/user.hpp"
#include "softver-za-prodavnice/util.hpp"
#include "softver-za-prodavnice/tui.hpp"

int main(int argc, char** argv) {
	Config paths(util::get_data_path(argv[0]));
	paths.load_paths("config.txt");
	paths.print();
	tui::login();
}
