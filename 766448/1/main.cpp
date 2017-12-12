#include <iostream>
#include <string>
#include <math.h>
#pragma hdrstop
#pragma argsused
using namespace std;
int main(int argc, char* argv[])
{
	size_t n;
	cout << "Input array size: ";
	cin >> n;
	double* a = new double[n];
	cout << "Input array items: ";
	for(size_t i = 0; i < n; i++)
		cin >> a[i];
	double sum = 0.0;
	bool add = false;
	for(size_t i = 0; i < n; i++) { // ���������� ����� ���������
		if(a[i] < 0) {
			if(add) { // ��������� ������ ������������� �������
				add = false;
				break;
			} else { // ��������� ������ ������������� �������
				add = true;
			}
		} else if(add) {
			sum += a[i];
		}
	}
	// �������� ������� (������� ����������� �������, � ������
	size_t m = (n + 1) / 2; // ��������� ����� ���������)
	for(size_t i = m, j = 0; i < n; i++) {
		if(fabs(a[i]) <= 1.0) {
			// ������� ����� ����������� � ������ �������
			// ������ ������ ���������� ��� ������ �� ������ ��������
			for(; j < m; j++)
				if(fabs(a[j]) > 1.0)
					break;
			if(j == m) {
				// ������ ������ �� � ���, �.�. � ������ �������� ���
				// ���������� ��������� (�.�. > 1)
				break;
			}
			// ���������� ������ (����� std::swap())
			double temp = a[i];
			a[i] = a[j];
			a[j] = temp;
		} // if(����� ����������� �������)
	} // for(i)
	if(add)
		cout << "single negative item" << endl;
	else
		cout << "sum between first negatives: " << sum << endl;
	cout << "sorted array:";
	for(size_t i = 0; i < n; i++)
		cout << " " << a[i];
	cout << endl;
	delete[] a;
	cout << "press enter to leave";
	string bye;
	getline(cin, bye);
	char ch = getc(stdin);
	return 0;
}
//---------------------------------------------------------------------------
 