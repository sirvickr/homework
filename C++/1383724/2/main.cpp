#include "main.h"

using namespace std;

void showAll(const std::list<Monitor>& componentRange) {
    printList(cout, componentRange, [](const Monitor& mon){ return true; }, "Мониторы");
}

void printList(ostream& stream, const list<Monitor>& componentRange, function<bool(const Monitor& mon)> cond, const string& title) {
	if(title.size())
		stream << title << '\n';
	stream << "Фирма\tЦена\tДиагональ\n";
	for(const auto& mon: componentRange) {
		if(cond(mon)) {
			stream << " " << mon.manufacturer() << "\t" << mon.price() << "р\t" << mon.diag() << "\"\n";
		}
	}
}

void searchByPrice(const list<Monitor>& componentRange) {
	string subTitle = " не дороже ";
	string units = "руб.";
	cout << "Вывести мониторы" << subTitle << "(" << units << "): ";
	double value = 0.0;
	cin >> value;
    printList(cout, componentRange, [value](const Monitor& mon){ return mon.price() <= value; }, "Мониторы" + subTitle + to_string(value) + units);
}

void searchByDiag(const list<Monitor>& componentRange) {
	string subTitle = " с диагональю не меньше ";
	string units = "\"";
	cout << "Вывести мониторы" << subTitle << "(" << units << "): ";
	double value = 0.0;
	cin >> value;
    printList(cout, componentRange, [value](const Monitor& mon){ return mon.diag() >= value; }, "Мониторы" + subTitle + to_string(value) + units);
}

list<Monitor> loadData(const string& fileName) {
	list<Monitor> result;
	ifstream file(fileName);
	if(!file.is_open()) {
		cerr << "Не удалось открыть файл: \"" << fileName << "\"" << endl;
		return result;
	}
	string token;
	while(getline(file, token, ';')) {
	//	cout << token << ":";
		string item;
		string manufacturer;
		double price = 0.0;
		double diag = 0.0;
		istringstream iss(token);
		
		if(!getline(iss, item, '-')) {
			cerr << "Неверный формат файла - не удалось прочитать фирму-производителя" << endl;
			break;
		}
		manufacturer = item;
		//cout << " Фирма " << item;
		
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
		istringstream issDiag(item);
		issDiag >> diag;

		result.push_back(Monitor(diag, price, manufacturer));
	//	cout << endl;
	}

	file.close();
	return result;
}

int mainMenu(const list<Monitor>& componentRange) {
	if(componentRange.empty()) {
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
        cout << "[3] Поиск по диагонали" << endl;

        cout << "\nВведите Ваш выбор: ";
        cin >> menu;

        switch(menu) {
        case 1:
	        showAll(componentRange);
        	break;
        case 2:
        	searchByPrice(componentRange);
        	break;
        case 3:
        	searchByDiag(componentRange);
        	break;
        }
    }
    return 0;
};

int main() {
	return mainMenu(loadData("data.txt"));
}
