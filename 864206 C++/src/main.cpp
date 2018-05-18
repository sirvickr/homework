#include <iostream>
#include <set>

#include "Country.h"
#include "Province.h"
#include "Reptile.h"
#include "Mammal.h"

using namespace std;

static const size_t MAX_REGIONS_COUNT = 20;
static Region* regions[MAX_REGIONS_COUNT] = { 0 };
static size_t regionsCount = 0;

static const size_t MAX_ANIMALS_COUNT = 100;
static Animal* animals[MAX_ANIMALS_COUNT] = { 0 };
static size_t animalsCount = 0;

typedef set< string > AnimalKinds;

template<class T>
void ListRegions(bool brief = false) {
	for(size_t i = 0; i < regionsCount; i++) {
		if(dynamic_cast< T* >(regions[i])) {
			cout << "[" << i << "] ";
			regions[i]->Display(brief);
		}
	}
}

// type: 0 - все; 1 - страны; 2 - области
/*void ListRegions(int type = 0, bool brief = false) {
	switch(type) {
	case 0:
		for(size_t i = 0; i < regionsCount; i++) {
			cout << "[" << i << "] ";
			regions[i]->Display(brief);
		}
		break;
	case 1:
		for(size_t i = 0; i < regionsCount; i++) {
			if(dynamic_cast< Country* >(regions[i])) {
				cout << "[" << i << "] ";
				regions[i]->Display(brief);
			}
		}
		break;
	case 2:
		for(size_t i = 0; i < regionsCount; i++) {
			if(dynamic_cast< Province* >(regions[i])) {
				cout << "[" << i << "] ";
				regions[i]->Display(brief);
			}
		}
		break;
	}
}*/

// type: 0 - все; 1 - млекопитающие; 2 - пресмыкающиеся
void ListAnimals(int type = 0) {
	switch(type) {
	case 0:
		for(size_t i = 0; i < animalsCount; i++) {
			cout << "[" << i << "] ";
			animals[i]->Display();
		}
		break;
	case 1:
		for(size_t i = 0; i < animalsCount; i++) {
			if(dynamic_cast< Mammal* >(animals[i])) {
				cout << "[" << i << "] ";
				animals[i]->Display();
			}
		}
		break;
	case 2:
		for(size_t i = 0; i < animalsCount; i++) {
			if(dynamic_cast< Reptile* >(animals[i])) {
				cout << "[" << i << "] ";
				animals[i]->Display();
			}
		}
		break;
	}
}

void ShowAnimal(size_t i) {
	cout << "[" << i << "] ";
	animals[i]->Display();
}

template<class FilterFn, class Operation>
void ProcessAnimals(FilterFn filter, Operation op) {
	for(size_t i = 0; i < animalsCount; i++) {
		if(filter(animals[i])) {
			op(animals[i]);
		}
	}
}

// Список животных указанного вида в конкретной стране
void ListAnimalsInCountry() {
	AnimalKinds kinds;

	int animalClass = 0;
    cout << "input animal class:" << endl;
    cout << "[1] Mammal" << endl;
    cout << "[2] Reptile" << endl;
    cout << "Ваш выбор: ";
    cin >> animalClass;
    if(animalClass != 1 && animalClass != 2) {
    	cerr << "wrong value" << endl;
    	return;
    }

	size_t index = 0;
	cout << "select country index:" << endl;
	ListRegions<Country>();
	cout << "your choice: ";
	cin >> index;
    if(index >= regionsCount) {
    	cerr << "wrong value" << endl;
    	return;
    }

	ProcessAnimals(
		// функция проверки каждого животного
		[animalClass, index](const Animal* animal) {
			return animal->Home()->ParentCountry() == regions[index] &&
			     ( animalClass == 1 ?
			       dynamic_cast< const Mammal* >(animal) != nullptr :
			       dynamic_cast< const Reptile* >(animal) != nullptr );
		},
		// функция обработки каждого животного, прошедшего проверку
		[&kinds](const Animal* animal) {
			kinds.insert(animal->Kind());
		}
	);

	for(const auto& kind: kinds) {
		cout << kind << endl;
	}
}

