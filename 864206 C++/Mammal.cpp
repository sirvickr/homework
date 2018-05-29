#include "Mammal.h"

#include <iostream>

using namespace std;

Mammal::Mammal() : Animal("mammal", 0.0) {
}

Mammal::Mammal(const std::string& kind, double weight) : Animal(kind, weight) {
}

Mammal::~Mammal() {
}

void Mammal::Display() const {
	cout << kind << " (млекопитающее, вес " << weight << " кг)" << endl;
}
