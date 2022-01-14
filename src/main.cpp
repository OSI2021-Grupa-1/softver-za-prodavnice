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
	User radnik{"Marko", "lozinka", "radnik", 0};

	 user_data.push_back(nikola);
	user_data.push_back(radnik);
	std::vector<Item> item_data{};
	item_data.push_back(Item("4123", "Jabuke", 1.30, 220));
	item_data.push_back(Item("6434", "Banane", 1.70, 70));
	item_data.push_back(Item("1325", "Pilece meso", 8.40, 40));
	item_data.push_back(Item("8352", "Sok", 1.30, 40));
	item_data.push_back(Item("4123", "Mandarine", 1.80, 90.50));
	item_data.push_back(Item("4123", "Voda", 0.80, 120));

	Config paths(util::get_data_path(argv[0]));
	paths.load_paths("config.txt");
	//	paths.print();
	Database db{paths};

	tui::login_interface(db);
}
