#pragma once
#include <vector>
#include "softver-za-prodavnice/item.hpp"
#include "softver-za-prodavnice/user.hpp"

class Database {
	vector<User> user_data;
	vector<Item> item_data;

  public:
	Database() = default;
	Database(vector<User> user_data, vector<Item> item_data);
	Database(const Database& other);

	void set_user_data(vector<User> user_data);
	void set_item_data(vector<Item> item_data);

	vector<User> get_user_data() const;
	vector<Item> get_item_data() const;
};
