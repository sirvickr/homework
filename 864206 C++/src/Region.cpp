#include "Region.h"

#include <iostream>

using namespace std;

size_t Region::instCount = 0;

Region::Region(const std::string& name) : name(name) {
	cout << "new Region: " << name << endl;
	instCount++;
}

// пустая заглушка для виртуального деструктора
Region::~Region() {
	cout << "del Region: " << name << endl;
	instCount--;
}

//static
size_t Region::InstanceCount() {
	return instCount;
}
