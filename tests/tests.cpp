#include <cassert>
#include <fstream>
#include <iostream>

int main() {
	if (auto file = std::ifstream("config.txt")) {
		std::cout << "Opened\n";
		return 0;
	} else {
		std::cout << "Filed\n";
		return 1;
	}
}
