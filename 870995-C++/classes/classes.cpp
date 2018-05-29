/* 5. ���������� ����� "���������� ������". � ����� �������� ��� ������������:
1) �������� ����������� ������� �� ���������� ���������; 2) ���������� �����������.
���������� �������-�����: ����� �� ����� ��������� �������, ��������� �������� 
�������� �� ��� ����������� ������, ���������� ������������ ���������, �������� �������
�� ������ ������ 5
*/

#include <iostream>
#include <time.h>

using namespace std;

// ����� ����������� �������
template<class T>
class Array {
public:
	// ����������� �� ���������� ���������
	Array(size_t size, T init = T()) : data(nullptr), size(size) {
		if (size) {
			data = new T[size];
			for (size_t i = 0; i < size; i++) {
				data[i] = init;
			}
		}
	}
	// ����������� �����������
	Array(const Array& a) {
		if (size < a.size) {
			if (data)
				delete[] data;
			data = new T[a.size];
		}
		size = a.size;
		for (size_t i = 0; i < size; i++) {
			data[i] = a.data[i];
		}
	}
	// ����������
	~Array() {
		if (data) {
			delete[] data;
		}
	}
	// ������� ������� � ���������� ���������
	size_t Size() const {
		return size;
	}
	// ����� �� ����� ��������� �������
	void Print() const {
		for (size_t i = 0; i < size; i++)
			cout << data[i] << " ";
		cout << endl;
	}
	// ��������� �������� �������� �� ��� ����������� ������ (�������)
	T& GetAt(size_t index) {
		return data[index];
	}
	// ��������� �������� �������� �� ��� ����������� ������ (��������)
	T& operator[](size_t index) {
		return data[index];
	}
	// ������������ ���������, �������� ������� �� ������ ������ 5
	T ProductLess5() const {
		T p = T(1);
		for (size_t i = 0; i < size; i++)
			if(abs(data[i]) < T(5))
				p *= data[i];
		return p;
	}
private:
	T* data;     // ��������� �� ������ � �������
	size_t size; // ������ ������� (���������� ���������)
};

int main()
{
	setlocale(LC_CTYPE, "Russian");

	// ���������� ��������� ���� ��� �������������� �������
	typedef Array<int> IntArray;
	// �������������� ��������� ��������� ����� ������� ��������
	srand(time(NULL));
	// ��������� ������������� ������ �� 10 ���������
	IntArray array(10);
	// ��������� ��� ���������� ���������� �� -9 �� 9
	for (size_t i = 0; i < array.Size(); i++)
		array[i]  = (rand() % 19) - 9;
	// ������������� ������������ ������
	array.Print();
	// ������� ������������ ���������, �������� ������� �� ������ ������ 5
	cout << "������������ ���������, �������� ������� �� ������ ������ 5: " << array.ProductLess5() << endl;

	return 0;
}
