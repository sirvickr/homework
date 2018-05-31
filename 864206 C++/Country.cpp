#include <iostream>

#include "Country.h"
#include "Province.h"

using namespace std;

Country::Country(const std::string& name) : Region(name) {
}

Country::~Country() {
}

void Country::Display(bool brief) const {
	cout << name;
	if (!brief)
		cout << " (������)";
	cout << endl;
}
