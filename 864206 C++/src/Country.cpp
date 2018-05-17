#include <iostream>

#include "Country.h"

using namespace std;

Country::Country(const std::string& name) : Region(name) {
}

Country::~Country() {
}

void Country::Display() {
	cout << "Отображение на карте: " << name << " (" << kind << ")" << endl;;
}
