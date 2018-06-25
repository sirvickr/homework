#include <iostream>
#include <locale>
#include "tree.h"

using namespace std;

// Поменять местами максимальный и минимальный элементы дерева
void Swap_Max_Min(btree *root) {
	if (!root)
		return;
	// спускаемся к минимальному элементу
	btree *pmin = root;
	while (1) {
		btree *left = pmin->left;
		if (!left) {
			break;
		}
		pmin = left;
	}
	// спускаемся к минимальному элементу
	btree *pmax = root;
	while (1) {
		btree *right = pmax->right;
		if (!right) {
			break;
		}
		pmax = right;
	}
	// меняем их местами
	swap(pmin->value, pmax->value);
}

int main()
{
	setlocale(LC_CTYPE, "Russian");
	btree *root = NULL;
#if 1
	const int size = 10;
	int a[size] = { 3, -1, 5, 9, 8, -7, -5, 2, 1, 4 };
	for(int i = 0; i < size; i++)
		Ins_Btree(a[i], &root);
#else
	cout << "Дерево. Ввод чисел:" << endl;
	int d;
	while (1) {
		cin >> d;
		if (d == 0)
			break;
		Ins_Btree(d, &root);
	}
#endif
	Print_Btree(root, 3);
	cout << endl;
	Swap_Max_Min(root);
	Print_Btree(root, 3);
	cout << endl;
	return 0;
}
