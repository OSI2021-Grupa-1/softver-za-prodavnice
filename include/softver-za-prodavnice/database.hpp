#pragma once
#include <vector>
#include "softver-za-prodavnice/config.hpp"
#include "softver-za-prodavnice/item.hpp"
#include "softver-za-prodavnice/user.hpp"

class Database {
	std::vector<User> user_data;
	std::vector<Item> item_data;
	Config paths{};

  public:
	Database() = default;
	Database(std::vector<User> user_data, std::vector<Item> item_data);
	Database(const Database& other);

	void set_user_data(std::vector<User> user_data);
	void set_item_data(std::vector<Item> item_data);

	std::string get_user_path() const;
	std::string get_item_path() const;

	std::vector<User> get_user_data() const;
	std::vector<Item> get_item_data() const;

	void delete_user(const std::vector<User>& users);
	void delete_item(const std::vector<Item>& items);

	void write_users_to_file(const std::string path); 
	void write_items_to_file(const std::string path);
};
