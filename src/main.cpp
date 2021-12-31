#include <memory>
#include <string>
#include "softver-za-prodavnice/config.hpp"

int main() {
	Config paths;
	paths.load_paths("config.txt");
	paths.print();
}
