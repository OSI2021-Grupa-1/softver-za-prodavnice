#include "softver-za-prodavnice/files.hpp"

vector<User*> file::read_users(std::string filename) {
	vector<User*> ret;
	std::string temp_line;
	if (auto file = std::ifstream(filename)) {
		while (std::getline(file, temp_line)) {
			User* user = parse_user(temp_line);
			ret.push_back(user);
		}
	} else {
		throw std::exception("File couldn't be opened\n");
	}
	return ret;
}
User* file::parse_user(const std::string& line) { 
	size_t max_token_amount = 2;
	std::string tokens[max_token_amount]{};
	size_t number_of_tokens = 0;
	size_t start = 0;
	size_t end = 0;
	while (end <= line.length()) {
		assert(number_of_tokens < max_token_amount);
		if (line[end] == ' ') {
			tokens[number_of_tokens++] = line.substr(start, end - start);
			start = end + 1;
		} else if (end == line.length()) {
			tokens[number_of_tokens++] = line.substr(start, end - start);
		}
		++end;
	}
	return new User(tokens[0], tokens[1]);
}
