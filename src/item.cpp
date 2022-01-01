#include "softver-za-prodavnice/item.hpp"

Item::Item(std::string barcode, std::string name, double price, double quantity)
	: barcode(barcode), name(name), price(price), quantity(quantity) {}

Item::Item(const Item& other)
	: barcode(other.barcode), name(other.name), price(other.price), quantity(other.quantity) {}

bool Item::operator==(const Item& other) const {
	if (barcode == other.barcode) return true;
	else
		return false;
}

void Item::set_price(double price) { this->price = price; }

void Item::set_quantity(double quantity) { this->quantity = quantity; }

std::string Item::get_barcode() const { return barcode; }

std::string Item::get_name() const { return name; }

double Item::get_price() const { return price; }

double Item::get_quantity() const { return quantity; }

std::ostream& operator<<(std::ostream& os, const Item& item) {
	os << item.barcode << "φ" << item.name << "φ" << item.price << "φ" << item.quantity;
	return os;
}
