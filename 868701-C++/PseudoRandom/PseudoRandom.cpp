#include "stdafx.h"

#include <iostream>       // std::cin, std::cout
#include <iomanip>        // std::setprecision
#include <sstream>        // std::ostringstream
#include <string>         // std::string
#include <vector>         // std::vector
#include <fstream>        // std::ofstream
#include <chrono>         // std::chrono::system_clock

#include "../PseudoRandomNumberGenerator.h"

using namespace std;

template<class Sequence>
void printSequence(const Sequence& seq, const string& title = "") {
	cout << title;
	for (const auto& item : seq)
		cout << " " << item;
	cout << endl;
}

// ����� ��� (�������� �������� �� ������ ������� ����������)
int gcd(int a, int b) {
	cout << " gcd " << a << ", " << b;
	int shift;
	if (a == 0)
		return b;
	if (b == 0)
		return a;
	for (shift = 0; ((a | b) & 1) == 0; ++shift) {
		a >>= 1;
		b >>= 1;
	}
	while ((a & 1) == 0)
		a >>= 1;
	do {
		while ((b & 1) == 0)
			b >>= 1;
		if (a > b) {
			int t = b;
			b = a;
			a = t;
		}
		b = b - a;
	} while (b != 0);
	cout << ": " << (a << shift) << endl;
	return a << shift;
}

// ���� ���������� ��������� � ����������
void inputParams(PseudoRandomNumberGen& gen) {
	uint32_t a, b, c0;
	cout << "������� �������� A: ";
	cin >> a;
	cout << "������� �������� B: ";
	cin >> b;
	cout << "������� �������� C0: ";
	cin >> c0;

	gen.SetParams(a, b, c0);
}

// ������������ � ��������� ��������� ���������� ���������
void createParams(PseudoRandomNumberGen& gen) {
	const size_t m = gen.M();

	// b: �������� ��������� 32-������ ����� �� ������ �������� �������
	uint32_t b = static_cast<uint32_t>(chrono::system_clock::now().time_since_epoch().count()) & 0xFFFF;
	//cout << "b " << b;
	// ������������ ���������� b
	b |= 1; 
	//cout << " -> " << b << endl;
	// ���������� �������� �����, ������� b, ���� �� ����� ������� ������� � m
	while (gcd(b, m) != 1)
		b += 2;
	//cout << "b " << b << endl;
	// a: �������� ��������� 32-������ ����� �� ������ �������� �������
	uint32_t a = static_cast<uint32_t>(chrono::system_clock::now().time_since_epoch().count()) & 0xFFFF;
	//cout << "a " << a;
	while ((a & 3) != 1)
		a++;
	//cout << " -> " << a << endl;
	// c0: �������� ��������� 32-������ ����� �� ������ �������� �������
	uint32_t c0 = static_cast<uint32_t>(chrono::system_clock::now().time_since_epoch().count()) % m;

	cout << "������������ ���������: A " << a << " B " << b << " C0 " << c0 << endl;

	gen.SetParams(a, b, c0);
}

// ���� ����� � ����������
uint32_t inputCount(const string& prompt) {
	size_t result = 0;
	cout << prompt;
	cin >> result;
	return result;
}

// ���������� �� 2 ������ ����� �������
double round2(double value) {
	return size_t(value * 100.0 + 0.5) / 100.0;
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

void generateNumbers(PseudoRandomNumberGen& gen, const string& fileName) {
	size_t count = inputCount("������� ������ ��������������� ������������������: ");
	if (count) {
		writeNumbers(gen.M(), gen.NextNumbers(count, true), fileName);
	}
}

int main()
{
	setlocale(LC_ALL, "Russian");

	// ������ ����������
	PseudoRandomNumberGen gen(24, 19997, 55529, 347675);
	// �������� ����� ��� ������ ��������������� ������������������
	string fileName = "output.txt";

	int menu = -1;
	while (menu != 0) {
		cout << endl << endl;
		cout << "-----------------------------------------------------------\n";
		cout << "��������� ���������: A " << gen.A() << " B " << gen.B() << " C0 " << gen.Base() << endl;
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
			createParams(gen);
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
