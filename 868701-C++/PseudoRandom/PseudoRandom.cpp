#include "stdafx.h"

#include <iostream>       // std::cin, std::cout
#include <iomanip>        // std::setprecision
#include <sstream>        // std::ostringstream
#include <string>         // std::string
#include <vector>         // std::vector
#include <fstream>        // std::ofstream
#include <chrono>         // std::chrono::system_clock

#include "../TaskLog.h"
#include "../PseudoRandomNumberGenerator.h"

using namespace std;

typedef PseudoRandomNumberGenerator<24> generator_t;

// Ввод параметров алгоритма с клавиатуры
void inputParams(generator_t& gen) {
	uint32_t a, b, c0;
	cout << "Введите значение A: ";
	cin >> a;
	cout << "Введите значение B: ";
	cin >> b;
	cout << "Введите значение C0: ";
	cin >> c0;

	gen.SetParams(a, b, c0);
}

// Ввод числа с клавиатуры
uint32_t inputCount(const string& prompt) {
	uint32_t result = 0;
	cout << prompt;
	cin >> result;
	return result;
}

// Вывод последовательности чисел на экран (со статистикой) и в файл
void writeNumbers(size_t m, const vector<uint32_t>& numbers, const string& fileName) {
	if (numbers.empty())
		return;

	if (numbers.size() <= 15) // пожалеем консоль (результат увидим в файле)
		printSequence(numbers, "ПСП:");

	vector<double> stats(100, 0);
	const size_t intervalLength = size_t(double(m) / double(stats.size()) + 0.5);
	// Подсчёт и вывод статистики
	for (auto number : numbers)
		stats[number / intervalLength] += 1;
	for (size_t i = 0; i < 100; i++)
		if (stats[i] > 0.0)
			cout << "[" << i << "]\t" << fixed << setprecision(2) << stats[i] * 100.0 / numbers.size() << "%" << endl;

	// Запись в файл
	ofstream output(fileName);
	if (!output) {
		cerr << "Не удалось открыть файл \"" << fileName << "\"" << endl;
		return;
	}

	for (auto number : numbers)
		output << number << "\n";

	cerr << "Последовательность записана в файл \"" << fileName << "\"" << endl;
}

void generateNumbers(generator_t& gen, const string& fileName) {
	size_t count = inputCount("Введите размер псевдослучайной последовательности: ");
	if (count) {
		writeNumbers(gen.M(), gen.NextNumbers(count, true), fileName);
	}
}

int main()
{
	setlocale(LC_ALL, "Russian");

	// объект генератора
	generator_t gen(19997, 55529, 347675);
	// название файла для записи псевдослучайной последовательности
	string fileName = "output.txt";

	int menu = -1;
	while (menu != 0) {
		cout << endl << endl;
		cout << "-----------------------------------------------------------\n";
		cout << "Параметры алгоритма: A " << gen.A() << " B " << gen.B() << " C0 " << gen.Seed() << endl;
		cout << "-----------------------------------------------------------\n";
		cout << "Выберите действие:" << endl;
		cout << "[0] Завершение работы" << endl;
		cout << "[1] Ввести параметры ГПСЧ" << endl;
		cout << "[2] Сгенерировать параметры ГПСЧ" << endl;
		cout << "[3] Сгенерировать следующее псевдослучайное число" << endl;
		cout << "[4] Сгенерировать последовательность псевдослучайных чисел" << endl;
		cout << "[5] Указать файл для записи ПСП ( " << fileName << " )" << endl;
		cout << "\nВаш выбор: ";
		cin.sync();
		cin >> menu;
		cout << endl;

		switch (menu) {
		case 1:
			inputParams(gen);
			break;
		case 2:
			gen.createParams();
			cout << "Сформированы параметры: A " << gen.A() << " B " << gen.B() << " C0 " << gen.Seed() << endl;
			break;
		case 3:
			cout << "ПСЧ: " << gen.NextNumber() << endl;
			break;
		case 4:
			generateNumbers(gen, fileName);
			break;
		case 5:
			cout << "Введите имя файла для записи ПСП: ";
			cin >> fileName;
			break;
		default:
			menu = 0;
		}
	}

	return 0;
}