//Список млекопитающих в конкретной территориальной единице
void ListMammalsInRegion() {
	AnimalKinds kinds;

	size_t index = 0;
	cout << "select region index:" << endl;
	ListRegions<Region>();
	cout << "your choice: ";
	cin >> index;
    if(index >= regionsCount) {
    	cerr << "wrong value" << endl;
    	return;
    }

	ProcessAnimals(
		// функция проверки каждого животного
		[ index ]( const Animal* animal ) {
			return dynamic_cast< const Mammal* >( animal ) != nullptr &&
			     ( dynamic_cast< Country* >( regions[ index ] ) ?
			       animal->Home()->ParentCountry() == regions[ index ] :
			       animal->Home() == regions[ index ] );
		},
		// функция обработки каждого животного, прошедшего проверку
		[ &kinds ]( const Animal* animal ) {
			kinds.insert( animal->Kind() );
		}
	);

	for(const auto& kind: kinds) {
		cout << kind << endl;
	}
}

// Список ядовитых пресмыкающихся в конкретной территориальной единице
void ListToxicReptiles() {
	AnimalKinds kinds;

	size_t index = 0;
	cout << "select region index:" << endl;
	ListRegions<Region>();
	cout << "your choice: ";
	cin >> index;
    if(index >= regionsCount) {
    	cerr << "wrong value" << endl;
    	return;
    }

	ProcessAnimals(
		// функция проверки каждого животного
		[ index ]( const Animal* animal ) {
			return dynamic_cast< const Reptile* >( animal ) != nullptr &&
					dynamic_cast< const Reptile* >( animal )->Toxic() &&
			     ( dynamic_cast< Country* >( regions[ index ] ) ?
			       animal->Home()->ParentCountry() == regions[ index ] :
			       animal->Home() == regions[ index ] );
		},
		// функция обработки каждого животного, прошедшего проверку
		[ &kinds ]( const Animal* animal ) {
			kinds.insert( animal->Kind() );
		}
	);

	for(const auto& kind: kinds) {
		cout << kind << endl;
	}
}

// Список млекопитающих с массой тела больше указанной
void ListHeavyMammals() {
	double weight = 0;
	cout << "input minimal weight: ";
	cin >> weight;

	ProcessAnimals(
		// функция проверки каждого животного
		[ weight ]( const Animal* animal ) {
			return dynamic_cast< const Mammal* >( animal ) != nullptr && animal->Weight() > weight;
		},
		// функция обработки каждого животного, прошедшего проверку
		[]( const Animal* animal ) {
			animal->Display();
		}
	);
}

void RegisterRegion(Region* region) {
    if(region) {
    	regions[regionsCount++] = region;
    	/*cout << "Region::ctor(" << region << "):";
    	for(size_t i = 0; i < regionsCount; i++)
    		cout << " " << regions[i];
    	cout << endl;*/
    }
}

void AddRegion() {
	int regionClass = 0;
	string regionName;
	if(regionsCount == MAX_REGIONS_COUNT) {
		cerr << "too many regions: " << regionsCount <<  endl;
		return;
	}
    cout << "input what region to add:" << endl;
    cout << "[1] Country" << endl;
    cout << "[2] Province" << endl;
    cout << "your choice: ";
    cin >> regionClass;
    Region* region = nullptr;
    cout << "input region name: ";
    cin >> regionName;
	size_t index = 0;
    switch(regionClass) {
    case 1:
    	region = new Country(regionName);
    	break;
    case 2:
		cout << "input country to reside in:" << endl;
		ListRegions<Country>(true);
		cout << "your choice: ";
		cin >> index;
	    if(index < regionsCount) {
	    	Country* country = dynamic_cast< Country* >(regions[index]);
	    	if(country) {
				Province* province = new Province(regionName);
				province->ParentCountry(country);
				region = province;
	    	}
	    }
    	break;
    }
    RegisterRegion(region);
}

