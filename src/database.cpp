#include "softver-za-prodavnice/database.hpp"

Database::Database(std::vector<User> user_data, std::vector<Item> item_data)
	: user_data(user_data), item_data(item_data) {}

Database::Database(const Database& other)
	: user_data(other.user_data), item_data(other.item_data) {}

void Database::set_user_data(std::vector<User> user_data) { this->user_data = user_data; }
void Database::set_item_data(std::vector<Item> item_data) { this->item_data = item_data; }

std::vector<User> Database::get_user_data() const { return user_data; }
std::vector<Item> Database::get_item_data() const { return item_data; }


void Database::delete_users(const std::vector<User>& users) {
	for (int i = 0; i < users.size(); i++)
		user_data.erase(std::remove(user_data.begin(), user_data.end(), users[i]), user_data.end());

	std::string path;
	for(int i = 0; i < paths.get_size(); i++)
		if (paths.get_path(i).first == "korisnici") {
			path = paths.get_path(i).second;
			break;
		}
	write_in_file(path);
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
	write_in_file(path);
}
