#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include "softver-za-prodavnice/config.hpp"
#include "softver-za-prodavnice/tui.hpp"
#include "softver-za-prodavnice/user.hpp"
#include "softver-za-prodavnice/util.hpp"

int main(int argc, char** argv) {

	std::vector<User> user_data;
	User nikola{"Nikola", "lozinka", "sef", 0};
	 user_data.push_back(nikola);
	std::vector<Item> item_data{};
	Config paths(util::get_data_path(argv[0]));
	paths.load_paths("config.txt");
//	paths.print();
	Database db{user_data, item_data, paths};
	tui::login_interface(db);
}
