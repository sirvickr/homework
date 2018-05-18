#include "Animal.h"
#include "Exceptions.h"

#include <iostream>

using namespace std;

size_t Animal::animalsCount = 0;
Animal* Animal::animals[MAX_ANIMALS_COUNT] = { 0 };

Animal::Animal(const std::string& kind, double weight) : kind(kind), weight(weight) {
	if(animalsCount == MAX_ANIMALS_COUNT)
		throw TooManyInstances(MAX_ANIMALS_COUNT);
	animals[animalsCount++] = this;

	cout << "Animal::ctor(" << this << "):";
	for(size_t i = 0; i < animalsCount; i++)
		cout << " " << animals[i];
	cout << endl;
}

// пустая заглушка для виртуального деструктора
Animal::~Animal() {
	size_t i;
	for(i = 0; i < animalsCount; i++)
		if(animals[i] == this)
			break;
	if(i < animalsCount) {
		animalsCount--;
		for(size_t j = i; j < animalsCount; j++) {
			animals[j] = animals[j + 1];
		}
	}

	cout << "Animal::dtor(" << this << "):";
	for(i = 0; i < animalsCount; i++)
		cout << " " << animals[i];
	cout << endl;
}
