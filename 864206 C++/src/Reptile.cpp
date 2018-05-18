#include "Reptile.h"

#include <iostream>

using namespace std;

Reptile::Reptile() : Animal("reptile", weight), toxic(false) {
}

Reptile::Reptile(const std::string& kind, double weight, bool toxic) : Animal(kind, weight), toxic(toxic) {
}

Reptile::~Reptile() {
}

void Reptile::Display() const {
	cout << "Reptile: kind " << kind << " weight " << weight;
	if(toxic)
		cout << " (toxic)";
	cout << endl;
}