void DeleteRegion() {
	size_t index = 0;
	if(regionsCount == 0) {
		cout << "no regions to delete" << endl;
		return;
	} else if(regionsCount > 1) {
		cout << "input what region to delete:" << endl;
		ListRegions<Region>();
		cout << "your choice: ";
		cin >> index;
	} // otherwise, there is only one region (with index [0]) to delete
    if(index < regionsCount) {
    	Region* region = regions[index];

    	regionsCount--;
		for(size_t i = index; i < regionsCount; i++) {
			regions[i] = regions[i + 1];
		}
		cout << "Region::dtor(" << region << "):";
		for(size_t i = 0; i < regionsCount; i++)
			cout << " " << regions[i];
		cout << endl;

		delete region;
    }
}

void RegisterAnimal(Animal* animal) {
    if(animal) {
    	animals[animalsCount++] = animal;
    	/*cout << "Animal::ctor(" << animal << "):";
    	for(size_t i = 0; i < animalsCount; i++)
    		cout << " " << animals[i];
    	cout << endl;*/
    }
}

void AddAnimal() {
	int animalClass = 0;
	string animalKind;
	double weight = 0.0;
	bool toxic = false;
    Animal* animal = nullptr;
	Province* province = nullptr;

	if(animalsCount == MAX_ANIMALS_COUNT) {
		cerr << "too many animals: " << animalsCount <<  endl;
		return;
	}

	size_t index = 0;
	cout << "input home province:" << endl;
	ListRegions<Province>(true);
	cout << "your choice: ";
	cin >> index;
    if(index >= regionsCount) {
    	cerr << "wrong value" << endl;
    	return;
    }
	province = dynamic_cast< Province* >(regions[index]);

    cout << "input what animal to add:" << endl;
    cout << "[1] Mammal" << endl;
    cout << "[2] Reptile" << endl;
    cout << "your choice: ";
    cin >> animalClass;
    cout << "input animal kind: ";
    cin >> animalKind;
    cout << "input animal weight: ";
    cin >> weight;
    switch(animalClass) {
    case 1:
    	animal = new Mammal(animalKind, weight);
    	break;
    case 2:
        cout << "is it toxic (1/0)?: ";
        cin >> toxic;
        cout << "toxic: " << toxic << endl;
    	animal = new Reptile(animalKind, weight, toxic);
    	break;
    }
	animal->Home(province);
    RegisterAnimal(animal);
}

void DeleteAnimal() {
	size_t index = 0;
	if(animalsCount == 0) {
		cout << "no animals to delete" << endl;
		return;
	} else if(animalsCount > 1) {
		cout << "input what animal to delete:" << endl;
		ListAnimals();
		cout << "your choice: ";
		cin >> index;
	} // otherwise, there is only one animal (with index [0]) to delete
    if(index < animalsCount) {
    	Animal* animal = animals[index];

    	animalsCount--;
		for(size_t i = index; i < animalsCount; i++) {
			animals[i] = animals[i + 1];
		}
		cout << "Region::dtor(" << animal << "):";
		for(size_t i = 0; i < animalsCount; i++)
			cout << " " << animals[i];
		cout << endl;

		delete animal;
    }
}

void TestData();

