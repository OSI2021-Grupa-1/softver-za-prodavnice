#pragma once
#include <filesystem>
#include <string>

class Store {
	std::string name{};
	std::string address{};
	std::string phone{};

  public:
	Store() = default;
	Store(std::string name, std::string address, std::string phone)
		: name(std::move(name)), address(std::move(address)), phone(std::move(phone)) {}
	std::string get_name() const { return name; }
	std::string get_address() const { return address; }
	std::string get_phone() const { return phone; }
};
