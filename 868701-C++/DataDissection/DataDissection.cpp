#include "stdafx.h"

#include <iostream>       // std::cin, std::cout
#include <sstream>        // std::ostringstream
#include <string>         // std::string
#include <vector>         // std::vector
#include <list>           // std::list
#include <fstream>        // std::ifstream, std::ofstream
#include <algorithm>      // std::random_shuffle

using namespace std;
// задание
static const size_t blockCount = 15; // количество блоков
static const size_t colCount = 5; // количество столбцов (длина ключа по столбцам)
static const size_t rowCount = blockCount / colCount; // длина ключа по строкам

typedef vector<uint8_t> data_t;
typedef list<uint8_t> block_t;
typedef vector<block_t> blocks_t;
typedef vector<size_t> key_t;

blocks_t encode(const data_t& data, const key_t& colKey, const key_t& rowKey) {
	blocks_t blocks(blockCount);

	for (size_t i = 0; i < data.size(); i++) {
		size_t row = i / colCount;
		size_t col = i % colCount;
		size_t k = colCount * rowKey[row % rowCount] + colKey[col];
		//cout << "data[" << i << "]: '" << data[i] << "' row " << row << " col " << col 
		//	<< " (" << rowKey[row % rowCount] << " " << colKey[col] << ")" << " k " << k << endl;
		blocks[k].push_back(data[i]);
	}

	return blocks;
}

void encode(const string& rawFileName, const string& encFileName, const key_t& colKey, const key_t& rowKey) {
	string text;

	size_t length = 0;
	char* lpBuf = 0;
	ifstream input(rawFileName);
	if (!input) {
		cerr << "не удалось открыть файл \"" << rawFileName << "\"" << endl;
		return;
	}
	input.seekg(0, ios::end);
	length = input.tellg();
	//cout << "length = " << length << endl;
	input.seekg(0, ios::beg);

	lpBuf = new char[length + 1];
	input.read(lpBuf, length);
	lpBuf[length] = '\0';
	text = lpBuf;

	cout << "\nтекст:\n" << text << endl << endl;

	data_t data(text.begin(), text.end());

	blocks_t blocks = encode(data, colKey, rowKey);
	
	ofstream output(encFileName);
	if (!output) {
		cerr << "не удалось открыть файл \"" << encFileName << "\"" << endl;
		return;
	}

	cout << "Сформированные блоки:" << endl;
	for (const auto& block : blocks) {
		cout << string(block.begin(), block.end()) << endl;
		for (const auto& byte : block) {
			output.put(byte);
		}
		output << endl;
	}
}

data_t decode(blocks_t& blocks, const key_t& colKey, const key_t& rowKey) {
	size_t size = 0;
	for (const auto& block : blocks)
		size += block.size();
	cout << "Ожидаемый размер данных: " << size << endl;
	
	data_t data(size, 0);
	for (size_t i = 0; i < data.size(); i++) {
		size_t row = i / colCount;
		size_t col = i % colCount;
		size_t k = colCount * rowKey[row % rowCount] + colKey[col];
		//cout << "data[" << i << "]: '" << data[i] << "' row " << row << " col " << col 
		//	<< " (" << rowKey[row % rowCount] << " " << colKey[col] << ")" << " k " << k << endl;
		if (!blocks[k].empty()) {
			data[i] = blocks[k].front();
			blocks[k].pop_front();
		}
	}
	return data;
}

void decode(const string& rawFileName, const string& encFileName, const key_t& colKey, const key_t& rowKey) {
	ifstream encFile(encFileName);
	if (!encFile) {
		cerr << "Не удалось открыть файл \"" << encFileName << "\"" << endl;
		return;
	}

	blocks_t blocks;
	string line;
	while (getline(encFile, line)) {
		cout << line << endl;
		block_t block;
		for (const auto byte : line)
			block.push_back(byte);
		blocks.push_back(block);
	}
	
	data_t data = decode(blocks, colKey, rowKey);
	string text = string(data.begin(), data.end());
	cout << "\nтекст:\n" << text << endl << endl;

	ofstream rawFile(rawFileName);
	if (!rawFile) {
		cerr << "Не удалось открыть файл \"" << rawFileName << "\"" << endl;
		return;
	}

	rawFile << text;
}

string keyStr(const key_t& key, size_t count) {
	ostringstream os;
	for (size_t i = 0; i < count; ++i)
		os << " " << key[i];
	return os.str();
}

void keyReq(key_t& key, size_t count, const string& prompt) {
	cout << prompt;
	for (size_t i = 0; i < count; ++i) {
		cin >> key[i];
	}
}

int main()
{
	setlocale(LC_ALL, "Russian");

	string rawFileName = "raw.txt";
	string encFileName = "encoded.txt";

	// ключи по столбцам и строкам:
	// проинициализированы по методичке, только для отладки,
	// потом перемешиваются случайным образом в std::random_shuffle()
	key_t colKey { 3, 0, 2, 1, 4 };
	key_t rowKey { 1, 0, 3, 2, 6, 5, 7, 6 };
#if 1
	for (int i = 0; i < colCount; ++i)
		colKey[i] = i;
	random_shuffle(colKey.begin(), colKey.begin() + colCount);
	for (int i = 0; i < rowCount; ++i)
		rowKey[i] = i;
	random_shuffle(rowKey.begin(), rowKey.begin() + rowCount);
#endif
	cout << "Параметры алгоритма: blockCount " << blockCount
		<< " colCount " << colCount << " rowCount " << rowCount;
	cout << " colKey " << keyStr(colKey, colCount) << " rowKey " << keyStr(rowKey, rowCount) << endl;

#if 1
	int menu = -1;
	while (menu != 0) {
		cout << endl << endl << "Выберите действие:" << endl;
		cout << "[0] Завершение работы" << endl;
		cout << "[1] Указать файл с исходными данными ( " << rawFileName << " )" << endl;
		cout << "[2] Указать файл для записи результата ( " << encFileName << " )" << endl;
		cout << "[3] Указать ключ по строкам (" << keyStr(rowKey, rowCount) << " )" << endl;
		cout << "[4] Указать ключ по столбцам (" << keyStr(colKey, colCount) << " )" << endl;
		cout << "[5] Закодировать данные" << endl;
		cout << "[6] Раскодировать данные" << endl;

		cout << endl;

		cout << "Ваш выбор: ";
		cin.sync();
		cin >> menu;

		blocks_t blocks;

		switch (menu) {
		case 1:
			cout << "Введите имя файла с исходными данными: ";
			cin >> rawFileName;
			break;
		case 2:
			cout << "Введите имя файла для записи результата: ";
			cin >> encFileName;
			break;
		case 3:
			keyReq(rowKey, rowCount, "Введите ключ по строкам: ");
			break;
		case 4:
			keyReq(colKey, colCount, "Введите ключ по столбцам: ");
			break;
		case 5:
			encode(rawFileName, encFileName, colKey, rowKey);
			break;
		case 6:
			decode(rawFileName, encFileName, colKey, rowKey);
			break;
		default:
			menu = 0;
		}
	}
#endif
	
	return 0;
}
