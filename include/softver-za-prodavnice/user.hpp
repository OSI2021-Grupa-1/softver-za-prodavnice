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
};
