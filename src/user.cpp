#include "softver-za-prodavnice/user.hpp"

void User::set_username(std::string usr) { username = usr; }

void User::set_password(std::string pw) { password = pw; }

std::string User::get_username() const { return username; }

std::string User::get_password() const { return password; }

void User::change_password() {
	std::string new_password;
	// interfejs za unos stare lozinke, provjera validnosti, unos nove lozinke i njeno potvrdjivanje, provjera jednakosti

}

void User::log_out() {

}

bool User::check_password_validity(std::string input) {
	if (input == password) 
		return true;
	else
		return false;
}

bool User::passwords_equal(std::string input_1, std::string input_2) {
	if (input_1 == input_2) 
		return true;
	else
		return false;
}
