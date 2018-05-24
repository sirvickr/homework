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
			cerr << "Не удалось открыть файл \"" << dataFileName << "\" на чтение" << endl;
			return;
		}
		input.seekg(0, ios::end);
		size = static_cast<size_t>(input.tellg());
		cout << "Размер файла данных: " << size << endl;
		input.seekg(0, ios::beg);
		if (!size) {
			cerr << "Файл \"" << dataFileName << "\" пуст" << endl;
			return;
		}
		lpBuf = new char[size];
		input.read(lpBuf, size);
	}

	using namespace std::chrono;
	// сохраняем текущее время
	auto start = chrono::steady_clock::now();

	// генерируем псевдослучайную последовательность нужной длины для гаммирования
	// размер данных в байтах (8 бит), а длина ПСП в numbers_t (32 бита)
	auto gamma = gen.NextNumbers((size - 1) / sizeof(generator_t::number_t) + 1, true);
	for (uint32_t i = 0; i < size; ++i) { // цикл по каждому байту исходных данных
		// выделяем соответствующий байт из псевдослучайной последовательности
		lpBuf[i] ^= gamma[i / sizeof(generator_t::number_t)] >> (i << 3);
	}

	// сохраняем текущее и вычисляем затраченное на обработку время
	auto finish = steady_clock::now();
	cout << "Время гаммирования: " << duration_cast<microseconds>(finish - start).count() << " мкс" << endl;

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
	string dataFileName = "1.txt";
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
		cout << "[3] Сгенерировать новые параметры ГПСЧ" << endl;
		cout << "[4] Выполнить гаммирование данных в файле" << endl;
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
			writeParams(gen, keyFileName);
			break;
		case 4:
			processFile(gen, dataFileName, keyFileName);
			break;
		default:
			menu = 0;
		}
	}

	return 0;
}
