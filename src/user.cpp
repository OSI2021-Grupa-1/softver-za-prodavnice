#include "softver-za-prodavnice/user.hpp"

void User::set_username(std::string usr) { username = usr; }

void User::set_password(std::string pw) { password = pw; }

std::string User::get_username() const { return username; }

std::string User::get_password() const { return password; }

void User::change_password(const std::string& new_pw) { 
	password = new_pw;
	// upis u fajl
}

void User::log_out() {

}

bool User::check_password_validity(const std::string& input) {
	if (input == password) 
		return true;
	else
		return false;
}

bool User::passwords_equal(const std::string& original, const std::string& confirmation) {
	if (original == confirmation) 
		return true;
	else
		return false;
}
