// dog.h puppy tester (puppy.tester.cpp)
// version 0400

// to compile the puppy.tester, run: 'g++ -std=c++17 -Wall -o puppy.tester puppy.tester.cpp'

#include <iostream>

// include the dog.h header file
#include "dog.h"

int main() {
	// initialize a constant string with the config filename
	const std::string filename = "dog.cfg";

	// examples
	// create a new empty config file
	if (!pup::Create(filename)) {
		std::cerr << "Error! Failed to create " << filename << "\n";
		return -1;
	}
	
	// load a config file into memory
	pup::Config cfg = pup::Load(filename);

	// add some values
	pup::Edit(cfg, "dog", "name", "Vasilij");
	pup::Edit(cfg, "dog", "hp", "10");
	pup::Edit(cfg, "dog", "evil", "true");
	
	// save edited values to the config file
	if (!pup::Save(filename, cfg)) {
		std::cerr << "Error! Failed to save " << filename << "\n";
		return -1;
	}

	// view the config file
	std::cout << "=== Config ===\n" << pup::View(cfg) << "\n";
	// pup::View basically returns the whole config file's contents

	// read values back
	std::string name = pup::Get<std::string>(cfg, "dog", "name", "Unknown");
	int hp = pup::Get<int>(cfg, "dog", "hp", 1);
	bool evil = pup::Get<bool>(cfg, "dog", "evil", false);

	std::cout
	<< "\nDog's name: " << name
	<< "\nTotal HP: " << hp
	<< "\nIs evil? " << (evil ? "Yes!" : "No!") << "\n";

	return 0;
}
