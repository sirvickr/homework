/* 1. �������� ��������� ����, ���������� �������� � ����������� ���������: 
������� �����������, �������� ������, ��� ��������, ���� ������, ���������, �����.
�������� ���������, ������� ��������� �������� ����, ������ ������� �������� 
�������� ����������, ������������ �� ������ ������ ���������� �����.
������������ ������ ��������� ����� � ������� ����������� ���� ��������.
�������� �� ����� ���������� � �����������, ��� ����� �� ��������� X.
*/

#include <iomanip>
#include <iostream>
#include <fstream>

using namespace std;

// ��������� ������ ����������
struct Employee {
	char surname[15];    // ������� �����������
	char department[15]; // �������� ������
	int year;       // ��� ��������
	int experience;  // ���� ������
	char post[15];       // ���������
	int salary;     // �����
};

// ����� ������ ���������� �� �����
ostream& operator<<(ostream& stream, const Employee& e) {
	stream << left << setw(15) << e.surname
		<< setw(15) << e.department
		<< setw(5) << e.year << setw(3)
		<< e.experience << setw(25)
		<< e.post << setw(7) << e.salary;
	return stream;
}

// ����������� ������ �� ���������� ����� � ��������
size_t copyTxtToBin(const char* inFileName, const char* outFileName) {
	Employee e;
	memset(&e, 0, sizeof(e));
	size_t count = 0;

	ifstream in(inFileName);
	if (!in) {
		cerr << "�� ������� ������� ���� \"" << inFileName << "\"" << endl;
		return 1;
	}

	ofstream out(outFileName, ios::binary);
	if (!out) {
		cerr << "�� ������� ������� ���� \"" << outFileName << "\"" << endl;
		return 1;
	}

	while (!in.eof()) {
		in >> e.surname >> e.department >> e.year >> e.experience >> e.post >> e.salary;
		out.write((char*)&e, sizeof(e));
		count++;
	}

	return count;
}

// ���������� ����� � ������� �����������:
// ��� ��������� ��������� ���� ����������� �������� ���������� �� ������� "������"
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

// ����� �� ����� �� ����� �����������, ��������������� �������� ������� filter()
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

	// �������� �������� ���������� �����
	const char* inFileName = "data.txt";
	// �������� ��������� ��������� �����
	const char* outFileName = "data.bin";
	
	// ��������� ������ �� ���������� ����� � ��������
	size_t count = copyTxtToBin(inFileName, outFileName);

	// ���������� ����� � ������� �����������:
	// ��� ����� ���������� �������, ������������
	// ���� �������� ����������� �� �����������
	// ����������� ���������� ������� ("������") ������������ ����� �� ����� � �������������
	sort(outFileName, count, [](const Employee& a, const Employee& b) { return a.year < b.year; });

	// ���� ������������� ������ ��� ������ �����������
	int X = 0;
	cout << "������� ������������ �����: ";
	cin >> X;

	// ����� �� ����� �����������, ��� ����� �� ��������� X:
	// ��� ����� ���������� �������, ������������ true, ����
	// ����� ���������� ����������� �� ���������� <= X
	// ����������� ���������� ������� ("������") ������������ ����� �� ����� � �������������
	printFilteredStaff(outFileName, count, [X](const Employee& e) { return e.salary <= X; });

	return 0;
}
