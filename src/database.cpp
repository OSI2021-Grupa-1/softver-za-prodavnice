#include "softver-za-prodavnice/database.hpp"

Database::Database(std::vector<User> user_data, std::vector<Item> item_data,
				   std::filesystem::path data_path)
	: user_data(std::move(user_data)), item_data(std::move(item_data)),
	  paths(std::move(data_path)) {}

void Database::set_user_data(std::vector<User> user_data) {
	this->user_data = std::move(user_data);
}
void Database::set_item_data(std::vector<Item> item_data) {
	this->item_data = std::move(item_data);
}

std::vector<User> Database::get_user_data() const { return user_data; }
std::vector<Item> Database::get_item_data() const { return item_data; }

void Database::delete_users(const std::vector<User>& users) {
	for (std::size_t i = 0; i < users.size(); i++) {
		user_data.erase(std::remove(user_data.begin(), user_data.end(), users[i]), user_data.end());
	}
	write_users_to_file(paths.get_path("korisnici"));
}

void Database::delete_items(const std::vector<Item>& items) {
	for (std::size_t i = 0; i < items.size(); i++) {
		item_data.erase(std::remove(item_data.begin(), item_data.end(), items[i]), item_data.end());
	}

	write_items_to_file(paths.get_path("artikli_na_stanju"));
}

void Database::change_password(const std::string& usr, const std::string& new_pw) {
	if (new_pw.length() < 8)
		throw std::length_error("Password too short"); // mora se primiti u interfejsu

	size_t index = find_user(usr);
	user_data[index].set_password(new_pw);

	write_users_to_file(paths.get_path("korisnici"));
}

bool Database::is_password_correct(const std::string& usr, const std::string& pw_input) const {
	size_t index = find_user(usr);

	if (pw_input == user_data[index].get_password()) return true;
	return false;
}

bool Database::are_passwords_equal(const std::string& original,
								   const std::string& confirmation) const {
	if (original == confirmation) return true;
	else
		return false;
}

size_t Database::find_user(const std::string& username) const {
	for (size_t i = 0; i < paths.get_size(); i++) {
		if (username == user_data[i].get_username()) return i;
	}
	throw std::invalid_argument("User couldn't be found");
	// ne bi ga trebao nikad baciti jer se username
	// prosljedjuje iz main-a i vec je provjeren
}

void Database::write_users_to_file(const std::string path) {
	if (auto file = std::ofstream(path)) {
		for (size_t i = 0; i < user_data.size(); ++i) {
			file << user_data[i] << "\n"; // da se upisuje i pozicija korisnika u fajl
		}
		file.close();
	} else {
		throw std::invalid_argument("File couldn't be opened");
	}
}

