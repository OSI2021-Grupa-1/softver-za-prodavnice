#pragma once
#include <functional>
#include <ostream>
#include <vector>
#include "ostream"
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

	void delete_users(const std::vector<User>& users);
	void delete_items(const std::vector<Item>& items);

	void change_password(const std::string& usr, const std::string& new_pw);
	bool is_password_correct(const std::string& usr, const std::string& input) const;
	bool are_passwords_equal(const std::string& original, const std::string& confirmation) const;

	size_t find_user(const std::string& usr) const;
	std::string find_path(const std::string& key) const;

	void write_users_to_file(const std::string path);
	void write_items_to_file(const std::string path);

	bool is_contained(std::string barcode);
	std::vector<Item> filter(std::function<bool(const Item&, double)> f, double comparator);
	bool greater_price(const Item& item, double price);
	bool greater_quantity(const Item& item, double quantity);
	bool lesser_price(const Item& item, double price);
	bool lesser_quantity(const Item& item, double quantity);
	std::vector<Item> filter_name(std::string substr);

	bool find_item(const std::string& other_barcode, const int& quantity);
};
