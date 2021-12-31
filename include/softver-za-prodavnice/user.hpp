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

	void change_password(const std::string& new_pw);
	void log_out();


	// interfejsi
	void change_password_interface();
	void log_out_interface();
	

  private:
	bool check_password_validity(const std::string& input);
	bool passwords_equal(const std::string& original, const std::string& confirmation);

};
