#include "softver-za-prodavnice/database.hpp"

Database::Database(vector<User> user_data, vector<Item> item_data) {
	this->user_data = user_data;
	this->item_data = item_data;
}

Database::Database(const Database& other) {
	this->user_data = other.user_data;
	this->item_data = item_data;
}


void Database::set_user_data(vector<User> user_data) { this->user_data = user_data; }
void Database::set_item_data(vector<Item> item_data) { this->item_data = item_data; }


vector<User> Database::get_user_data() const { return user_data; }
vector<Item> Database::get_item_data() const { return item_data; }
