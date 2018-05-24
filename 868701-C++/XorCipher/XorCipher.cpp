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
size_t inputCount(const string& prompt) {
	size_t result = 0;
	cout << prompt;
	cin >> result;
	return result;
}
/*
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
}*/

// Запись параметров ГПСЧ в файл
void writeParams(const generator_t& gen, const string& fileName) {
	ofstream output(fileName);
	if (!output) {
		cerr << "Не удалось открыть файл \"" << fileName << "\"" << endl;
		return;
	}
	output << gen.A() << " " << gen.B() << " " << gen.Seed();
}

// Чтение параметров ГПСЧ из файла
void readParams(generator_t& gen, const string& fileName) {
	ifstream input(fileName);
	if (!input) {
		cerr << "Не удалось открыть файл \"" << fileName << "\"" << endl;
		return;
	}
	uint32_t a = 0, b = 0, c0 = 0;
	input >> a >> b >> c0;
	gen.SetParams(a, b, c0);
}

// Гаммирование данных в файле
void processFile(generator_t& gen, const string& dataFileName, const string& keyFileName) {
	// читаем параметры ГПСЧ из файла
	readParams(gen, keyFileName);
	// читаем исходные данные из файла
	size_t size = 0;
	char* lpBuf = nullptr;
	{
		ifstream input(dataFileName, ios::binary);
		if (!input) {
			cerr << "не удалось открыть файл \"" << dataFileName << "\" на чтение" << endl;
			return;
		}
		input.seekg(0, ios::end);
		size = static_cast<size_t>(input.tellg());
		cout << "data size " << size << endl;
		input.seekg(0, ios::beg);
		if (!size) {
			cerr << "файл \"" << dataFileName << "\" пуст" << endl;
			return;
		}
		lpBuf = new char[size];
		input.read(lpBuf, size);
	}
	// генерируем псевдослучайную последовательность нужной длины для гаммирования
	// размер данных в байтах (8 бит), а длина ПСП в numbers_t (32 бита)
	generator_t::numbers_t gamma = gen.NextNumbers((size - 1) / sizeof(generator_t::number_t) + 1, true);
	cout << "gamma size " << gamma.size() << ":" << uppercase << setfill('0');
	for (auto word : gamma)
		cout << " " << hex << setw(8) << word;
	cout << dec << endl;
	for (uint32_t i = 0; i < size; ++i) { // цикл по каждому байту исходных данных
		// выбеляем соответствующий байт из псевдослучайной последовательности
		//char g = gamma[i / sizeof(generator_t::number_t)] >> (i << 3);
		lpBuf[i] ^= gamma[i / sizeof(generator_t::number_t)] >> (i << 3);
		//cout << "data " << hex << setw(2) << int(lpBuf[i]) << " gamma " << setw(2) << int(g) << ": " << setw(2) << int(g ^ lpBuf[i]) << endl;
	}
	// записываем данные в тот же файл
	ofstream output(dataFileName, ios::binary);
	if (!output) {
		cerr << "Не удалось открыть файл \"" << dataFileName << "\" на запись" << endl;
		return;
	}
	output.write(lpBuf, size);

	delete[] lpBuf;
	lpBuf = nullptr;
}

int main()
{
	setlocale(LC_ALL, "Russian");

	// объект генератора
	generator_t gen(19997, 55529, 347675);
	// название файла с данными для гаммирования
	string dataFileName = "data.txt";
	// название файла для хранения параметров ГПСЧ
	string keyFileName = "params.key";

	int menu = -1;
	while (menu != 0) {
		cout << endl << endl;
		cout << "-----------------------------------------------------------\n";
		cout << "Параметры алгоритма: A " << gen.A() << " B " << gen.B() << " C0 " << gen.Seed() << endl;
		cout << "-----------------------------------------------------------\n";
		cout << "Выберите действие:" << endl;
		cout << "[0] Завершение работы" << endl;
		cout << "[1] Указать файл данных ( " << dataFileName << " )" << endl;
		cout << "[2] Указать файл параметров ГПСЧ ( " << keyFileName << " )" << endl;
		cout << "[3] Сгенерировать параметры ГПСЧ" << endl;
		cout << "[4] Сохранить параметры ГПСЧ в файл" << endl;
		cout << "[5] Выполнить гаммирование данных в файле" << endl;
		cout << "\nВаш выбор: ";
		cin.sync();
		cin >> menu;
		cout << endl;

		switch (menu) {
		case 1:
			cout << "Введите имя файла данных: ";
			cin >> dataFileName;
			break;
		case 2:
			cout << "Введите имя файла параметров: ";
			cin >> keyFileName;
			break;
		case 3:
			gen.createParams();
			cout << "Сформированы параметры: A " << gen.A() << " B " << gen.B() << " C0 " << gen.Seed() << endl;
			break;
		case 4:
			writeParams(gen, keyFileName);
			break;
		case 5:
			processFile(gen, dataFileName, keyFileName);
			break;
		default:
			menu = 0;
		}
	}

	return 0;
}
