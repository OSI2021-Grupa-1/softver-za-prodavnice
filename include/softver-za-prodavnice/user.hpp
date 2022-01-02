#pragma once

#include <cstddef>
#include <ostream>
#include <string>

enum class Position {
	worker,
	supervisor,
};

class User {
	std::string username{};
	std::string password{};
	std::string position{};
	std::size_t number_of_logins{};

  public:
	User() = default;
	User(std::string username, std::string password, std::string position,
		 std::size_t number_of_logins);
	User(const User& other);

	bool operator==(const User& other) const; // potrebno za std::remove
	User& operator=(const User& other);

	std::string get_username() const;
	std::string get_password() const;
	std::string get_position() const;
	std::size_t get_number_of_logins() const;

	void set_password(const std::string& pw);
	friend std::ostream& operator<<(std::ostream& os, const User& user);
};
