#pragma once
#include "user.hpp"
#include <vector>

namespace file {
//bool write_user(const User& user) const;
vector<User*> read_users(std:string filename);
User* parse_user(const std::string& line);
}

