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

int InputRegionClass() {
	int result = 0;
	cout << "Выберите класс добавляемой территориальной единицы:" << endl;
	cout << "[1] Страна" << endl;
	cout << "[2] Область" << endl;
	cout << "Ваш выбор: ";
	cin >> result;
	if (result != 1 && result != 2) {
		cerr << "Ошибка: введено недопустимое значение" << endl;
		result = -1;
	}
	return result;
}

int InputAnimalClass() {
	int result = 0;
	cout << "Выберите класс животного:" << endl;
	cout << "[1] Млекопитающее" << endl;
	cout << "[2] Пресмыкающееся" << endl;
	cout << "Ваш выбор: ";
	cin >> result;
	if (result != 1 && result != 2) {
		cerr << "Ошибка: введено недопустимое значение" << endl;
		result = -1;
	}
	return result;
}

template<class T>
size_t InputRegion(const string& title) {
	size_t result = 0;
	cout << title << endl;
	ListRegions<T>(false);
	cout << "Ваш выбор: ";
	cin >> result;
	if (result >= regionsCount) {
		cerr << "Ошибка: введено недопустимое значение" << endl;
		result = regionsCount;
	}
	return result;
}

template<class T>
void ListRegions(bool brief, bool showCount = false) {
	if (showCount)
		cout << "Regions count: " << regionsCount << " = " << Region::InstanceCount() << endl;
	for (size_t i = 0; i < regionsCount; i++) {
		if (dynamic_cast< T* >(regions[i])) {
			cout << "[" << i << "] ";
			regions[i]->Display(brief);
		}
	}
}

template<class T>
void ListAnimals(bool showCount = false) {
	if (showCount)
		cout << "Animals count: " << animalsCount << " = " << Animal::InstanceCount() << endl;
	for (size_t i = 0; i < animalsCount; i++) {
		if (dynamic_cast< T* >(animals[i])) {
			cout << "[" << i << "] ";
			animals[i]->Display();
		}
	}
}

template<class Filter, class Operation>
void ProcessAnimals(Filter filter, Operation op) {
	for (size_t i = 0; i < animalsCount; i++) {
		if (filter(animals[i])) {
			op(animals[i]);
		}
	}
}

// Список животных указанного вида в конкретной стране
void ListAnimalsInCountry() {
	AnimalKinds kinds;

	int animalClass = InputAnimalClass();
	if (animalClass < 0) {
		return;
	}

	size_t index = InputRegion<Country>("Выберите страну:");
	if (index == regionsCount)
		return;

	ProcessAnimals(
		// функция проверки каждого животного
		[animalClass, index](const Animal* animal) {
		return animal->Home()->ParentCountry() == regions[index] &&
			(animalClass == 1 ?
				dynamic_cast< const Mammal* >(animal) != nullptr :
				dynamic_cast< const Reptile* >(animal) != nullptr);
	},
		// функция обработки каждого животного, прошедшего проверку
		[&kinds](const Animal* animal) {
		kinds.insert(animal->Kind());
	}
	);

	for (const auto& kind : kinds) {
		cout << kind << endl;
	}
}

// Список млекопитающих в конкретной территориальной единице
void ListMammalsInRegion() {
	AnimalKinds kinds;

	size_t index = InputRegion<Region>("Выберите территориальную единицу:");
	if (index == regionsCount)
		return;

	ProcessAnimals(
		// функция проверки каждого животного
		[index](const Animal* animal) {
		return dynamic_cast< const Mammal* >(animal) != nullptr &&
			(dynamic_cast< Country* >(regions[index]) ?
				animal->Home()->ParentCountry() == regions[index] :
				animal->Home() == regions[index]);
	},
		// функция обработки каждого животного, прошедшего проверку
		[&kinds](const Animal* animal) {
		kinds.insert(animal->Kind());
	}
	);

	for (const auto& kind : kinds) {
		cout << kind << endl;
	}
}

// Список ядовитых пресмыкающихся в конкретной территориальной единице
void ListToxicReptiles() {
	AnimalKinds kinds;

	size_t index = InputRegion<Region>("Выберите территориальную единицу:");
	if (index == regionsCount)
		return;

	ProcessAnimals(
		// функция проверки каждого животного
		[index](const Animal* animal) {
		return dynamic_cast< const Reptile* >(animal) != nullptr &&
			dynamic_cast< const Reptile* >(animal)->Toxic() &&
			(dynamic_cast< Country* >(regions[index]) ?
				animal->Home()->ParentCountry() == regions[index] :
				animal->Home() == regions[index]);
	},
		// функция обработки каждого животного, прошедшего проверку
		[&kinds](const Animal* animal) {
		kinds.insert(animal->Kind());
	}
	);

	for (const auto& kind : kinds) {
		cout << kind << endl;
	}
}

