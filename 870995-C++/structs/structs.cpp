/* 1. Создайте текстовый файл, содержащий сведения о сотрудниках института: 
фамилия работающего, название отдела, год рождения, стаж работы, должность, оклад.
Напишите программу, которая формирует двоичный файл, каждый элемент которого 
является структурой, составленной на основе данных текстового файла.
Отсортируйте данные бинарного файла в порядке возрастания года рождения.
Выведите на экран информацию о сотрудниках, чей оклад не превышает X.
*/

#include <iomanip>
#include <iostream>
#include <fstream>

using namespace std;

// структура данных сотрудника
struct Employee {
	char surname[15];    // фамилия работающего
	char department[15]; // название отдела
	int year;       // год рождения
	int experience;  // стаж работы
	char post[15];       // должность
	int salary;     // оклад
};

// вывод данных сотрудника на экран
ostream& operator<<(ostream& stream, const Employee& e) {
	stream << left << setw(15) << e.surname
		<< setw(15) << e.department
		<< setw(5) << e.year << setw(3)
		<< e.experience << setw(25)
		<< e.post << setw(7) << e.salary;
	return stream;
}

// копирование данных из текстового файла в бинарный
size_t copyTxtToBin(const char* inFileName, const char* outFileName) {
	Employee e;
	memset(&e, 0, sizeof(e));
	size_t count = 0;

	ifstream in(inFileName);
	if (!in) {
		cerr << "не удалось открыть файл \"" << inFileName << "\"" << endl;
		return 1;
	}

	ofstream out(outFileName, ios::binary);
	if (!out) {
		cerr << "не удалось открыть файл \"" << outFileName << "\"" << endl;
		return 1;
	}

	while (!in.eof()) {
		in >> e.surname >> e.department >> e.year >> e.experience >> e.post >> e.salary;
		out.write((char*)&e, sizeof(e));
		count++;
	}

	return count;
}

// сортировка файла с данными сотрудников:
// для сравнения очередных двух сотрудников вызывает переданную ей функцию "Меньше"
template<class Less>
void sort(const char* fileName, size_t count, Less less) {
	fstream file(fileName, ios::in | ios::out | ios::binary);
	const size_t size = sizeof(Employee);
	Employee min, curr;
	size_t index;
	for (size_t i = 0; i < count; i++) {
		file.seekg(i * size);
		file.read((char*)&min, size);
		index = i;
		for (size_t j = i + 1; j < count; j++) {
			file.read((char*)&curr, size);
			if(less(curr, min)) {
				min = curr;
				index = j;
			}
		}
		file.seekg(i * size);
		file.read((char*)&curr, size);
		file.seekp(i * size);
		file.write((char*)&min, size);
		file.seekp(index * size);
		file.write((char*)&curr, size);
	}
}

// Вывод из файла на экран сотрудников, удовлетворяющих условиям функции filter()
template<class Filter>
void printFilteredStaff(const char* fileName, size_t count, Filter filter) {
	ifstream file(fileName, ios::binary);
	const size_t size = sizeof(Employee);
	Employee e;
	for (size_t i = 0; i < count; i++) {
		file.seekg(i * size);
		file.read((char*)&e, size);
		if (filter(e)) {
			cout << e << endl;
		}
	}
}

int main()
{
	setlocale(LC_CTYPE, "Russian");

	// название входного текстового файла
	const char* inFileName = "data.txt";
	// название выходного двоичного файла
	const char* outFileName = "data.bin";
	
	// переносим данные из текстового файла в двоичный
	size_t count = copyTxtToBin(inFileName, outFileName);

	// сортировка файла с данными сотрудников:
	// для этого определяем функцию, сравнивающую
	// годы рождения переданного ей сотрудников
	// одноразовая безымянная функция ("лямбда") определяется прямо по месту её использования
	sort(outFileName, count, [](const Employee& a, const Employee& b) { return a.year < b.year; });

	// ввод максимального оклада для отбора сотрудников
	int X = 0;
	cout << "Введите максимальный оклад: ";
	cin >> X;

	// вывод на экран сотрудников, чей оклад не превышает X:
	// для этого определяем функцию, возвращающую true, если
	// оклад очередного переданного ей сотрудника <= X
	// одноразовая безымянная функция ("лямбда") определяется прямо по месту её использования
	printFilteredStaff(outFileName, count, [X](const Employee& e) { return e.salary <= X; });

	return 0;
}
