#include "softver-za-prodavnice/user.hpp"


User::User(std::string username, std::string password, Position position, std::size_t number_of_logins) 
	: username(username), password(password), position(position), number_of_logins(number_of_logins) {}

User::User(const User& other)
	: username(other.username), password(other.password), position(other.position),
	  number_of_logins(other.number_of_logins) {}


std::string User::get_username() const { return username; }

std::string User::get_password() const { return password; }

Position User::get_position() const { return position; }

std::size_t User::get_number_of_logins() const { return number_of_logins; }
