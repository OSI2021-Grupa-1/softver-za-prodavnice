#include <fstream>
#include <memory>
#include <string>
#include "softver-za-prodavnice/config.hpp"
#include "softver-za-prodavnice/util.hpp"

int main(int argc, char** argv) {
	Config paths(util::get_data_path(argv[0]));
	paths.load_paths("config.txt");
	paths.print();
}