int main() {
	//system("chcp 1251");
    //setlocale(LC_ALL, "RUS");

	//TestData(); // for debug

	int counter = 0;
    int menu = -1;
    while (menu != 0) {
        cout << endl << endl << "Main menu:" << endl;
        cout << "[0] Exit:                 Выход" << endl;
        cout << "[1] ListRegions:          Список всех территориальных единиц" << endl;
        cout << "[2] ListAnimals:          Список всех животных" << endl;
        cout << "[3] ListAnimalsInCountry: Список животных указанного вида в конкретной стране" << endl;
        cout << "[4] ListMammalsInRegion:  Список млекопитающих в конкретной территориальной единице" << endl;
        cout << "[5] ListToxicReptiles:    Список ядовитых пресмыкающихся в конкретной территориальной единице" << endl;
        cout << "[6] ListHeavyMammals:     Список млекопитающих с массой тела больше указанной" << endl;
        cout << "[7] AddRegion:            Добавить территориальную единицу" << endl;
        cout << "[8] DeleteRegion:         Удалить территориальную единицу" << endl;
        cout << "[9] AddAnimal:            Добавить животное" << endl;
        cout << "[10] DeleteAnimal:        Удалить животное" << endl;
        cout << "[11] Regions count        Общее число территориальных единиц" << endl;
        cout << "[12] Animals count:       Общее число животных" << endl;
        cout << "[13] TestData:            Создать тестовую иерархию объектов" << endl;

        cout  << endl;

        cout << "Ваш выбор: ";
        cin.sync();
        cin >> menu;

        switch(menu) {
        case 1:
            ListRegions<Region>();
        	break;
        case 2:
            ListAnimals();
        	break;
        case 3:
        	ListAnimalsInCountry();
        	break;
        case 4:
        	ListMammalsInRegion();
        	break;
        case 5:
        	ListToxicReptiles();
        	break;
        case 6:
        	ListHeavyMammals();
        	break;
        case 7:
        	AddRegion();
        	break;
        case 8:
        	DeleteRegion();
        	break;
        case 9:
        	AddAnimal();
        	break;
        case 10:
        	DeleteAnimal();
        	break;
        case 11:
        	cout << "Regions count: " << regionsCount << " = " << Region::InstanceCount() << endl;
        	break;
        case 12:
        	cout << "Animals count: " << animalsCount << " = " << Animal::InstanceCount() << endl;
        	break;
        case 13:
        	TestData();
        	break;
        }
        if(counter++ == 10) {
        	cout << "emergency exit" << endl;
        	break;
		}
        //break; // for debug
    }

	for(size_t i = 0; i < regionsCount; i++) {
		delete regions[i];
	}
	for(size_t i = 0; i < animalsCount; i++) {
		delete animals[i];
	}

    cout << "Работа завершена" << endl;
	return 0;
}

