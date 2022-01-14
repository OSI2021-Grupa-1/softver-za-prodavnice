#pragma once
#include <ostream>
#include <string>

class Item {
	std::string barcode{};
	std::string name{};
	double price{};
	double quantity{};

  public:
	Item() = default;
	Item(std::string barcode, std::string name, double price, double quantity);
	Item(const Item& other);

	bool operator==(const Item& other) const; // potrebno za std::remove
	Item& operator=(const Item& other);

	void set_price(double price);
	void set_quantity(double quantity);

	std::string get_barcode() const;
	std::string get_name() const;
	double get_price() const;
	double get_quantity() const;
	friend std::ostream& operator<<(std::ostream& os, const Item& item);
};
