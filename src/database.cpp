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

	write_users_to_file(path);
}

void Database::delete_items(const std::vector<Item>& items) {
	for (int i = 0; i < items.size(); i++)
		item_data.erase(std::remove(item_data.begin(), item_data.end(), items[i]), item_data.end());

	std::string path = paths.get_path("artikli_na_stanju");

	write_items_to_file(path);
}

void Database::change_password(const std::string& usr, const std::string& new_pw) {
	if (new_pw.length() < 8)
		throw std::length_error("Password too short"); // mora se primiti u interfejsu

	size_t index = find_user(usr);
	user_data[index].set_password(new_pw);

	std::string path = paths.get_path("korisnici");

	write_users_to_file(path);
}

bool Database::is_password_correct(const std::string& usr, const std::string& pw_input) const {
	size_t index = find_user(usr);

	if (pw_input == user_data[index].get_password()) return true;
	else
		return false;
}

bool Database::are_passwords_equal(const std::string& original, const std::string& confirmation) const {
	if (original == confirmation) return true;
	else
		return false;
}

size_t Database::find_user(const std::string& username) const {
	for (size_t i = 0; i < paths.get_size(); i++) {
		if (username == user_data[i].get_username()) return i;
	}
	throw std::exception(); // ne bi ga trebao nikad baciti jer se username
							// prosljedjuje iz main-a i vec je provjeren
}

void Database::write_users_to_file(const std::string path) {
	if (auto file = std::ofstream(path)) {
		for (size_t i = 0; i < user_data.size(); ++i) {
			file << user_data[i] << "\n"; // da se upisuje i pozicija korisnika u fajl
		}
		file.close();
	} else {
		throw std::exception();
	}
}

void Database::write_items_to_file(const std::string path) {
	if (auto file = std::ofstream(path)) {
		for (size_t i = 0; i < item_data.size(); ++i) {
			file << item_data[i] << "\n";
		}
		file.close();
	} else {
		throw std::exception();
	}
}

bool Database::is_contained(std::string barcode) {
	for (size_t i = 0; i < item_data.size(); ++i) {
		if (item_data[i].get_barcode() == barcode) return true;
	}
	return false;
}

std::vector<Item> Database::filter(std::function<bool(const Item&, double)> f, double comparator) {
	std::vector<Item> ret;
	for (size_t i = 0; i < item_data.size(); ++i) {
		if (f(item_data[i], comparator)) ret.push_back(item_data[i]);
	}
	return ret;
}

std::vector<Item> Database::filter_name(std::string substr) {
	std::vector<Item> ret;
	for (size_t i = 0; i < item_data.size(); ++i) {
		if (item_data[i].get_name().find(substr) != std::string::npos) ret.push_back(item_data[i]);
	}
	return ret;
}

bool Database::greater_price(const Item& item, double price) {
	if (item.get_price() > price) return true;
	return false;
}

bool Database::greater_quantity(const Item& item, double quantity) {
	if (item.get_quantity() > quantity) return true;
	return false;
}

bool Database::lesser_price(const Item& item, double price) {
	if (item.get_price() < price) return true;
	return false;
}

bool Database::lesser_quantity(const Item& item, double quantity) {
	if (item.get_quantity() < quantity) return true;
	return false;
}

bool Database::find_item(const std::string& other_barcode, const int& quantity) {

	int i;
	for (i = 0; i < item_data.size(); i++) {
		if (item_data[i].get_barcode() == other_barcode) break;
	}
	if (i == item_data.size()) return false;
	if (item_data[i].get_quantity() < quantity) return false;
	return true;
}

// provjera stanja dostupnosti se provjerava prije ove funkcije
void Database::generate_receipt(std::vector<std::pair<Item, double>> sold_items, std::string username) {
	// nije jos definisana putanja gdje ce se fajl praviti
	// std::string path = paths.get_path("");

	std::string current_time = current_date_time();
	std::string file_name = util::generete_receipt_file_name(current_time); 

	std::fstream file;
	file.open(file_name, std::ios::out);
	double sum = 0;
	if (file.is_open()) {
		int width = 48;
		file << std::setw(width) << std::setfill('=') << "\n";
		file << std::format("{:^48}\n", "Naziv prodavnice");
		file << std::format("{:^48}\n", "Adresa");
		file << std::format("{:^48}\n", "Broj telefona");
		file << std::setw(width) << std::setfill('-') << '\n';
		file << std::left << "Datum i vrijeme: " << current_time << '\n';
		file << std::left << "Blagajnik: " << "ph" << '\n';
		// moze se dodati broj racuna, ali je to dosta posla jer bi nekad moglo doci do overflowa a
		// ta staticka promjenljiva bi se morala cuvati u fajlu
		file << "\n";
		file << std::left << std::setw(27) << std::setfill(' ') << "Artikal" << std::setw(7)
			 << "Cijena" << std::setw(6) << "Kol." << std::setw(8) << "Ukupno" << std::endl;
		file << std::setw(width) << std::setfill('-') << "" << '\n';
		for (int i = 0; i < sold_items.size(); i++) {
			double price = sold_items[i].first.get_price();
			double quantity = sold_items[i].second;
			sum += price * quantity;
			file << std::setprecision(2);
			if (sold_items[i].first.get_name().length() >= 20)
				file << std::left << std::setw(23) << sold_items[i].first.get_name() << "\n"
					 << std::string(23, ' ') << std::setw(9) << std::fixed << std::setprecision(2)
					 << price << std::setw(8) << quantity << std::setw(10) << price * quantity << "\n";
			else
				file << std::left << std::setw(23) << sold_items[i].first.get_name() << std::setw(9) << std::fixed << std::setprecision(2)
					 << price << std::setw(8) << quantity << std::setw(10) << price * quantity << "\n";
		}
		file << std::setw(width) << std::setfill('-') << "" << '\n';
		file << std::setfill(' ');
		file << "UKUPNO:" << std::string(33, ' ') << sum << std::endl;
		file << std::setw(width) << std::setfill('-') << "" << '\n';
		file << std::setfill(' ');
		file << std::left << std::setw(15) << "Vrsta poreza" << std::setw(12) << "Stopa (\%)"
			 << std::setw(13) << "Osnovica" << std::setw(8) << "Iznos" << std::endl;
		file << std::left << std::setw(15) << "PDV 17%" << std::setw(12) << "17.00" << std::setw(13)
			 << sum - sum * 0.17 << std::setw(8) << sum * 0.17 << std::endl; // PDV hardkodovan
		file << std::format("{:^48}\n", "Hvala na posjeti!");
		file << std::setw(width) << std::setfill('=') << "" << '\n';

	} else {
		throw std::exception("File couldn't be opened\n");
	}
}

const std::string Database::current_date_time() {
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%d.%m.%Y. %X", &tstruct);

	return buf;
}
