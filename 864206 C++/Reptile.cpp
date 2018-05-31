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
	cout << kind << " (��������������, ��� " << weight << " ��";
	if(toxic)
		cout << ", ��������";
	cout << ")" << endl;
}
