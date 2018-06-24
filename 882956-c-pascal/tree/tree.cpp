#include <iostream>
#include <iomanip>
#include "tree.h"

// Включение вершины в дерево
void Ins_Btree(int val,	btree **q)
{
	if (*q == NULL) {
		// Нашли место для добавления
		*q = new btree;
		(*q)->left = NULL;
		(*q)->right = NULL;
		(*q)->value = val;
		return;
	}
	if ((*q)->value > val)
		// Добавляем в левое поддерево
		Ins_Btree(val, &(*q)->left);
	else
		// Добавляем в правое поддерево
		Ins_Btree(val, &(*q)->right);
}

// Вывод содержимого дерева
void Print_Btree(btree *p, int w)
{
	if (p == NULL)
		return;
	Print_Btree(p->left, w);
	std::cout << " " << std::setw(w) << p->value;
	Print_Btree(p->right, w);
}
