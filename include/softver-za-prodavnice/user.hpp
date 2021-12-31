#pragma once

#include <cstddef>
#include <string>

enum class Position {
	worker,
	supervisor,
};

class User {
	std::string username{};
	std::string password{};
	Position position{};
	std::size_t number_of_logins{};

  public:
	User() = default;
	User(std::string username, std::string password, Position position,
		 std::size_t number_of_logins);
	User(const User& other);

	std::string get_username() const;
	std::string get_password() const;
	Position get_position() const;
	std::size_t get_number_of_logins() const;
};
