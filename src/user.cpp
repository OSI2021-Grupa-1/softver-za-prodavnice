#include "softver-za-prodavnice/user.hpp"
#include <string>

User::User(std::string username, std::string password, std::string position,
		   std::size_t number_of_logins)
	: username(username), password(password), position(position),
	  number_of_logins(number_of_logins) {}

User::User(const User& other)
	: username(other.username), password(other.password), position(other.position),
	  number_of_logins(other.number_of_logins) {}

bool User::operator==(const User& other) const {
	if (username == other.username) return true;
	return false;
}

User& User::operator=(const User& other) {
	if (this == &other) return *this;
	username = other.username;
	password = other.password;
	position = other.position;
	number_of_logins = other.number_of_logins;
	return *this;
}

std::string User::get_username() const { return username; }

std::string User::get_password() const { return password; }

std::string User::get_position() const { return position; }

std::size_t User::get_number_of_logins() const { return number_of_logins; }

void User::set_password(const std::string& pw) { password = pw; }
std::ostream& operator<<(std::ostream& os, const User& user) {
	os << user.username << "φ" << user.password << "φ" << user.position << "φ"
	   << user.number_of_logins;
	return os;
}
