#include "Region.h"

#include "Exceptions.h"
#include "Animal.h"

#include <iostream>

using namespace std;

Region* Region::regions[MAX_REGIONS_COUNT];
size_t Region::regionsCount = 0;

Region::Region(const std::string& name) : name(name) {
	if(regionsCount == MAX_REGIONS_COUNT)
		throw TooManyInstances(MAX_REGIONS_COUNT);
	regions[regionsCount++] = this;

	cout << "Region::ctor(" << this << "):";
	for(size_t i = 0; i < regionsCount; i++)
		cout << " " << regions[i];
	cout << endl;
}

// пустая заглушка для виртуального деструктора
Region::~Region() {
	size_t i;
	for(i = 0; i < regionsCount; i++)
		if(regions[i] == this)
			break;
	if(i < regionsCount) {
		regionsCount--;
		for(size_t j = i; j < regionsCount; j++) {
			regions[j] = regions[j + 1];
		}
	}

	cout << "Region::dtor(" << this << "):";
	for(i = 0; i < regionsCount; i++)
		cout << " " << regions[i];
	cout << endl;
}

void Region::AddAnimal(const Animal* animal) {
	animals.push_back(animal);
}