// Список млекопитающих с массой тела больше указанной
void ListHeavyMammals() {
	double weight = 0;
	cout << "Введите минимальную массу животного: ";
	cin >> weight;

	ProcessAnimals(
		// функция проверки каждого животного
		[weight](const Animal* animal) {
		return dynamic_cast< const Mammal* >(animal) != nullptr && animal->Weight() > weight;
	},
		// функция обработки каждого животного, прошедшего проверку
		[](const Animal* animal) {
		animal->Display();
	}
	);
}

void RegisterRegion(Region* region) {
	if (region) {
		regions[regionsCount++] = region;
		/*cout << "Region::ctor(" << region << "):";
		for(size_t i = 0; i < regionsCount; i++)
		cout << " " << regions[i];
		cout << endl;*/
	}
}

void AddProvince(size_t countryIndex, const std::string& name) {
	Province* province = new Province(name);
	province->ParentCountry(dynamic_cast< Country* >(regions[countryIndex]));
	RegisterRegion(province);
}

void AddRegion() {
	string regionName;
	if (regionsCount == MAX_REGIONS_COUNT) {
		cerr << "Достигнуто предельное количество экземпляров: " << regionsCount << endl;
		return;
	}

	int regionClass = InputRegionClass();
	if (regionClass < 0) {
		return;
	}

	Region* region = nullptr;
	cout << "Введите название добавляемой территориальной единицы: ";
	cin >> regionName;
	size_t index = 0;
	switch (regionClass) {
	case 1:
		region = new Country(regionName);
		break;
	case 2:
		index = InputRegion<Country>("Выберите страну, которой принадлежит добавляемая область:");
		if (index == regionsCount)
			return;
		Country* country = dynamic_cast< Country* >(regions[index]);
		if (country) {
			Province* province = new Province(regionName);
			province->ParentCountry(country);
			region = province;
		}
		break;
	}
	RegisterRegion(region);
}

void DeleteRegion() {
	size_t index = 0;
	if (regionsCount == 0) {
		cout << "Список территориальных единиц пуст, нечего удалять" << endl;
		return;
	}
	else if (regionsCount > 1) {
		index = InputRegion<Region>("Выберите территориальную единицу для удаления:");
		if (index == regionsCount) {
			return;
		}
	} // В противном случае, есть только одна территориальная единица (с индексом [0]) для удаления
	Region* region = regions[index];

	regionsCount--;
	for (size_t i = index; i < regionsCount; i++) {
		regions[i] = regions[i + 1];
	}
	/*cout << "Region::dtor(" << region << "):";
	for (size_t i = 0; i < regionsCount; i++)
		cout << " " << regions[i];
	cout << endl;*/

	delete region;
}

void RegisterAnimal(Animal* animal) {
	if (animal) {
		animals[animalsCount++] = animal;
		/*cout << "Animal::ctor(" << animal << "):";
		for(size_t i = 0; i < animalsCount; i++)
		cout << " " << animals[i];
		cout << endl;*/
	}
}

void AddMammal(size_t regionIndex, const std::string& kind, double weight) {
	Animal* animal = new Mammal(kind, weight);
	animal->Home(dynamic_cast< Province* >(regions[regionIndex]));
	RegisterAnimal(animal);
}

void AddReptile(size_t regionIndex, const std::string& kind, double weight, bool toxic) {
	Animal* animal = new Reptile(kind, weight, toxic);
	animal->Home(dynamic_cast< Province* >(regions[regionIndex]));
	RegisterAnimal(animal);
}

