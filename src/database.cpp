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

	std::string path = paths.get_path("korisnici");
	
	write_in_file(path);
}

void Database::delete_items(const std::vector<Item>& items) {
	for (int i = 0; i < items.size(); i++)
		item_data.erase(std::remove(item_data.begin(), item_data.end(), items[i]), item_data.end());

	std::string path = paths.get_path("artikli_na_stanju");

	write_in_file(path);
}

void Database::change_password(const std::string& usr, const std::string& new_pw) {
	if(new_pw.length() < 8)
		throw std::length_error("Password too short"); // mora se primiti u interfejsu

	size_t index = find_user(usr);
	user_data[index].set_password(new_pw);

	// upis u fajl
}

bool Database::password_correct(const std::string& usr, const std::string& pw_input) const {
	size_t index = find_user(usr);

	if (pw_input == user_data[index].get_password()) return true;
	else
		return false;
}

bool Database::passwords_equal(const std::string& original, const std::string& confirmation) const {
	if (original == confirmation) return true;
	else
		return false;
}

size_t Database::find_user(const std::string& username) const {
	for (size_t i = 0; i < paths.get_size(); i++) {
		if (username == user_data[i].get_username()) return i;
	}
	throw std::exception("User not found"); // ne bi ga trebao nikad baciti jer se username prosljedjuje iz main-a i vec je provjeren
}

std::string Database::find_path(const std::string& key) const {
	for (int i = 0; i < paths.get_size(); i++)
		if(key == paths.get_path(key))
			return paths.get_path(key);
}
