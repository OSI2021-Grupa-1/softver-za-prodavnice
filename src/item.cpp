#include "softver-za-prodavnice/item.hpp"

Item::Item(std::string barcode, std::string name, double price, std::size_t quantity)
	: barcode(barcode), name(name), price(price), quantity(quantity) {}

Item::Item(const Item& other)
	: barcode(other.barcode), name(other.name), price(other.price), quantity(other.quantity) {}


bool Item::operator==(const Item& other) const {
	if (barcode == other.barcode) return true;
	else
		return false;
}

void Item::set_price(double price) { this->price = price; }

void Item::set_quantity(std::size_t quantity) { this->quantity = quantity; }


std::string Item::get_barcode() const { return barcode; }

std::string Item::get_name() const { return name; }

double Item::get_price() const { return price; }

std::size_t Item::get_quantity() const { return quantity; }
