#include <iostream>

#include "Province.h"

using namespace std;

Province::Province(const std::string& name) : Region(name) {
}

Province::~Province() {
}

void Province::Display() {
	cout << "Отображение на карте: " << name << " (" << kind << ")" << endl;;
}
