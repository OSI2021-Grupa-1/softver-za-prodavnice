#pragma once
#include <string>


class User {
  private:
	std::string username;
	std::string password;

  public:
	void set_username(std::string usr);
	void set_password(std::string pw);
	std::string get_username() const;
	std::string get_password() const;

	void change_password();
	void log_out();

};
