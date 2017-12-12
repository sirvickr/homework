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
	for(size_t i = 0; i < n; i++) { // вычисление суммы элементов
		if(a[i] < 0) {
			if(add) { // встретили второй отрицательный элемент
				add = false;
				break;
			} else { // встретили первый отрицательный элемент
				add = true;
			}
		} else if(add) {
			sum += a[i];
		}
	}
	// половина размера (включая центральный элемент, в случае
	size_t m = (n + 1) / 2; // нечетного числа элементов)
	for(size_t i = m, j = 0; i < n; i++) {
		if(fabs(a[i]) <= 1.0) {
			// элемент нужно переместить в первую массива
			// найдем первый подходящий для обмена из первой половины
			for(; j < m; j++)
				if(fabs(a[j]) > 1.0)
					break;
			if(j == m) {
				// больше менять не с чем, т.е. в первой половине нет
				// подходящих элементов (т.е. > 1)
				break;
			}
			// производим замену (можно std::swap())
			double temp = a[i];
			a[i] = a[j];
			a[j] = temp;
		} // if(нужно переместить элемент)
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
 