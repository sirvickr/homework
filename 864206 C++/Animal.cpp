#include "Animal.h"
#include "Province.h"

#include <iostream>

using namespace std;

size_t Animal::instCount = 0;

Animal::Animal(const std::string& kind, double weight) : kind(kind), weight(weight) {
	cout << "new Animal: kind " << kind << " weight " << weight << endl;
	instCount++;
}

// пустая заглушка для чисто виртуального деструктора
Animal::~Animal() {
	cout << "del Animal: kind " << kind << " weight " << weight << endl;
	instCount--;
}

//static
size_t Animal::InstanceCount() {
	return instCount;
}
