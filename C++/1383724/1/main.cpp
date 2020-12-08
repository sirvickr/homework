#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <functional>

using namespace std;

// товар
class Commodity
{
public:
	Commodity(double price = 0.0) : mPrice(price) {
	}
	double price() const {
		return mPrice;
	}
	void price(double value) {
		mPrice = value;
	}
private:
	// цена
	double mPrice;
};

// топливо
class Fuel : public Commodity
{
public:
	Fuel(double exhaust = 0.0, double price = 0.0) : Commodity(price), mExhaust(exhaust) {
	}
	double exhaust() const {
		return mExhaust;
	}
	void exhaust(double value) {
		mExhaust = value;
	}
private:
	// количество выбросов
	double mExhaust;
};

// класс, представляющий бензин
class Petrol : public Fuel
{
public:
	Petrol(const string& type, double exhaust = 0.0, double price = 0.0) : Fuel(exhaust, price), mType(type) {
	}
	const string& type() const {
		return mType;
	}
	void type(const string& value) {
		mType = value;
	}
private:
	// марка бензина
	string mType;
};

void printList(ostream& stream, const list<Petrol>& petrolRange, function<bool(const Petrol& fuel)> cond, const string& title = "") {
	if(title.size())
		stream << title << '\n';
	stream << "Марка\tЦена\tВыбросы\n";
	for(const auto& fuel: petrolRange) {
		if(cond(fuel)) {
			stream << " " << fuel.type() << "\t" << fuel.price() << "р\t" << fuel.exhaust() << "г/км\n";
		}
	}
}

void showAll(const list<Petrol>& petrolRange) {
    printList(cout, petrolRange, [](const Petrol& fuel){ return true; }, "Виды топлива");
}

void searchByPrice(const list<Petrol>& petrolRange) {
	string subTitle = " не дороже ";
	string units = "руб.";
	cout << "Вывести топливо" << subTitle << "(" << units << "): ";
	double value = 0.0;
	cin >> value;
    printList(cout, petrolRange, [value](const Petrol& fuel){ return fuel.price() <= value; }, "Виды топлива" + subTitle + to_string(value) + units);
}

void searchByExhaust(const list<Petrol>& petrolRange) {
	string subTitle = " с выбросом не больше ";
	string units = "г/км";
	cout << "Вывести топливо" << subTitle << "(" << units << "): ";
	double value = 0.0;
	cin >> value;
    printList(cout, petrolRange, [value](const Petrol& fuel){ return fuel.exhaust() <= value; }, "Виды топлива" + subTitle + to_string(value) + units);
}

list<Petrol> loadData(const string& fileName) {
	list<Petrol> result;
	ifstream file(fileName);
	if(!file.is_open()) {
		cerr << "Не удалось открыть файл: \"" << fileName << "\"" << endl;
		return result;
	}

	string token;
	while(getline(file, token, ';')) {
	//	cout << token << ":";
		string item;
		string type;
		double price = 0.0;
		double exhaust = 0.0;
		istringstream iss(token);
		
		if(!getline(iss, item, '-')) {
			cerr << "Неверный формат файла - не удалось прочитать марку" << endl;
			break;
		}
		type = item;
		//cout << " марка " << item;
		
		if(!getline(iss, item, '-')) {
			cerr << "Неверный формат файла - не удалось прочитать цену" << endl;
			break;
		}
		istringstream issPrice(item);
		issPrice >> price;
		//cout << " цена " << price;
		
		if(!getline(iss, item, '-')) {
			cerr << "Неверный формат файла - не удалось прочитать выброс" << endl;
			break;
		}
		istringstream issExhaust(item);
		issExhaust >> exhaust;

		result.push_back(Petrol(type, exhaust, price));
	//	cout << endl;
	}

	file.close();
	return result;
}

int main() {
	list<Petrol> petrolRange = loadData("data.txt");
	if(petrolRange.empty()) {
		cerr << "База данных пуста" << endl;
		return -1;
	}

	int counter = 0;
    int menu = -1;
    while (menu != 0) {
        cout << endl << endl << "Main menu:" << endl;
        cout << "[0] Выход" << endl;
        cout << "[1] Показать весь список" << endl;
        cout << "[2] Поиск по стоимости" << endl;
        cout << "[3] Поиск по количеству выбросов" << endl;

        cout << "\nВведите Ваш выбор: ";
        cin >> menu;

        switch(menu) {
        case 1:
	        showAll(petrolRange);
        	break;
        case 2:
        	searchByPrice(petrolRange);
        	break;
        case 3:
        	searchByExhaust(petrolRange);
        	break;
        }
    }

	return 0;
}
