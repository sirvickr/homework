#include <iostream>

#include "Exceptions.h"
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
	try {
	Country* country = new Country("Россия");
	Province* province = new Province("Самарская область");
	Mammal* mammal = new Mammal("Бурый медведь", 220.5);
	Reptile* reptile = new Reptile("Китайский аллигатор", 170.0);
//	cout << country->Name() << " (" << province->Name() << ")" << endl; // prints !!!Hello World!!!

	int counter = 0;
    int menu = 0;
    while (menu != 11) {
        cout << endl << endl;
        cout << "[1] Список всех территориальных единиц" << endl;
        cout << "[2] Список всех животных" << endl;
        cout << "[3] Список животных указанного вида в конкретной стране" << endl;
        cout << "[4] Список млекопитающих в конкретной территориальной единице" << endl;
        cout << "[5] Список ядовитых пресмыкающихся в конкретной территориальной единице" << endl;
        cout << "[6] Список млекопитающих с массой тела больше указанной" << endl;
        cout << "[7] Добавить территориальную единицу" << endl;
        cout << "[8] Удалить территориальную единицу" << endl;
        cout << "[9] Добавить животное" << endl;
        cout << "[10] Удалить животное" << endl;
        cout << "[11] Выход" << endl << endl;
        cout << "Ваш выбор: ";
        cin.sync();
        cin >> menu;

        switch(menu) {
        case 1:
            //ListRegions();
        	break;
        case 2:
            //ListAnimals();
        	break;
        case 3:
        	//ListAnimalsInCountry();
        	break;
        case 4:
        	//ListMammalsInRegion();
        	break;
        case 5:
        	//ListToxicReptiles();
        	break;
        case 6:
        	//ListHeavyMammals();
        	break;
        case 7:
        	//AddRegion();
        	break;
        case 8:
        	//DeleteRegion();
        	break;
        case 9:
        	//AddAnimal();
        	break;
        case 10:
        	//DeleteAnimal();
        	break;
        }
        if(counter++ == 10) {
        	cout << "emergency exit" << endl;
        	break;
		}
    }

	delete country;
	delete province;
	delete mammal;
	delete reptile;
	} catch(TooManyInstances& ex) {
		cerr << "too many instances: " << ex.MaxCount() <<  endl;
	}
	return 0;
}
