#include <iostream>

#include "Province.h"

using namespace std;

Province::Province(const std::string& name) : Region(name) {
}

Province::~Province() {
}

void Province::Display(bool brief) const {
	if(!brief)
		cout << "Область:";
	cout << " " << name << endl;
}