// Создать тестовую иерархию объектов
void TestData() {
	Province* province = nullptr;

	// [0]
    RegisterRegion(new Country("Russia"));
	// [1]
    RegisterRegion(new Country("China"));

	// [2]
	province = new Province("Samara");
	province->ParentCountry(dynamic_cast< Country* >(regions[0]));
	RegisterRegion(province);
	// [3]
	province = new Province("Astrakhan");
	province->ParentCountry(dynamic_cast< Country* >(regions[0]));
	RegisterRegion(province);
	// [4]
	province = new Province("Khabarovsk");
	province->ParentCountry(dynamic_cast< Country* >(regions[0]));
	RegisterRegion(province);
	// [5]
	province = new Province("Spb");
	province->ParentCountry(dynamic_cast< Country* >(regions[0]));
	RegisterRegion(province);
	// [6]
	province = new Province("Sichuan"); // Сычуань
	province->ParentCountry(dynamic_cast< Country* >(regions[1]));
	RegisterRegion(province);
	// [7]
	province = new Province("Hainan"); // Хайнань
	province->ParentCountry(dynamic_cast< Country* >(regions[1]));
	RegisterRegion(province);
	// [8]
	province = new Province("Shandun"); // Шаньдун
	province->ParentCountry(dynamic_cast< Country* >(regions[1]));
	RegisterRegion(province);
	// [9]
	province = new Province("Laonin"); // Ляонин
	province->ParentCountry(dynamic_cast< Country* >(regions[1]));
	RegisterRegion(province);

	Animal* animal = nullptr;

	// [0]
	animal = new Mammal("Elk", 145.0); // Лось
	animal->Home(dynamic_cast< Province* >(regions[2]));
    RegisterAnimal(animal);
	// [1]
	animal = new Mammal("Elk", 208.0); // Лось
	animal->Home(dynamic_cast< Province* >(regions[4]));
    RegisterAnimal(animal);
	// [2]
	animal = new Mammal("Elk", 181.0); // Лось
	animal->Home(dynamic_cast< Province* >(regions[6]));
    RegisterAnimal(animal);
	// [3]
	animal = new Mammal("Bear", 287.0); // Бурый медведь
	animal->Home(dynamic_cast< Province* >(regions[2]));
    RegisterAnimal(animal);
	// [4]
	animal = new Mammal("Bear", 225.0); // Бурый медведь
	animal->Home(dynamic_cast< Province* >(regions[3]));
    RegisterAnimal(animal);
	// [5]
	animal = new Mammal("Bear", 344.0); // Бурый медведь
	animal->Home(dynamic_cast< Province* >(regions[4]));
    RegisterAnimal(animal);
	// [6]
	animal = new Mammal("Bear", 250.0); // Бурый медведь
	animal->Home(dynamic_cast< Province* >(regions[5]));
    RegisterAnimal(animal);
    // [7]
	animal = new Mammal("Muskrat", 25.0); // Ондатра
	animal->Home(dynamic_cast< Province* >(regions[2]));
    RegisterAnimal(animal);
    // [8]
	animal = new Mammal("Muskrat", 28.0); // Ондатра
	animal->Home(dynamic_cast< Province* >(regions[8]));
    RegisterAnimal(animal);
	// [9]
	animal = new Mammal("Panda", 65.0); // Панда
	animal->Home(dynamic_cast< Province* >(regions[6]));
    RegisterAnimal(animal);
	// [10]
	animal = new Mammal("Panda", 57.0); // Панда
	animal->Home(dynamic_cast< Province* >(regions[7]));
    RegisterAnimal(animal);
	// [11]
	animal = new Mammal("Panda", 68.0); // Панда
	animal->Home(dynamic_cast< Province* >(regions[8]));
    RegisterAnimal(animal);
	// [12]
	animal = new Mammal("Panda", 61.0); // Панда
	animal->Home(dynamic_cast< Province* >(regions[9]));
    RegisterAnimal(animal);
	// [13]
    animal = new Reptile("Viper", 0.23, true); // Обыкновенная гадюка
	animal->Home(dynamic_cast< Province* >(regions[2]));
    RegisterAnimal(animal);
	// [14]
    animal = new Reptile("Viper", 0.51, true); // Обыкновенная гадюка
	animal->Home(dynamic_cast< Province* >(regions[3]));
    RegisterAnimal(animal);
	// [15]
    animal = new Reptile("Viper", 0.5, true); // Обыкновенная гадюка
	animal->Home(dynamic_cast< Province* >(regions[4]));
    RegisterAnimal(animal);
	// [16]
    animal = new Reptile("Viper", 0.43, true); // Обыкновенная гадюка
	animal->Home(dynamic_cast< Province* >(regions[5]));
    RegisterAnimal(animal);
	// [17]
    animal = new Reptile("Viper", 0.38, true); // Обыкновенная гадюка
	animal->Home(dynamic_cast< Province* >(regions[6]));
    RegisterAnimal(animal);
	// [18]
    animal = new Reptile("Viper", 0.37, true); // Обыкновенная гадюка
	animal->Home(dynamic_cast< Province* >(regions[9]));
    RegisterAnimal(animal);
	// [19]
    animal = new Reptile("Tortoise", 5.5, false); // Дальневосточная черепаха
	animal->Home(dynamic_cast< Province* >(regions[4]));
    RegisterAnimal(animal);
	// [20]
    animal = new Reptile("Tortoise", 6.1, false); // Дальневосточная черепаха
	animal->Home(dynamic_cast< Province* >(regions[6]));
    RegisterAnimal(animal);
	// [21]
    animal = new Reptile("Tortoise", 5.8, false); // Дальневосточная черепаха
	animal->Home(dynamic_cast< Province* >(regions[7]));
    RegisterAnimal(animal);
	// [22]
    animal = new Reptile("Alligator", 287.0, false); // Китайский аллигатор
	animal->Home(dynamic_cast< Province* >(regions[8]));
    RegisterAnimal(animal);
	// [23]
    animal = new Reptile("Alligator", 302.0, false); // Китайский аллигатор
	animal->Home(dynamic_cast< Province* >(regions[9]));
    RegisterAnimal(animal);
	// [24]
    animal = new Reptile("Carpet viper", 0.65, true); // Эфа
	animal->Home(dynamic_cast< Province* >(regions[7]));
    RegisterAnimal(animal);
}
