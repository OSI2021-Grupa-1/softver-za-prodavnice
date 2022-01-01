#include "softver-za-prodavnice/database.hpp"

Database::Database(std::vector<User> user_data, std::vector<Item> item_data)
	: user_data(user_data), item_data(item_data) {}

Database::Database(const Database& other)
	: user_data(other.user_data), item_data(other.item_data) {}

void Database::set_user_data(std::vector<User> user_data) { this->user_data = user_data; }
void Database::set_item_data(std::vector<Item> item_data) { this->item_data = item_data; }

std::vector<User> Database::get_user_data() const { return user_data; }
std::vector<Item> Database::get_item_data() const { return item_data; }


void Database::delete_user(const std::vector<User>& users) {
	for (int i = 0; i < users.size(); i++)
		user_data.erase(std::remove(user_data.begin(), user_data.end(), users[i]), user_data.end());

	std::string path;
	for(int i = 0; i < paths.get_size(); i++)
		if (paths.get_path(i).first == "korisnici") {
			path = paths.get_path(i).second;
			break;
		}
	write_users_to_file(path);
}

void Database::delete_item(const std::vector<Item>& items) {
	for (int i = 0; i < items.size(); i++)
		item_data.erase(std::remove(item_data.begin(), item_data.end(), items[i]), item_data.end());

	std::string path;
	for (int i = 0; i < paths.get_size(); i++)
		if (paths.get_path(i).first == "artikli_na_stanju") {
			path = paths.get_path(i).second;
			break;
		}
	write_items_to_file(path);
}


void Database::write_users_to_file(const std::string path) {
	if (auto file = std::ofstream(path)) {
		for (size_t i = 0; i < user_data.size(); ++i) {
			file << user_data[i].get_username() << "φ" << user_data[i].get_password() << "φ"
				 /*
				 << user_data[i].get_position()*/ << "φ" << user_data[i].get_number_of_logins() //treba preklopiti operator << u structu Position pa nakon toga ukloniti komentar
				 << "φ\n";                                                                      //da se upisuje i pozicija korisnika u fajl
		}
		file.close();
	} else {
		throw std::exception("File couldn't be opened\n");
	}
}

void Database::write_items_to_file(const std::string path) {
	if (auto file = std::ofstream(path)) {
		for (size_t i = 0; i < item_data.size(); ++i) {
			file << item_data[i].get_barcode() << "φ" << item_data[i].get_name() << "φ"
				 << item_data[i].get_quantity() << "φ" << item_data[i].get_price() << "φ\n";
		}
		file.close();
	} else {
		throw std::exception("File couldn't be opened\n");
	}
}
