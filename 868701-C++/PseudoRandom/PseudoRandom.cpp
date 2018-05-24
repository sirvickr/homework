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
uint32_t inputCount(const string& prompt) {
	uint32_t result = 0;
	cout << prompt;
	cin >> result;
	return result;
}

// ����� ������������������ ����� �� ����� (�� �����������) � � ����
void writeNumbers(size_t m, const vector<uint32_t>& numbers, const string& fileName) {
	if (numbers.empty())
		return;

	if (numbers.size() <= 15) // �������� ������� (��������� ������ � �����)
		printSequence(numbers, "���:");

	vector<double> stats(100, 0);
	const size_t intervalLength = size_t(double(m) / double(stats.size()) + 0.5);
	// ������� � ����� ����������
	for (auto number : numbers)
		stats[number / intervalLength] += 1;
	for (size_t i = 0; i < 100; i++)
		if (stats[i] > 0.0)
			cout << "[" << i << "]\t" << fixed << setprecision(2) << stats[i] * 100.0 / numbers.size() << "%" << endl;

	// ������ � ����
	ofstream output(fileName);
	if (!output) {
		cerr << "�� ������� ������� ���� \"" << fileName << "\"" << endl;
		return;
	}

	for (auto number : numbers)
		output << number << "\n";

	cerr << "������������������ �������� � ���� \"" << fileName << "\"" << endl;
}

void generateNumbers(generator_t& gen, const string& fileName) {
	size_t count = inputCount("������� ������ ��������������� ������������������: ");
	if (count) {
		writeNumbers(gen.M(), gen.NextNumbers(count, true), fileName);
	}
}

int main()
{
	setlocale(LC_ALL, "Russian");

	// ������ ����������
	generator_t gen(19997, 55529, 347675);
	// �������� ����� ��� ������ ��������������� ������������������
	string fileName = "output.txt";

	int menu = -1;
	while (menu != 0) {
		cout << endl << endl;
		cout << "-----------------------------------------------------------\n";
		cout << "��������� ���������: A " << gen.A() << " B " << gen.B() << " C0 " << gen.Seed() << endl;
		cout << "-----------------------------------------------------------\n";
		cout << "�������� ��������:" << endl;
		cout << "[0] ���������� ������" << endl;
		cout << "[1] ������ ��������� ����" << endl;
		cout << "[2] ������������� ��������� ����" << endl;
		cout << "[3] ������������� ��������� ��������������� �����" << endl;
		cout << "[4] ������������� ������������������ ��������������� �����" << endl;
		cout << "[5] ������� ���� ��� ������ ��� ( " << fileName << " )" << endl;
		cout << "\n��� �����: ";
		cin.sync();
		cin >> menu;
		cout << endl;

		switch (menu) {
		case 1:
			inputParams(gen);
			break;
		case 2:
			gen.createParams();
			cout << "������������ ���������: A " << gen.A() << " B " << gen.B() << " C0 " << gen.Seed() << endl;
			break;
		case 3:
			cout << "���: " << gen.NextNumber() << endl;
			break;
		case 4:
			generateNumbers(gen, fileName);
			break;
		case 5:
			cout << "������� ��� ����� ��� ������ ���: ";
			cin >> fileName;
			break;
		default:
			menu = 0;
		}
	}

	return 0;
}
