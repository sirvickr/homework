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

// ���� ���������� ��������� � ����������
void inputParams(generator_t& gen) {
	uint32_t a, b, c0;
	cout << "������� �������� A: ";
	cin >> a;
	cout << "������� �������� B: ";
	cin >> b;
	cout << "������� �������� C0: ";
	cin >> c0;

	gen.SetParams(a, b, c0);
}

// ���� ����� � ����������
size_t inputCount(const string& prompt) {
	size_t result = 0;
	cout << prompt;
	cin >> result;
	return result;
}

// ������ ���������� ���� � ����
void writeParams(const generator_t& gen, const string& fileName) {
	ofstream output(fileName);
	if (!output) {
		cerr << "�� ������� ������� ���� \"" << fileName << "\"" << endl;
		return;
	}
	output << gen.A() << " " << gen.B() << " " << gen.Seed();
}

// ������ ���������� ���� �� �����
void readParams(generator_t& gen, const string& fileName) {
	ifstream input(fileName);
	if (!input) {
		cerr << "�� ������� ������� ���� \"" << fileName << "\"" << endl;
		return;
	}
	uint32_t a = 0, b = 0, c0 = 0;
	input >> a >> b >> c0;
	gen.SetParams(a, b, c0);
}

// ������������ ������ � �����
void processFile(generator_t& gen, const string& dataFileName, const string& keyFileName) {
	// ������ ��������� ���� �� �����
	readParams(gen, keyFileName);
	// ������ �������� ������ �� �����
	size_t size = 0;
	char* lpBuf = nullptr;
	{
		ifstream input(dataFileName, ios::binary);
		if (!input) {
			cerr << "�� ������� ������� ���� \"" << dataFileName << "\" �� ������" << endl;
			return;
		}
		input.seekg(0, ios::end);
		size = static_cast<size_t>(input.tellg());
		cout << "������ ����� ������: " << size << endl;
		input.seekg(0, ios::beg);
		if (!size) {
			cerr << "���� \"" << dataFileName << "\" ����" << endl;
			return;
		}
		lpBuf = new char[size];
		input.read(lpBuf, size);
	}

	using namespace std::chrono;
	// ��������� ������� �����
	auto start = chrono::steady_clock::now();

	// ���������� ��������������� ������������������ ������ ����� ��� ������������
	// ������ ������ � ������ (8 ���), � ����� ��� � numbers_t (32 ����)
	auto gamma = gen.NextNumbers((size - 1) / sizeof(generator_t::number_t) + 1, true);
	for (uint32_t i = 0; i < size; ++i) { // ���� �� ������� ����� �������� ������
		// �������� ��������������� ���� �� ��������������� ������������������
		lpBuf[i] ^= gamma[i / sizeof(generator_t::number_t)] >> (i << 3);
	}

	// ��������� ������� � ��������� ����������� �� ��������� �����
	auto finish = steady_clock::now();
	cout << "����� ������������: " << duration_cast<microseconds>(finish - start).count() << " ���" << endl;

	// ���������� ������ � ��� �� ����
	ofstream output(dataFileName, ios::binary);
	if (!output) {
		cerr << "�� ������� ������� ���� \"" << dataFileName << "\" �� ������" << endl;
		return;
	}
	output.write(lpBuf, size);

	delete[] lpBuf;
	lpBuf = nullptr;
}

int main()
{
	setlocale(LC_ALL, "Russian");

	// ������ ����������
	generator_t gen(19997, 55529, 347675);
	// �������� ����� � ������� ��� ������������
	string dataFileName = "1.txt";
	// �������� ����� ��� �������� ���������� ����
	string keyFileName = "params.key";

	int menu = -1;
	while (menu != 0) {
		cout << endl << endl;
		cout << "-----------------------------------------------------------\n";
		cout << "��������� ���������: A " << gen.A() << " B " << gen.B() << " C0 " << gen.Seed() << endl;
		cout << "-----------------------------------------------------------\n";
		cout << "�������� ��������:" << endl;
		cout << "[0] ���������� ������" << endl;
		cout << "[1] ������� ���� ������ ( " << dataFileName << " )" << endl;
		cout << "[2] ������� ���� ���������� ���� ( " << keyFileName << " )" << endl;
		cout << "[3] ������������� ����� ��������� ����" << endl;
		cout << "[4] ��������� ������������ ������ � �����" << endl;
		cout << "\n��� �����: ";
		cin.sync();
		cin >> menu;
		cout << endl;

		switch (menu) {
		case 1:
			cout << "������� ��� ����� ������: ";
			cin >> dataFileName;
			break;
		case 2:
			cout << "������� ��� ����� ����������: ";
			cin >> keyFileName;
			break;
		case 3:
			gen.createParams();
			cout << "������������ ���������: A " << gen.A() << " B " << gen.B() << " C0 " << gen.Seed() << endl;
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
