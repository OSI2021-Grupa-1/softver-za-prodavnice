#pragma once
#include <string>


class Item {
	std::string barcode{};
	std::string name{};
	double price{};
	std::size_t quantity{};

  public:
	Item() = default;
	Item(std::string barcode, std::string name, double price, std::size_t quantity);
	Item(const Item& other);

	void set_price(double price);
	void set_quantity(std::size_t quantity);

	std::string get_barcode() const;
	std::string get_name()const;
	double get_price() const;
	std::size_t get_quantity() const;
};