void AddAnimal() {
	string animalKind;
	double weight = 0.0;
	bool toxic = false;
	Animal* animal = nullptr;
	Province* province = nullptr;

	if (animalsCount == MAX_ANIMALS_COUNT) {
		cerr << "Достигнуто предельное количество экземпляров: " << animalsCount << endl;
		return;
	}

	size_t index = 0;
	cout << "Выберите ареал обитания животного:" << endl;
	ListRegions<Province>(true);
	cout << "Ваш выбор: ";
	cin >> index;
	if (index >= regionsCount) {
		cerr << "Ошибка: введено недопустимое значение" << endl;
		return;
	}
	province = dynamic_cast< Province* >(regions[index]);

	int animalClass = InputAnimalClass();
	if (animalClass < 0)
		return;
	cout << "Введите название (вид) животного: ";
	cin >> animalKind;
	cout << "Введите вес животного: ";
	cin >> weight;
	switch (animalClass) {
	case 1:
		animal = new Mammal(animalKind, weight);
		break;
	case 2:
		cout << "Является ли рептилия ядовитой (1/0)?: ";
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
	if (animalsCount == 0) {
		cout << "Список животных пуст, нечего удалять" << endl;
		return;
	}
	else if (animalsCount > 1) {
		cout << "Выберите животное для удаления:" << endl;
		ListAnimals<Animal>();
		cout << "Ваш выбор: ";
		cin >> index;
	} // otherwise, there is only one animal (with index [0]) to delete
	if (index < animalsCount) {
		Animal* animal = animals[index];

		animalsCount--;
		for (size_t i = index; i < animalsCount; i++) {
			animals[i] = animals[i + 1];
		}

		/*cout << "Region::dtor(" << animal << "):";
		for (size_t i = 0; i < animalsCount; i++)
			cout << " " << animals[i];
		cout << endl;*/

		delete animal;
	}
}

void TestData();

int main()
{
	setlocale(LC_ALL, "RUS");
	//system("chcp 1251");
	//SetConsoleCP(1251);
	//SetConsoleOutputCP(1251);

	//TestData(); // for debug

	int counter = 0;
	int menu = -1;
	while (menu != 0) {
		cout << endl << endl << "Main menu:" << endl;
		cout << "[0] Выход" << endl;
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
		cout << "[11] Создать тестовую иерархию объектов" << endl;

		cout << endl;

		cout << "Ваш выбор: ";
		cin.sync();
		cin >> menu;

		switch (menu) {
		case 1:
			ListRegions<Region>(false, true);
			break;
		case 2:
			ListAnimals<Animal>(true);
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
			TestData();
			break;
		}

		// защита от непредвиденных ошибок ввода
		if (counter++ == 10) {
			cout << "emergency exit" << endl;
			break;
		}
		//break; // for debug
	}

	for (size_t i = 0; i < regionsCount; i++) {
		delete regions[i];
	}
	for (size_t i = 0; i < animalsCount; i++) {
		delete animals[i];
	}

	cout << "Работа завершена" << endl;
	return 0;
}

// Создать тестовую иерархию объектов
void TestData() {
	RegisterRegion(new Country("Россия")); // [0]
	RegisterRegion(new Country("Китай")); // [1]

	AddProvince(0, "Самарская область"); // [2]
	AddProvince(0, "Астраханская область"); // [3]
	AddProvince(0, "Хабаровская область"); // [4]
	AddProvince(0, "Петрозаводская область"); // [5]
	AddProvince(1, "Провинция Сычуань"); // [6]
	AddProvince(1, "Провинция Хайнань"); // [7]
	AddProvince(1, "Провинция Шаньдун"); // [8]
	AddProvince(1, "Провинция Ляонин"); // [9]

	AddMammal(2, "Лось", 145.0); // [0]
	AddMammal(4, "Лось", 208.0); // [1]
	AddMammal(6, "Лось", 181.0); // [2]
	AddMammal(2, "Бурый медведь", 287.0); // [3]
	AddMammal(3, "Бурый медведь", 225.0); // [4]
	AddMammal(4, "Бурый медведь", 344.0); // [5]
	AddMammal(5, "Бурый медведь", 250.0); // [6]
	AddMammal(2, "Ондатра", 25.0); // [7]
	AddMammal(8, "Ондатра", 28.0); // [8]
	AddMammal(6, "Панда", 65.0); // [9]
	AddMammal(7, "Панда", 57.0); // [10]
	AddMammal(8, "Панда", 68.0); // [11]
	AddMammal(9, "Панда", 61.0); // [12]
	AddReptile(2, "Обыкновенная гадюка", 0.23, true); // [13]
	AddReptile(3, "Обыкновенная гадюка", 0.51, true); // [14]
	AddReptile(4, "Обыкновенная гадюка", 0.5, true); // [15]
	AddReptile(5, "Обыкновенная гадюка", 0.43, true); // [16]
	AddReptile(6, "Обыкновенная гадюка", 0.38, true); // [17]
	AddReptile(9, "Обыкновенная гадюка", 0.37, true); // [18]
	AddReptile(4, "Дальневосточная черепаха", 5.5, false); // [19]
	AddReptile(6, "Дальневосточная черепаха", 6.1, false); // [20]
	AddReptile(7, "Дальневосточная черепаха", 5.8, false); // [21]
	AddReptile(8, "Китайский аллигатор", 287.0, false); // [22]
	AddReptile(9, "Китайский аллигатор", 302.0, false); // [23]
	AddReptile(7, "Эфа", 0.65, true); // [24]
	AddReptile(3, "Гюрза", 1.15, true); // [25]
}
