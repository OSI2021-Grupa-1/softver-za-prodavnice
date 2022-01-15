#pragma once
#pragma warning(disable : 4996) // zbog localtime(u current_date_time funkciji), moze biti nesigurno
#include <algorithm>
#include <functional>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <vector>
#include <time.h>
#include "softver-za-prodavnice/config.hpp"
#include "softver-za-prodavnice/item.hpp"
#include "softver-za-prodavnice/user.hpp"
#include "softver-za-prodavnice/util.hpp"

class Database {
	std::vector<User> user_data;
	std::vector<Item> item_data;
	Config paths;
	User current_user{};

  public:
	Database(std::vector<User> user_data, std::vector<Item> item_data, Config paths);
	// treba biti dovoljno da se samo proslijedi konfiguracioni fajl i da baza podataka sama cita
	// sve potrebne informacije
	Database(Config& paths);

	void set_current_user(User user) { current_user = std::move(user); }
	User get_current_user() const { return current_user; }
	Config get_pahts() const { return paths; }
	void set_user_data(std::vector<User> user_data);
	void set_item_data(std::vector<Item> item_data);

	std::vector<User> get_user_data() const;
	std::vector<Item> get_item_data() const;

	void delete_users(const std::vector<User>& users);
	void delete_items(const std::vector<Item>& items);

	void change_password(const std::string& usr, const std::string& new_pw);
	void reset_attempts(const std::string& usr);
	bool is_password_correct(const std::string& usr, const std::string& input) const;
	bool are_passwords_equal(const std::string& original, const std::string& confirmation) const;
	bool is_password_valid(std::string& pw) const { return pw.size() >= 8; }

	int find_user(const std::string& usr) const;

	void write_users_to_file(const std::string path);
	void write_items_to_file(const std::string path);

	bool is_contained(std::string barcode);
	std::vector<Item> filter(std::function<bool(const Item&, double)> f, double comparator);
	std::vector<Item> filter_name(std::string substr);

	void update_items(std::vector<std::pair<Item, double>> items);

	bool check_item_availability(const std::string& other_barcode, const double& quantity);

	std::vector<Item> create_report(const std::vector<Item>& items, const int& start_date,
									const int& end_date, std::string& path);
	int search_item_in_vector(const std::vector<Item>& vect, const std::string& barcode);

	void generate_receipt(std::vector<std::pair<Item, double>> sold_items, const std::string& date);

	void write_sold_items_to_file(const std::vector<Item>& items, const std::string& date);

	Item find_item_by_barcode(const std::string& id) const;
	std::vector<std::vector<std::string>> items_table();
	void update_quantity_by_id(std::vector<Item>& copy, std::string id, double quantity);
	void change_quantity(std::string id, double const quantity);

	const std::string current_date_time();
	bool backup();
};
