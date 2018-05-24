#include "stdafx.h"

#include <iostream>       // std::cin, std::cout
#include <sstream>        // std::ostringstream
#include <string>         // std::string
#include <vector>         // std::vector

#include "../TaskLog.h"

using namespace std;

typedef vector<size_t> bits_t;

template<class Sequence>
string seqStr(const Sequence& seq) {
	ostringstream os;
	for (const auto& item : seq)
		os << " " << item;
	return os.str();
}

bool inputBits(bits_t& bits, const string& prompt) {
	bool result = true;
	cout << prompt;
	bits_t v(bits.size(), 0);
	for (auto& item : v) {
		cin >> item;
		if (item != 0 && item != 1) {
			result = false;
			break;
		}
	}
	if (result)
		bits = v;
	return result;
}

// ������� ����������� ����� 
size_t checkBits(const bits_t& a, const bits_t& bt, size_t m, size_t N) {
	size_t result = 0;
	for (size_t i = 0; i < m; i++) {
		size_t bit = 0;
		for (size_t j = 0; j < N; j++)
			bit += a[i * N + j] * bt[j];
		bit &= 1;
		result |= (bit << i);
	}
	return result;
}

// ������������ ������� ������������ ������������
bits_t makeChecksumMatrix(size_t m, size_t N) {
	bits_t a(m * N);
	for (size_t i = 0; i < m; i++)
		for (size_t j = 0; j < N; j++)
			a[i * N + j] = ((j + 1) >> i) & 1;
	return a;
}

// ����������� �������� �������� ������������������ �� ������ ��������
void encode(const bits_t& b, bits_t& bt, size_t m, size_t N) {
	cout << "����������� �����������..." << endl;
	// ��������� ������� ������������ ������������
	auto a = makeChecksumMatrix(m, N);
	printMatrix(a, m, N, "������� ������������ ������������ (A):\n");
	// ��������� ������������ ������, ���� � �������� ������������ ������
	for (size_t k = 0, j = 0; j < N; j++) {
		size_t v = j + 1;
		if (!(v && !(v & (v - 1)))) { // ��������, �� �������� �� ����� �������� ������
			bt[j] = b[k++];
		}
	}
	// ������������ ����������� ����
	size_t S = checkBits(a, bt, m, N);
	cout << "S: " << uppercase << hex << S << dec << endl;
	// ���������� ����������� ���� � ��������������� ������� (������� ������) ������������� �������
	for (size_t i = 0; i < m; i++) {
		// ���������� i-� ����������� ��� � ((2 � ������� i) - 1)-� ������ 
		// ������������ ������������������ (����������, �������, �� ����)
		bt[(1 << i) - 1] = (S >> i) & 1;
	}
	printSequence(bt, "������������ ������� ������������������ (Bt):");
}

// �������� ��������� ��������� ���� ��������
void check(bits_t& bt, size_t m, size_t N) {
	cout << "����������� ��������..." << endl;
	printSequence(bt, "�������� ������� ������������������ (Bt):");
	// ��������� ������� ������������ ������������
	auto a = makeChecksumMatrix(m, N);
	printMatrix(a, m, N, "������� ������������ ������������ (A):\n");
	// ��������� S
	size_t S = checkBits(a, bt, m, N);
	cout << "S: " << uppercase << hex << S << dec << endl;
	if (S) {
		cout << "� " << S << "� ������� ���������� ������" << endl;
	}
	else {
		cout << "������ �� ����������" << endl;
	}
}

// ���������� ������������ ���������� ����������� ��� 
// ��� �������� ����� ���������� ������������������
size_t minCheckBitsCount(size_t n) {
	size_t m;
	for (m = 1; m < 32; m++)
		if ((size_t(1) << m) >= (n + m + 1))
			break;
	return m;
}

int main()
{
	setlocale(LC_ALL, "Russian");

	// �������
	const size_t n = 10;

	// ���������� ���������� ���������
	size_t m = minCheckBitsCount(n);
	size_t N = n + m;
	// ������������������ �� ��������� (�� ������������� �������)
	const bits_t defaultBits{ 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0 };
	// �������� �������� ������������������
	bits_t b(defaultBits.begin(), defaultBits.begin() + n);
	// ������������ �������� ������������������
	bits_t bt(N, 0);

	int menu = -1;
	while (menu != 0) {
		cout << endl << endl;
		cout << "------------------------------------\n";
		cout << "��������� ���������: n " << n << " m " << m << " N " << N << endl;
		cout << "------------------------------------\n";
		cout << "�������� ��������:" << endl;
		cout << "[0] ���������� ������" << endl;
		cout << "[1] ������ �������� ������� ������������������ (" << seqStr(b) << " )" << endl;
		cout << "[2] ������ ������������ ������������������ (" << seqStr(bt) << " )" << endl;
		cout << "[3] ��������� �����������" << endl;
		cout << "[4] ��������� ��������" << endl;
		cout << "\n��� �����: ";
		cin.sync();
		cin >> menu;
		cout << endl;

		switch (menu) {
		case 1:
			if (!inputBits(b, "������� �������� ������� ������������������: ")) {
				cerr << "������� ������������ ��������" << endl;
			}
			break;
		case 2:
			if (!inputBits(bt, "������� ������������ ������������������: ")) {
				cerr << "������� ������������ ��������" << endl;
			}
			break;
		case 3:
			encode(b, bt, m, N);
			break;
		case 4:
			check(bt, m, N);
			break;
		default:
			menu = 0;
		}
	}

	return 0;
}