void Database::write_items_to_file(const std::string path) {
	if (auto file = std::ofstream(path)) {
		for (size_t i = 0; i < item_data.size(); ++i) {
			file << item_data[i] << "\n";
		}
		file.close();
	} else {
		throw std::invalid_argument("File couldn't be opened");
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

bool Database::check_item_availability(const std::string& other_barcode, const int& quantity) {

	int i;
	for (i = 0; i < item_data.size(); i++) {
		if (item_data[i].get_barcode() == other_barcode) break;
	}
	if (i == item_data.size()) return false;
	if (item_data[i].get_quantity() < quantity) return false;
	return true;
}

std::vector<Item> Database::create_report(const std::vector<Item>& items, const int& start_date,
										  const int& end_date, std::string& path) {
	std::vector<Item> report;
	std::string barcode, name, price, quantity, day, month, year;
	int position, date;
	if (auto file = std::ifstream(path)) {
		do {
			std::getline(file, barcode, '#');
			std::getline(file, name, '#');
			std::getline(file, price, '#');
			std::getline(file, quantity, '#');
			std::getline(file, day, '/');
			std::getline(file, month, '/');
			std::getline(file, year);

			date = stoi(year + month + day);

			if (date >= start_date && date <= end_date) {
				if (search_item_in_vector(items, barcode) !=
					-1) { // Provjerava da li se za ucitani artikal trazi izvjestaj
					if ((position = search_item_in_vector(report, barcode)) ==
						-1) { // Provjerava da li se artikal vec nalazi u izvjestaju
						Item new_item(barcode, name, std::stod(price), std::stod(quantity));
						report.push_back(new_item);
					} else {
						report[position].set_price(std::stod(price) + report[position].get_price());
						report[position].set_quantity(std::stod(quantity) +
													  report[position].get_quantity());
					}
				}
			}
		} while (date <= end_date);
		file.close();
	} else
		throw std::invalid_argument("File couldn't be opened");
	return report;
}

int Database::search_item_in_vector(const std::vector<Item>& vect, const std::string& barcode) {
	if (vect.size() == 0) return -1;
	int i;
	for (i = 0; i < vect.size(); i++) {
		if (vect[i].get_barcode() == barcode) return i;
	}
	return -1;
}

// provjera stanja dostupnosti se provjerava prije ove funkcije
void Database::generate_receipt(std::vector<std::pair<Item, double>> sold_items,
								std::string username) {
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
		file << util::helper(width, "Naziv prodavnice") << "\n";
		file << util::helper(width, "Naziv prodavnice") << "\n";
		file << util::helper(width, "Naziv prodavnice") << "\n";
		file << std::setw(width) << std::setfill('-') << '\n';
		file << std::left << "Datum i vrijeme: " << current_time << '\n';
		file << std::left << "Blagajnik: "
			 << "ph" << '\n';
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
					 << price << std::setw(8) << quantity << std::setw(10) << price * quantity
					 << "\n";
			else
				file << std::left << std::setw(23) << sold_items[i].first.get_name() << std::setw(9)
					 << std::fixed << std::setprecision(2) << price << std::setw(8) << quantity
					 << std::setw(10) << price * quantity << "\n";
		}
		file << std::setw(width) << std::setfill('-') << "" << '\n';
		file << std::setfill(' ');
		file << "UKUPNO:" << std::string(33, ' ') << sum << std::endl;
		file << std::setw(width) << std::setfill('-') << "" << '\n';
		file << std::setfill(' ');
		file << std::left << std::setw(15) << "Vrsta poreza" << std::setw(12)
			 << "Stopa (\%)" // Ovaj % urzokuje upozorenje
			 << std::setw(13) << "Osnovica" << std::setw(8) << "Iznos" << std::endl;
		file << std::left << std::setw(15) << "PDV 17%" << std::setw(12) << "17.00" << std::setw(13)
			 << sum - sum * 0.17 << std::setw(8) << sum * 0.17 << std::endl; // PDV hardkodovan
		file << util::helper(width, "Naziv prodavnice") << "\n";
		file << std::setw(width) << std::setfill('=') << "" << '\n';

	} else {
		throw std::invalid_argument("File couldn't be opened");
	}
}

void Database::write_sold_items_to_file(const std::vector<Item>& items, const std::string& date) {
	std::fstream tmp_file;
	std::fstream transaction_file;

	std::string putanja_do_pomocnog;	   // ovo treba zamjenit sa pravim putanjama do fajlova!!!!!!!!!!!!!
	std::string putanja_do_pravog;         // ovo treba zamjenit sa pravim putanjama do fajlova!!!!!!!!!!!!!

	tmp_file.open(putanja_do_pomocnog, std::ios::out);
	if (!tmp_file.is_open()) throw std::exception();

	transaction_file.open(putanja_do_pravog, std::ios::in);
	if (!transaction_file.is_open()) throw std::exception();

	for (size_t i = 0; i < items.size(); i++) {
		tmp_file << items[i] << "#" << date << std::endl;
	}
	tmp_file << transaction_file.rdbuf();
	
	tmp_file.close();
	transaction_file.close();

	tmp_file.open(putanja_do_pomocnog, std::ios::in);
	if (!tmp_file.is_open()) throw std::exception();

	transaction_file.open(putanja_do_pravog, std::ios::out);
	if (!transaction_file.is_open()) throw std::exception();

	transaction_file << tmp_file.rdbuf();

	tmp_file.close();
	transaction_file.close();

	tmp_file.open(putanja_do_pomocnog, std::ios::out);  // cisto da se prebrisu podaci u pomocnom fajlu kako ne bi trosili resurse
	if (!tmp_file.is_open()) throw std::exception();	// cisto da se prebrisu podaci u pomocnom fajlu kako ne bi trosili resurse
	tmp_file.close();									// cisto da se prebrisu podaci u pomocnom fajlu kako ne bi trosili resurse
}; 

const std::string Database::current_date_time() {
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%d.%m.%Y. %X", &tstruct);

	return buf;
}
