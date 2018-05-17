#include <iostream>

#include "Country.h"
#include "Province.h"
#include "Reptile.h"
#include "Mammal.h"

using namespace std;
/*
static const size_t MAX_REGIONS_COUNT = 10;
static TerritorialUnit* regions[MAX_REGIONS_COUNT] = { 0 };
static size_t regionsCount = 0;

static const size_t MAX_ANIMALS_COUNT = 10;
static Animal* animals[MAX_ANIMALS_COUNT] = { 0 };
static size_t animalsCount = 0;
*/
int main() {
	Country* country = new Country("Россия");
	Province* province = new Province("Самарская область");
	Mammal* mammal = new Mammal();
	Reptile* reptile = new Reptile();
	cout << country->Name() << " (" << province->Name() << ")" << endl; // prints !!!Hello World!!!

	delete country;
	delete province;
	delete mammal;
	delete reptile;
	return 0;
}
